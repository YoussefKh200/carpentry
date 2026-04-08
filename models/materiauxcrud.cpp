#include "materiauxcrud.h"

#include <QSqlError>
#include <QSqlQuery>

#include "../db/connection.h"

MateriauxCrud::MateriauxCrud(QObject *parent) : QObject(parent)
{
}

QString MateriauxCrud::validateMateriau(const MateriauData &m)
{
    if (m.id <= 0) return "ID invalide";
    if (m.nom.trimmed().isEmpty()) return "Nom obligatoire";
    if (m.type.trimmed().isEmpty()) return "Type obligatoire";
    if (m.quantite < 0) return "Quantite invalide";
    if (m.seuil < 0) return "Seuil invalide";
    if (m.fournisseur.trimmed().isEmpty()) return "Fournisseur obligatoire";
    return QString();
}

MateriauData MateriauxCrud::fromQuery(QSqlQuery &q)
{
    MateriauData m;
    m.id = q.value(0).toInt();
    m.nom = q.value(1).toString();
    m.type = q.value(2).toString();
    m.quantite = q.value(3).toInt();
    m.seuil = q.value(4).toInt();
    m.fournisseur = q.value(5).toString();
    return m;
}

bool MateriauxCrud::addMateriau(const MateriauData &m, QString &error)
{
    if (!Connection::instance()->createConnect()) {
        error = "Connexion base de donnees echouee";
        return false;
    }
    error = validateMateriau(m);
    if (!error.isEmpty()) return false;

    QSqlQuery q(Connection::instance()->database());
    q.prepare("INSERT INTO MATER (ID, NOM, \"TYPE\", QUANTITE, SEUIL, FOURNISSEUR) VALUES (:id, :nom, :type, :quantite, :seuil, :fournisseur)");
    q.bindValue(":id", m.id);
    q.bindValue(":nom", m.nom.trimmed());
    q.bindValue(":type", m.type.trimmed());
    q.bindValue(":quantite", m.quantite);
    q.bindValue(":seuil", m.seuil);
    q.bindValue(":fournisseur", m.fournisseur.trimmed());

    if (!q.exec()) {
        error = q.lastError().text();
        return false;
    }
    return true;
}

bool MateriauxCrud::updateMateriau(const MateriauData &m, QString &error)
{
    if (!Connection::instance()->createConnect()) {
        error = "Connexion base de donnees echouee";
        return false;
    }
    error = validateMateriau(m);
    if (!error.isEmpty()) return false;

    QSqlQuery q(Connection::instance()->database());
    q.prepare("UPDATE MATER SET NOM=:nom, \"TYPE\"=:type, QUANTITE=:quantite, SEUIL=:seuil, FOURNISSEUR=:fournisseur WHERE ID=:id");
    q.bindValue(":id", m.id);
    q.bindValue(":nom", m.nom.trimmed());
    q.bindValue(":type", m.type.trimmed());
    q.bindValue(":quantite", m.quantite);
    q.bindValue(":seuil", m.seuil);
    q.bindValue(":fournisseur", m.fournisseur.trimmed());

    if (!q.exec()) {
        error = q.lastError().text();
        return false;
    }
    return true;
}

bool MateriauxCrud::deleteMateriau(int id, QString &error)
{
    if (!Connection::instance()->createConnect()) {
        error = "Connexion base de donnees echouee";
        return false;
    }
    if (id <= 0) {
        error = "ID invalide";
        return false;
    }

    QSqlQuery q(Connection::instance()->database());
    q.prepare("DELETE FROM MATER WHERE ID = :id");
    q.bindValue(":id", id);

    if (!q.exec()) {
        error = q.lastError().text();
        return false;
    }
    return true;
}

QList<MateriauData> MateriauxCrud::getAll(const QString &orderBy)
{
    QList<MateriauData> out;
    if (!Connection::instance()->createConnect()) return out;
    QString sql = "SELECT ID, NOM, \"TYPE\", QUANTITE, SEUIL, FOURNISSEUR FROM MATER";
    if (orderBy == "nom") sql += " ORDER BY NOM";
    else if (orderBy == "quantite") sql += " ORDER BY QUANTITE DESC";
    else sql += " ORDER BY ID";

    QSqlQuery q(Connection::instance()->database());
    if (!q.exec(sql)) return out;

    while (q.next()) out.append(fromQuery(q));
    return out;
}

QList<MateriauData> MateriauxCrud::searchByNom(const QString &text, const QString &orderBy)
{
    if (!Connection::instance()->createConnect()) return {};
    if (text.trimmed().isEmpty()) return getAll(orderBy);

    QList<MateriauData> out;
    QString sql = "SELECT ID, NOM, \"TYPE\", QUANTITE, SEUIL, FOURNISSEUR FROM MATER WHERE LOWER(NOM) LIKE :s";
    if (orderBy == "nom") sql += " ORDER BY NOM";
    else if (orderBy == "quantite") sql += " ORDER BY QUANTITE DESC";
    else sql += " ORDER BY ID";

    QSqlQuery q(Connection::instance()->database());
    q.prepare(sql);
    q.bindValue(":s", "%" + text.toLower() + "%");
    if (!q.exec()) return out;

    while (q.next()) out.append(fromQuery(q));
    return out;
}

QList<QPair<QString, int>> MateriauxCrud::countByNom()
{
    QList<QPair<QString, int>> out;
    if (!Connection::instance()->createConnect()) return out;
    QSqlQuery q(Connection::instance()->database());
    if (!q.exec("SELECT NOM, COUNT(*) FROM MATER GROUP BY NOM ORDER BY COUNT(*) DESC")) return out;
    while (q.next()) out.append(qMakePair(q.value(0).toString(), q.value(1).toInt()));
    return out;
}

QList<QPair<QString, int>> MateriauxCrud::quantiteByType()
{
    QList<QPair<QString, int>> out;
    if (!Connection::instance()->createConnect()) return out;
    QSqlQuery q(Connection::instance()->database());
    if (!q.exec("SELECT \"TYPE\", SUM(QUANTITE) FROM MATER GROUP BY \"TYPE\" ORDER BY \"TYPE\"")) return out;
    while (q.next()) out.append(qMakePair(q.value(0).toString(), q.value(1).toInt()));
    return out;
}
