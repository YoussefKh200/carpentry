#include "clientcrud.h"

#include <QDate>
#include <QRegularExpression>
#include <QSqlError>
#include <QSqlQuery>

#include "../db/connection.h"

namespace {

struct ClientSchema {
    QString table;
    QString id;
    QString nom;
    QString prenom;
    QString tel;
    QString mail;
    QString date;
    QString statut;
    QString adresse; // optional in DB
};

QString pickColumn(const QStringList &columns, const QStringList &tokens, const QString &startsWith = QString())
{
    for (const QString &col : columns) {
        const QString u = col.toUpper();
        if (!startsWith.isEmpty() && !u.startsWith(startsWith)) continue;

        bool ok = true;
        for (const QString &token : tokens) {
            if (!u.contains(token)) {
                ok = false;
                break;
            }
        }
        if (ok) return col;
    }
    return QString();
}

ClientSchema resolveClientSchema(const QSqlDatabase &db)
{
    ClientSchema s;
    s.table = "CLIENT";

    QSqlQuery tq(db);
    if (tq.exec("SELECT TABLE_NAME FROM USER_TABLES WHERE TABLE_NAME IN ('CLIENTS', 'CLIENT') ORDER BY CASE TABLE_NAME WHEN 'CLIENTS' THEN 0 ELSE 1 END")) {
        if (tq.next()) s.table = tq.value(0).toString();
    }

    QStringList cols;
    QSqlQuery cq(db);
    cq.prepare("SELECT COLUMN_NAME FROM USER_TAB_COLUMNS WHERE TABLE_NAME=:t ORDER BY COLUMN_ID");
    cq.bindValue(":t", s.table.toUpper());
    if (cq.exec()) {
        while (cq.next()) cols << cq.value(0).toString();
    }

    s.id = pickColumn(cols, {"ID"}, "ID");
    s.prenom = pickColumn(cols, {"NOM"}, "PR");
    s.nom = pickColumn(cols, {"NOM"}, "NOM");
    s.tel = pickColumn(cols, {"TEL"});
    if (s.tel.isEmpty()) s.tel = pickColumn(cols, {"PHONE"});
    s.mail = pickColumn(cols, {"MAIL"});
    s.date = pickColumn(cols, {"DATE"});
    s.statut = pickColumn(cols, {"STATUT"});
    if (s.statut.isEmpty()) s.statut = pickColumn(cols, {"STATUS"});
    if (s.statut.isEmpty()) s.statut = pickColumn(cols, {"ETAT"});
    s.adresse = pickColumn(cols, {"ADR"});

    if (s.id.isEmpty()) s.id = "ID_CLIENT";
    if (s.nom.isEmpty()) s.nom = "NOM";
    if (s.prenom.isEmpty()) s.prenom = "PRENOM";
    if (s.tel.isEmpty()) s.tel = "TEL";
    if (s.mail.isEmpty()) s.mail = "MAIL";
    if (s.date.isEmpty()) s.date = "DATE_INSCRIPTION";
    if (s.statut.isEmpty()) s.statut = "STATUT";

    return s;
}

int nextClientId(const QSqlDatabase &db, const ClientSchema &s)
{
    QSqlQuery q(db);
    if (!q.exec("SELECT NVL(MAX(" + s.id + "), 0) + 1 FROM " + s.table)) return 1;
    if (!q.next()) return 1;
    return q.value(0).toInt();
}

QString esc(const QString &v)
{
    return v.trimmed().replace("'", "''");
}

}

ClientCrud::ClientCrud(QObject *parent) : QObject(parent)
{
}

QString ClientCrud::validateClient(const ClientData &client)
{
    if (client.nom.trimmed().isEmpty()) return "Nom obligatoire";
    if (client.prenom.trimmed().isEmpty()) return "Prenom obligatoire";

    const QRegularExpression telRe("^\\d{8}$");
    if (!telRe.match(client.tel.trimmed()).hasMatch()) return "Telephone invalide (8 chiffres)";

    const QString mail = client.mail.trimmed();
    if (mail.isEmpty() || !mail.contains('@') || !mail.contains('.')) return "Email invalide";

    const QDate parsedDate = QDate::fromString(client.dateInscription.trimmed(), "yyyy-MM-dd");
    if (!parsedDate.isValid()) return "Date inscription invalide (format YYYY-MM-DD)";
    return QString();
}

