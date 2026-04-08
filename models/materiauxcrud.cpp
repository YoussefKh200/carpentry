#include "materiauxcrud.h"

#include <QSqlError>
#include <QSqlQuery>

#include "../db/connection.h"

namespace {

struct MateriauxSchema {
    QString table;
    QString id;
    QString nom;
    QString type;
    QString quantite;
    QString seuil;
    QString fournisseur;
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

MateriauxSchema resolveSchema(const QSqlDatabase &db)
{
    MateriauxSchema s;
    s.table = "MATERIAUX";

    QStringList cols;
    QSqlQuery cq(db);
    cq.prepare("SELECT COLUMN_NAME FROM USER_TAB_COLUMNS WHERE TABLE_NAME=:t ORDER BY COLUMN_ID");
    cq.bindValue(":t", s.table);
    if (cq.exec()) {
        while (cq.next()) cols << cq.value(0).toString();
    }

    s.id = pickColumn(cols, {"ID"}, "ID");
    s.nom = pickColumn(cols, {"NOM"});
    s.type = pickColumn(cols, {"TYPE"});
    s.quantite = pickColumn(cols, {"QUANTIT"});
    s.seuil = pickColumn(cols, {"SEUIL"});
    s.fournisseur = pickColumn(cols, {"FOURNISSEUR"});

    if (s.id.isEmpty()) s.id = "ID";
    if (s.nom.isEmpty()) s.nom = "NOM";
    if (s.type.isEmpty()) s.type = "TYPE";
    if (s.quantite.isEmpty()) s.quantite = "QUANTITE";
    if (s.seuil.isEmpty()) s.seuil = "SEUIL";
    if (s.fournisseur.isEmpty()) s.fournisseur = "FOURNISSEUR";
    return s;
}

}

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

    const QSqlDatabase db = Connection::instance()->database();
    const MateriauxSchema s = resolveSchema(db);
    QSqlQuery q(db);
    q.prepare("INSERT INTO " + s.table + " (" + s.id + ", " + s.nom + ", " + s.type + ", " + s.quantite + ", " + s.seuil + ", " + s.fournisseur + ") VALUES (?, ?, ?, ?, ?, ?)");
    q.addBindValue(m.id);
    q.addBindValue(m.nom.trimmed());
    q.addBindValue(m.type.trimmed());
    q.addBindValue(m.quantite);
    q.addBindValue(m.seuil);
    q.addBindValue(m.fournisseur.trimmed());

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

    const QSqlDatabase db = Connection::instance()->database();
    const MateriauxSchema s = resolveSchema(db);
    QSqlQuery q(db);
    q.prepare("UPDATE " + s.table + " SET " + s.nom + "=?, " + s.type + "=?, " + s.quantite + "=?, " + s.seuil + "=?, " + s.fournisseur + "=? WHERE " + s.id + "=?");
    q.addBindValue(m.nom.trimmed());
    q.addBindValue(m.type.trimmed());
    q.addBindValue(m.quantite);
    q.addBindValue(m.seuil);
    q.addBindValue(m.fournisseur.trimmed());
    q.addBindValue(m.id);

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

    const QSqlDatabase db = Connection::instance()->database();
    const MateriauxSchema s = resolveSchema(db);
    QSqlQuery q(db);
    q.prepare("DELETE FROM " + s.table + " WHERE " + s.id + " = :id");
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
    const QSqlDatabase db = Connection::instance()->database();
    const MateriauxSchema s = resolveSchema(db);

    QString sql = "SELECT " + s.id + ", " + s.nom + ", " + s.type + ", " + s.quantite + ", " + s.seuil + ", " + s.fournisseur + " FROM " + s.table;
    if (orderBy == "nom") sql += " ORDER BY " + s.nom;
    else if (orderBy == "quantite") sql += " ORDER BY TO_NUMBER(" + s.quantite + ") DESC";
    else sql += " ORDER BY " + s.id;

    QSqlQuery q(db);
    if (!q.exec(sql)) return out;
    while (q.next()) out.append(fromQuery(q));
    return out;
}

QList<MateriauData> MateriauxCrud::searchByNom(const QString &text, const QString &orderBy)
{
    if (!Connection::instance()->createConnect()) return {};
    if (text.trimmed().isEmpty()) return getAll(orderBy);
    const QSqlDatabase db = Connection::instance()->database();
    const MateriauxSchema s = resolveSchema(db);

    QList<MateriauData> out;
    QString sql = "SELECT " + s.id + ", " + s.nom + ", " + s.type + ", " + s.quantite + ", " + s.seuil + ", " + s.fournisseur + " FROM " + s.table + " WHERE LOWER(" + s.nom + ") LIKE :s";
    if (orderBy == "nom") sql += " ORDER BY " + s.nom;
    else if (orderBy == "quantite") sql += " ORDER BY TO_NUMBER(" + s.quantite + ") DESC";
    else sql += " ORDER BY " + s.id;

    QSqlQuery q(db);
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
    const QSqlDatabase db = Connection::instance()->database();
    const MateriauxSchema s = resolveSchema(db);
    QSqlQuery q(db);
    if (!q.exec("SELECT " + s.nom + ", COUNT(*) FROM " + s.table + " GROUP BY " + s.nom + " ORDER BY COUNT(*) DESC")) return out;
    while (q.next()) out.append(qMakePair(q.value(0).toString(), q.value(1).toInt()));
    return out;
}

QList<QPair<QString, int>> MateriauxCrud::quantiteByType()
{
    QList<QPair<QString, int>> out;
    if (!Connection::instance()->createConnect()) return out;
    const QSqlDatabase db = Connection::instance()->database();
    const MateriauxSchema s = resolveSchema(db);
    QSqlQuery q(db);
    if (!q.exec("SELECT " + s.type + ", SUM(TO_NUMBER(" + s.quantite + ")) FROM " + s.table + " GROUP BY " + s.type + " ORDER BY " + s.type)) return out;
    while (q.next()) out.append(qMakePair(q.value(0).toString(), q.value(1).toInt()));
    return out;
}
