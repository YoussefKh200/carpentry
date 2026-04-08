#include "clientcrud.h"

#include <QDate>
#include <QRegularExpression>
#include <QSqlError>
#include <QSqlQuery>

#include "../db/connection.h"

static QString resolveClientTableName(const QSqlDatabase &db)
{
    QSqlQuery q(db);
    if (q.exec("SELECT TABLE_NAME FROM USER_TABLES WHERE TABLE_NAME IN ('CLIENT', 'CLIENTS') ORDER BY CASE TABLE_NAME WHEN 'CLIENT' THEN 0 ELSE 1 END")) {
        if (q.next()) return q.value(0).toString();
    }
    return "CLIENT";
}

static QString resolveClientStatusColumn(const QSqlDatabase &db, const QString &tableName)
{
    QSqlQuery q(db);
    q.prepare(
        "SELECT COLUMN_NAME FROM USER_TAB_COLUMNS "
        "WHERE TABLE_NAME=:t AND COLUMN_NAME IN ('STATUT', 'STATUS', 'ETAT') "
        "ORDER BY CASE COLUMN_NAME WHEN 'STATUT' THEN 0 WHEN 'STATUS' THEN 1 ELSE 2 END"
    );
    q.bindValue(":t", tableName.toUpper());
    if (q.exec() && q.next()) return q.value(0).toString();
    return "STATUT";
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

    if (client.adresse.trimmed().isEmpty()) return "Adresse obligatoire";
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
    const QDate parsedDate = QDate::fromString(client.dateInscription.trimmed(), "yyyy-MM-dd");
    const QString tableName = resolveClientTableName(Connection::instance()->database());
    const QString statusCol = resolveClientStatusColumn(Connection::instance()->database(), tableName);

    QSqlQuery q(Connection::instance()->database());
    q.prepare("INSERT INTO " + tableName + " (NOM, PRENOM, TEL, MAIL, ADRESSE, DATE_INSCRIPTION, " + statusCol + ") VALUES (?, ?, ?, ?, ?, ?, ?)");
    q.addBindValue(client.nom.trimmed());
    q.addBindValue(client.prenom.trimmed());
    q.addBindValue(client.tel.trimmed());
    q.addBindValue(client.mail.trimmed());
    q.addBindValue(client.adresse.trimmed());
    q.addBindValue(parsedDate);
    q.addBindValue(client.statut.trimmed());

    if (!q.exec()) {
        // Some Oracle ODBC drivers intermittently fail prepared statements (S1010).
        // Fallback to a direct SQL statement with escaped values.
        auto esc = [](QString v) {
            return v.trimmed().replace("'", "''");
        };
        QSqlQuery fallback(Connection::instance()->database());
        const QString sql =
            "INSERT INTO " + tableName + " (NOM, PRENOM, TEL, MAIL, ADRESSE, DATE_INSCRIPTION, " + statusCol + ") VALUES ('" +
            esc(client.nom) + "', '" +
            esc(client.prenom) + "', '" +
            esc(client.tel) + "', '" +
            esc(client.mail) + "', '" +
            esc(client.adresse) + "', DATE '" +
            parsedDate.toString("yyyy-MM-dd") + "', '" +
            esc(client.statut) + "')";

        if (!fallback.exec(sql)) {
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
    const QDate parsedDate = QDate::fromString(client.dateInscription.trimmed(), "yyyy-MM-dd");
    const QString tableName = resolveClientTableName(Connection::instance()->database());
    const QString statusCol = resolveClientStatusColumn(Connection::instance()->database(), tableName);

    QSqlQuery q(Connection::instance()->database());
    q.prepare("UPDATE " + tableName + " SET NOM=?, PRENOM=?, TEL=?, MAIL=?, ADRESSE=?, DATE_INSCRIPTION=?, " + statusCol + "=? WHERE ID_CLIENT=?");
    q.addBindValue(client.nom.trimmed());
    q.addBindValue(client.prenom.trimmed());
    q.addBindValue(client.tel.trimmed());
    q.addBindValue(client.mail.trimmed());
    q.addBindValue(client.adresse.trimmed());
    q.addBindValue(parsedDate);
    q.addBindValue(client.statut.trimmed());
    q.addBindValue(client.id);

    if (!q.exec()) {
        auto esc = [](QString v) {
            return v.trimmed().replace("'", "''");
        };
        QSqlQuery fallback(Connection::instance()->database());
        const QString sql =
            "UPDATE " + tableName + " SET NOM='" + esc(client.nom) +
            "', PRENOM='" + esc(client.prenom) +
            "', TEL='" + esc(client.tel) +
            "', MAIL='" + esc(client.mail) +
            "', ADRESSE='" + esc(client.adresse) +
            "', DATE_INSCRIPTION=DATE '" + parsedDate.toString("yyyy-MM-dd") +
            "', " + statusCol + "='" + esc(client.statut) +
            "' WHERE ID_CLIENT=" + QString::number(client.id);

        if (!fallback.exec(sql)) {
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
    const QString tableName = resolveClientTableName(Connection::instance()->database());

    QSqlQuery q(Connection::instance()->database());
    q.prepare("DELETE FROM " + tableName + " WHERE ID_CLIENT=:id");
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
    const QString tableName = resolveClientTableName(Connection::instance()->database());
    const QString statusCol = resolveClientStatusColumn(Connection::instance()->database(), tableName);
    QString sql = "SELECT ID_CLIENT, NOM, PRENOM, TEL, MAIL, ADRESSE, DATE_INSCRIPTION, " + statusCol + " AS STATUT FROM " + tableName;
    if (orderBy == "nom") sql += " ORDER BY NOM";
    if (orderBy == "date") sql += " ORDER BY DATE_INSCRIPTION DESC";

    QSqlQuery q(Connection::instance()->database());
    if (!q.exec(sql)) return out;
    while (q.next()) out.append(fromQuery(q));
    return out;
}

QList<ClientData> ClientCrud::searchClients(const QString &text, const QString &orderBy)
{
    if (!Connection::instance()->createConnect()) return {};
    if (text.trimmed().isEmpty()) return getAllClients(orderBy);

    QList<ClientData> out;
    const QString tableName = resolveClientTableName(Connection::instance()->database());
    const QString statusCol = resolveClientStatusColumn(Connection::instance()->database(), tableName);
    QString sql = "SELECT ID_CLIENT, NOM, PRENOM, TEL, MAIL, ADRESSE, DATE_INSCRIPTION, " + statusCol + " AS STATUT FROM " + tableName + " WHERE LOWER(NOM) LIKE :s OR LOWER(PRENOM) LIKE :s OR LOWER(TEL) LIKE :s OR LOWER(MAIL) LIKE :s";
    if (orderBy == "nom") sql += " ORDER BY NOM";
    if (orderBy == "date") sql += " ORDER BY DATE_INSCRIPTION DESC";

    QSqlQuery q(Connection::instance()->database());
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
    const QString tableName = resolveClientTableName(Connection::instance()->database());
    QSqlQuery q(Connection::instance()->database());
    if (!q.exec("SELECT ADRESSE, COUNT(*) FROM " + tableName + " GROUP BY ADRESSE ORDER BY ADRESSE")) return out;

    while (q.next()) {
        out.append(qMakePair(q.value(0).toString(), q.value(1).toInt()));
    }
    return out;
}