ClientData ClientCrud::fromQuery(QSqlQuery &q)
{
    ClientData c;
    c.id = q.value(0).toInt();
    c.nom = q.value(1).toString();
    c.prenom = q.value(2).toString();
    c.tel = q.value(3).toString();
    c.mail = q.value(4).toString();
    c.adresse = q.value(5).toString();
    c.dateInscription = q.value(6).toString();
    c.statut = q.value(7).toString();
    return c;
}

bool ClientCrud::addClient(const ClientData &client, QString &error)
{
    if (!Connection::instance()->createConnect()) {
        error = "Connexion base de donnees echouee";
        return false;
    }
    error = validateClient(client);
    if (!error.isEmpty()) return false;

    const QSqlDatabase db = Connection::instance()->database();
    const ClientSchema s = resolveClientSchema(db);
    const QDate parsedDate = QDate::fromString(client.dateInscription.trimmed(), "yyyy-MM-dd");
    const int newId = nextClientId(db, s);

    QStringList cols {s.id, s.nom, s.prenom, s.tel, s.mail, s.date, s.statut};
    QStringList placeholders {"?", "?", "?", "?", "?", "?", "?"};
    if (!s.adresse.isEmpty()) {
        cols << s.adresse;
        placeholders << "?";
    }

    QSqlQuery q(db);
    q.prepare("INSERT INTO " + s.table + " (" + cols.join(", ") + ") VALUES (" + placeholders.join(", ") + ")");
    q.addBindValue(newId);
    q.addBindValue(client.nom.trimmed());
    q.addBindValue(client.prenom.trimmed());
    q.addBindValue(client.tel.trimmed());
    q.addBindValue(client.mail.trimmed());
    q.addBindValue(parsedDate);
    q.addBindValue(client.statut.trimmed());
    if (!s.adresse.isEmpty()) q.addBindValue(client.adresse.trimmed());

    if (!q.exec()) {
        // Fallback path for flaky Oracle ODBC prepared execution.
        QString sql = "INSERT INTO " + s.table + " (" + cols.join(", ") + ") VALUES (" +
                      QString::number(newId) + ", '" + esc(client.nom) + "', '" + esc(client.prenom) + "', '" +
                      esc(client.tel) + "', '" + esc(client.mail) + "', DATE '" +
                      parsedDate.toString("yyyy-MM-dd") + "', '" + esc(client.statut) + "'";
        if (!s.adresse.isEmpty()) sql += ", '" + esc(client.adresse) + "'";
        sql += ")";

        QSqlQuery fallback(db);
        if (!fallback.exec(sql)) {
            if (fallback.lastError().nativeErrorCode() == "3114") {
                Connection::instance()->createConnect();
            }
            error = fallback.lastError().text();
            return false;
        }
    }
    return true;
}

bool ClientCrud::updateClient(const ClientData &client, QString &error)
{
    if (!Connection::instance()->createConnect()) {
        error = "Connexion base de donnees echouee";
        return false;
    }
    if (client.id <= 0) {
        error = "ID client invalide";
        return false;
    }

    error = validateClient(client);
    if (!error.isEmpty()) return false;

    const QSqlDatabase db = Connection::instance()->database();
    const ClientSchema s = resolveClientSchema(db);
    const QDate parsedDate = QDate::fromString(client.dateInscription.trimmed(), "yyyy-MM-dd");

    QStringList sets {
        s.nom + "=?",
        s.prenom + "=?",
        s.tel + "=?",
        s.mail + "=?",
        s.date + "=?",
        s.statut + "=?"
    };
    if (!s.adresse.isEmpty()) sets << s.adresse + "=?";

    QSqlQuery q(db);
    q.prepare("UPDATE " + s.table + " SET " + sets.join(", ") + " WHERE " + s.id + "=?");
    q.addBindValue(client.nom.trimmed());
    q.addBindValue(client.prenom.trimmed());
    q.addBindValue(client.tel.trimmed());
    q.addBindValue(client.mail.trimmed());
    q.addBindValue(parsedDate);
    q.addBindValue(client.statut.trimmed());
    if (!s.adresse.isEmpty()) q.addBindValue(client.adresse.trimmed());
    q.addBindValue(client.id);

    if (!q.exec()) {
        QString sql = "UPDATE " + s.table + " SET " +
                      s.nom + "='" + esc(client.nom) + "', " +
                      s.prenom + "='" + esc(client.prenom) + "', " +
                      s.tel + "='" + esc(client.tel) + "', " +
                      s.mail + "='" + esc(client.mail) + "', " +
                      s.date + "=DATE '" + parsedDate.toString("yyyy-MM-dd") + "', " +
                      s.statut + "='" + esc(client.statut) + "'";
        if (!s.adresse.isEmpty()) sql += ", " + s.adresse + "='" + esc(client.adresse) + "'";
        sql += " WHERE " + s.id + "=" + QString::number(client.id);

        QSqlQuery fallback(db);
        if (!fallback.exec(sql)) {
            if (fallback.lastError().nativeErrorCode() == "3114") {
                Connection::instance()->createConnect();
            }
            error = fallback.lastError().text();
            return false;
        }
    }
    return true;
}

