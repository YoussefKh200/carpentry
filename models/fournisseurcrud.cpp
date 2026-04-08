#include "fournisseurcrud.h"

#include <QRegularExpression>
#include <QSqlError>
#include <QSqlQuery>

#include "../db/connection.h"

namespace {

struct FournisseurSchema {
    QString table;
    QString id;
    QString nom;
    QString prenom;
    QString mail;
    QString adresse;
    QString tel;
    QString quantite;
    QString type;
    QString date; // optional
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

FournisseurSchema resolveSchema(const QSqlDatabase &db)
{
    FournisseurSchema s;
    s.table = "FOURNISSEURS";

    QStringList cols;
    QSqlQuery cq(db);
    cq.prepare("SELECT COLUMN_NAME FROM USER_TAB_COLUMNS WHERE TABLE_NAME=:t ORDER BY COLUMN_ID");
    cq.bindValue(":t", s.table);
    if (cq.exec()) {
        while (cq.next()) cols << cq.value(0).toString();
    }

    s.id = pickColumn(cols, {"ID"}, "ID");
    s.nom = pickColumn(cols, {"NOM"}, "NOM");
    s.prenom = pickColumn(cols, {"NOM"}, "PR");
    s.mail = pickColumn(cols, {"MAIL"});
    s.adresse = pickColumn(cols, {"ADR"});
    s.tel = pickColumn(cols, {"TEL"});
    s.quantite = pickColumn(cols, {"QUANTIT"});
    s.type = pickColumn(cols, {"TYPE"});
    s.date = pickColumn(cols, {"DATE"});

    if (s.id.isEmpty()) s.id = "ID_FOURNISSEURS";
    if (s.nom.isEmpty()) s.nom = "NOM_FOURNISSEURS";
    if (s.prenom.isEmpty()) s.prenom = "PRENOM_FOURNISSEURS";
    if (s.mail.isEmpty()) s.mail = "MAIL_FOURNISSEURS";
    if (s.adresse.isEmpty()) s.adresse = "ADRESSE_FOURNISSEURS";
    if (s.tel.isEmpty()) s.tel = "TEL_FOURNISSEURS";
    if (s.quantite.isEmpty()) s.quantite = "QUANTITE_FOURNISSEURS";
    if (s.type.isEmpty()) s.type = "TYPE_FOURNISSEURS";
    return s;
}

int nextId(const QSqlDatabase &db, const FournisseurSchema &s)
{
    QSqlQuery q(db);
    if (!q.exec("SELECT NVL(MAX(" + s.id + "), 0) + 1 FROM " + s.table)) return 1;
    if (!q.next()) return 1;
    return q.value(0).toInt();
}

}

FournisseurCrud::FournisseurCrud(QObject *parent) : QObject(parent)
{
}

QString FournisseurCrud::validateFournisseur(const FournisseurData &f)
{
    if (f.nom.trimmed().isEmpty()) return "Nom obligatoire";
    if (f.prenom.trimmed().isEmpty()) return "Prenom obligatoire";
    if (f.mail.trimmed().isEmpty() || !f.mail.contains('@') || !f.mail.contains('.')) return "Email invalide";
    if (f.adresse.trimmed().isEmpty()) return "Adresse obligatoire";

    const QRegularExpression telRe("^\\d{8}$");
    if (!telRe.match(f.tel.trimmed()).hasMatch()) return "Telephone invalide (8 chiffres)";

    if (f.quantite < 0) return "Quantite invalide";
    if (f.type.trimmed().isEmpty()) return "Type obligatoire";
    return QString();
}

FournisseurData FournisseurCrud::fromQuery(QSqlQuery &q)
{
    FournisseurData f;
    f.id = q.value(0).toInt();
    f.nom = q.value(1).toString();
    f.prenom = q.value(2).toString();
    f.mail = q.value(3).toString();
    f.adresse = q.value(4).toString();
    f.tel = q.value(5).toString();
    f.quantite = q.value(6).toInt();
    f.type = q.value(7).toString();
    f.date = q.value(8).toDate();
    return f;
}

bool FournisseurCrud::addFournisseur(const FournisseurData &f, QString &error)
{
    if (!Connection::instance()->createConnect()) {
        error = "Connexion base de donnees echouee";
        return false;
    }
    error = validateFournisseur(f);
    if (!error.isEmpty()) return false;

    const QSqlDatabase db = Connection::instance()->database();
    const FournisseurSchema s = resolveSchema(db);
    const int id = nextId(db, s);

    QStringList cols {s.id, s.nom, s.prenom, s.mail, s.adresse, s.tel, s.quantite, s.type};
    QStringList ph {"?", "?", "?", "?", "?", "?", "?", "?"};
    if (!s.date.isEmpty()) {
        cols << s.date;
        ph << "?";
    }

    QSqlQuery q(db);
    q.prepare("INSERT INTO " + s.table + " (" + cols.join(", ") + ") VALUES (" + ph.join(", ") + ")");
    q.addBindValue(id);
    q.addBindValue(f.nom.trimmed());
    q.addBindValue(f.prenom.trimmed());
    q.addBindValue(f.mail.trimmed());
    q.addBindValue(f.adresse.trimmed());
    q.addBindValue(f.tel.trimmed());
    q.addBindValue(f.quantite);
    q.addBindValue(f.type.trimmed());
    if (!s.date.isEmpty()) q.addBindValue(f.date);

    if (!q.exec()) {
        error = q.lastError().text();
        return false;
    }
    return true;
}

bool FournisseurCrud::updateFournisseur(const FournisseurData &f, QString &error)
{
    if (!Connection::instance()->createConnect()) {
        error = "Connexion base de donnees echouee";
        return false;
    }
    if (f.id <= 0) {
        error = "ID fournisseur invalide";
        return false;
    }

    error = validateFournisseur(f);
    if (!error.isEmpty()) return false;

    const QSqlDatabase db = Connection::instance()->database();
    const FournisseurSchema s = resolveSchema(db);
    QStringList sets {s.nom + "=?", s.prenom + "=?", s.mail + "=?", s.adresse + "=?", s.tel + "=?", s.quantite + "=?", s.type + "=?"};
    if (!s.date.isEmpty()) sets << s.date + "=?";

    QSqlQuery q(db);
    q.prepare("UPDATE " + s.table + " SET " + sets.join(", ") + " WHERE " + s.id + "=?");
    q.addBindValue(f.nom.trimmed());
    q.addBindValue(f.prenom.trimmed());
    q.addBindValue(f.mail.trimmed());
    q.addBindValue(f.adresse.trimmed());
    q.addBindValue(f.tel.trimmed());
    q.addBindValue(f.quantite);
    q.addBindValue(f.type.trimmed());
    if (!s.date.isEmpty()) q.addBindValue(f.date);
    q.addBindValue(f.id);

    if (!q.exec()) {
        error = q.lastError().text();
        return false;
    }
    return true;
}

bool FournisseurCrud::deleteFournisseur(int id, QString &error)
{
    if (!Connection::instance()->createConnect()) {
        error = "Connexion base de donnees echouee";
        return false;
    }
    if (id <= 0) {
        error = "ID fournisseur invalide";
        return false;
    }

    const QSqlDatabase db = Connection::instance()->database();
    const FournisseurSchema s = resolveSchema(db);
    QSqlQuery q(db);
    q.prepare("DELETE FROM " + s.table + " WHERE " + s.id + "=:id");
    q.bindValue(":id", id);
    if (!q.exec()) {
        error = q.lastError().text();
        return false;
    }
    return true;
}

QList<FournisseurData> FournisseurCrud::getAllFournisseurs(const QString &orderBy)
{
    QList<FournisseurData> out;
    if (!Connection::instance()->createConnect()) return out;
    const QSqlDatabase db = Connection::instance()->database();
    const FournisseurSchema s = resolveSchema(db);

    QString order = s.id;
    if (orderBy == "nom") order = s.nom;
    else if (orderBy == "type") order = s.type;
    else if (orderBy == "quantite") order = s.quantite + " DESC";
    else if (orderBy == "date" && !s.date.isEmpty()) order = s.date + " DESC";

    const QString dateSelect = s.date.isEmpty() ? "NULL" : s.date;
    QSqlQuery q(db);
    const QString sql =
        "SELECT " + s.id + ", " + s.nom + ", " + s.prenom + ", " + s.mail + ", " + s.adresse + ", " + s.tel + ", " + s.quantite + ", " + s.type + ", " + dateSelect + " "
        "FROM " + s.table + " ORDER BY " + order;
    if (!q.exec(sql)) return out;
    while (q.next()) out.append(fromQuery(q));
    return out;
}

QList<FournisseurData> FournisseurCrud::searchFournisseurs(const QString &typeText, const QString &orderBy)
{
    if (!Connection::instance()->createConnect()) return {};
    if (typeText.trimmed().isEmpty()) return getAllFournisseurs(orderBy);

    QList<FournisseurData> out;
    const QSqlDatabase db = Connection::instance()->database();
    const FournisseurSchema s = resolveSchema(db);

    QString order = s.id;
    if (orderBy == "nom") order = s.nom;
    else if (orderBy == "type") order = s.type;
    else if (orderBy == "quantite") order = s.quantite + " DESC";
    else if (orderBy == "date" && !s.date.isEmpty()) order = s.date + " DESC";

    const QString dateSelect = s.date.isEmpty() ? "NULL" : s.date;
    QSqlQuery q(db);
    q.prepare(
        "SELECT " + s.id + ", " + s.nom + ", " + s.prenom + ", " + s.mail + ", " + s.adresse + ", " + s.tel + ", " + s.quantite + ", " + s.type + ", " + dateSelect + " "
        "FROM " + s.table + " WHERE LOWER(" + s.type + ") LIKE :s ORDER BY " + order
    );
    q.bindValue(":s", "%" + typeText.toLower() + "%");
    if (!q.exec()) return out;
    while (q.next()) out.append(fromQuery(q));
    return out;
}

QList<QPair<QString, int>> FournisseurCrud::quantiteByType()
{
    QList<QPair<QString, int>> out;
    if (!Connection::instance()->createConnect()) return out;
    const QSqlDatabase db = Connection::instance()->database();
    const FournisseurSchema s = resolveSchema(db);

    QSqlQuery q(db);
    if (!q.exec("SELECT " + s.type + ", SUM(" + s.quantite + ") FROM " + s.table + " GROUP BY " + s.type + " ORDER BY " + s.type)) return out;
    while (q.next()) out.append(qMakePair(q.value(0).toString(), q.value(1).toInt()));
    return out;
}
