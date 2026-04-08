#include "fournisseurcrud.h"

#include <QRegularExpression>
#include <QSqlError>
#include <QSqlQuery>

#include "../db/connection.h"

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
    if (!f.date.isValid()) return "Date invalide";
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

    QSqlQuery q(Connection::instance()->database());
    q.prepare("INSERT INTO FOURNISSEURS (NOM_FOURNISSEURS, PRENOM_FOURNISSEURS, MAIL_FOURNISSEURS, ADRESSE_FOURNISSEURS, TEL_FOURNISSEURS, QUANTITE_FOURNISSEURS, TYPE_FOURNISSEURS, DATE_FOURNISSEURS) VALUES (:nom, :prenom, :mail, :adresse, :tel, :qte, :type, :datef)");
    q.bindValue(":nom", f.nom.trimmed());
    q.bindValue(":prenom", f.prenom.trimmed());
    q.bindValue(":mail", f.mail.trimmed());
    q.bindValue(":adresse", f.adresse.trimmed());
    q.bindValue(":tel", f.tel.trimmed());
    q.bindValue(":qte", f.quantite);
    q.bindValue(":type", f.type.trimmed());
    q.bindValue(":datef", f.date);

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

    QSqlQuery q(Connection::instance()->database());
    q.prepare("UPDATE FOURNISSEURS SET NOM_FOURNISSEURS=:nom, PRENOM_FOURNISSEURS=:prenom, MAIL_FOURNISSEURS=:mail, ADRESSE_FOURNISSEURS=:adresse, TEL_FOURNISSEURS=:tel, QUANTITE_FOURNISSEURS=:qte, TYPE_FOURNISSEURS=:type, DATE_FOURNISSEURS=:datef WHERE ID_FOURNISSEURS=:id");
    q.bindValue(":id", f.id);
    q.bindValue(":nom", f.nom.trimmed());
    q.bindValue(":prenom", f.prenom.trimmed());
    q.bindValue(":mail", f.mail.trimmed());
    q.bindValue(":adresse", f.adresse.trimmed());
    q.bindValue(":tel", f.tel.trimmed());
    q.bindValue(":qte", f.quantite);
    q.bindValue(":type", f.type.trimmed());
    q.bindValue(":datef", f.date);

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

    QSqlQuery q(Connection::instance()->database());
    q.prepare("DELETE FROM FOURNISSEURS WHERE ID_FOURNISSEURS=:id");
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

    QString order = "ID_FOURNISSEURS";
    if (orderBy == "nom") order = "NOM_FOURNISSEURS";
    else if (orderBy == "type") order = "TYPE_FOURNISSEURS";
    else if (orderBy == "quantite") order = "QUANTITE_FOURNISSEURS DESC";
    else if (orderBy == "date") order = "DATE_FOURNISSEURS DESC";

    QSqlQuery q(Connection::instance()->database());
    const QString sql =
        "SELECT ID_FOURNISSEURS, NOM_FOURNISSEURS, PRENOM_FOURNISSEURS, MAIL_FOURNISSEURS, ADRESSE_FOURNISSEURS, TEL_FOURNISSEURS, QUANTITE_FOURNISSEURS, TYPE_FOURNISSEURS, DATE_FOURNISSEURS "
        "FROM FOURNISSEURS ORDER BY " + order;
    if (!q.exec(sql)) {
        return out;
    }

    while (q.next()) out.append(fromQuery(q));
    return out;
}

QList<FournisseurData> FournisseurCrud::searchFournisseurs(const QString &typeText, const QString &orderBy)
{
    if (!Connection::instance()->createConnect()) return {};
    if (typeText.trimmed().isEmpty()) return getAllFournisseurs(orderBy);

    QList<FournisseurData> out;
    QString order = "ID_FOURNISSEURS";
    if (orderBy == "nom") order = "NOM_FOURNISSEURS";
    else if (orderBy == "type") order = "TYPE_FOURNISSEURS";
    else if (orderBy == "quantite") order = "QUANTITE_FOURNISSEURS DESC";
    else if (orderBy == "date") order = "DATE_FOURNISSEURS DESC";

    QSqlQuery q(Connection::instance()->database());
    q.prepare(
        "SELECT ID_FOURNISSEURS, NOM_FOURNISSEURS, PRENOM_FOURNISSEURS, MAIL_FOURNISSEURS, ADRESSE_FOURNISSEURS, TEL_FOURNISSEURS, QUANTITE_FOURNISSEURS, TYPE_FOURNISSEURS, DATE_FOURNISSEURS "
        "FROM FOURNISSEURS WHERE LOWER(TYPE_FOURNISSEURS) LIKE :s ORDER BY " + order
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
    QSqlQuery q(Connection::instance()->database());
    if (!q.exec("SELECT TYPE_FOURNISSEURS, SUM(QUANTITE_FOURNISSEURS) FROM FOURNISSEURS GROUP BY TYPE_FOURNISSEURS ORDER BY TYPE_FOURNISSEURS")) {
        return out;
    }

    while (q.next()) {
        out.append(qMakePair(q.value(0).toString(), q.value(1).toInt()));
    }
    return out;
}