bool ClientCrud::deleteClient(int id, QString &error)
{
    if (!Connection::instance()->createConnect()) {
        error = "Connexion base de donnees echouee";
        return false;
    }
    if (id <= 0) {
        error = "ID client invalide";
        return false;
    }

    const QSqlDatabase db = Connection::instance()->database();
    const ClientSchema s = resolveClientSchema(db);
    QSqlQuery q(db);
    q.prepare("DELETE FROM " + s.table + " WHERE " + s.id + "=:id");
    q.bindValue(":id", id);
    if (!q.exec()) {
        error = q.lastError().text();
        return false;
    }
    return true;
}

QList<ClientData> ClientCrud::getAllClients(const QString &orderBy)
{
    QList<ClientData> out;
    if (!Connection::instance()->createConnect()) return out;

    const QSqlDatabase db = Connection::instance()->database();
    const ClientSchema s = resolveClientSchema(db);
    QString sql =
        "SELECT " + s.id + ", " + s.nom + ", " + s.prenom + ", " + s.tel + ", " + s.mail + ", " +
        (s.adresse.isEmpty() ? "''" : s.adresse) + ", " + s.date + ", " + s.statut +
        " FROM " + s.table;

    if (orderBy == "nom") sql += " ORDER BY " + s.nom;
    else if (orderBy == "date") sql += " ORDER BY " + s.date + " DESC";
    else sql += " ORDER BY " + s.id;

    QSqlQuery q(db);
    if (!q.exec(sql)) return out;
    while (q.next()) out.append(fromQuery(q));
    return out;
}

QList<ClientData> ClientCrud::searchClients(const QString &text, const QString &orderBy)
{
    if (!Connection::instance()->createConnect()) return {};
    if (text.trimmed().isEmpty()) return getAllClients(orderBy);

    QList<ClientData> out;
    const QSqlDatabase db = Connection::instance()->database();
    const ClientSchema s = resolveClientSchema(db);

    QString sql =
        "SELECT " + s.id + ", " + s.nom + ", " + s.prenom + ", " + s.tel + ", " + s.mail + ", " +
        (s.adresse.isEmpty() ? "''" : s.adresse) + ", " + s.date + ", " + s.statut +
        " FROM " + s.table + " WHERE LOWER(" + s.nom + ") LIKE :s OR LOWER(" + s.prenom + ") LIKE :s OR LOWER(" + s.tel + ") LIKE :s OR LOWER(" + s.mail + ") LIKE :s";

    if (orderBy == "nom") sql += " ORDER BY " + s.nom;
    else if (orderBy == "date") sql += " ORDER BY " + s.date + " DESC";
    else sql += " ORDER BY " + s.id;

    QSqlQuery q(db);
    q.prepare(sql);
    q.bindValue(":s", "%" + text.toLower() + "%");
    if (!q.exec()) return out;
    while (q.next()) out.append(fromQuery(q));
    return out;
}

QList<QPair<QString, int>> ClientCrud::clientsByAddress()
{
    QList<QPair<QString, int>> out;
    if (!Connection::instance()->createConnect()) return out;

    const QSqlDatabase db = Connection::instance()->database();
    const ClientSchema s = resolveClientSchema(db);
    const QString groupCol = s.adresse.isEmpty() ? s.statut : s.adresse;

    QSqlQuery q(db);
    if (!q.exec("SELECT " + groupCol + ", COUNT(*) FROM " + s.table + " GROUP BY " + groupCol + " ORDER BY " + groupCol)) return out;

    while (q.next()) {
        out.append(qMakePair(q.value(0).toString(), q.value(1).toInt()));
    }
    return out;
}
