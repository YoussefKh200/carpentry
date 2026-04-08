#include "commandecrud.h"

#include <QSqlError>
#include <QSqlQuery>

#include "../db/connection.h"

namespace {

struct CommandeSchema {
    QString table;
    QString id;
    QString produit;
    QString quantite;
    QString prixU;
    QString total;
    QString dateCmd;
    QString dateLiv;
    QString mode;
    QString statut;
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

CommandeSchema resolveSchema(const QSqlDatabase &db)
{
    CommandeSchema s;
    s.table = "COMMANDE";

    QStringList cols;
    QSqlQuery cq(db);
    cq.prepare("SELECT COLUMN_NAME FROM USER_TAB_COLUMNS WHERE TABLE_NAME=:t ORDER BY COLUMN_ID");
    cq.bindValue(":t", s.table);
    if (cq.exec()) {
        while (cq.next()) cols << cq.value(0).toString();
    }

    s.id = pickColumn(cols, {"ID"}, "ID");
    s.produit = pickColumn(cols, {"PRODUIT"});
    s.quantite = pickColumn(cols, {"QUANTIT"});
    s.prixU = pickColumn(cols, {"PRIX", "UNIT"});
    s.total = pickColumn(cols, {"TOTAL"});
    s.dateCmd = pickColumn(cols, {"DATE", "COMMANDE"});
    s.dateLiv = pickColumn(cols, {"DATE", "LIVRAISON"});
    s.mode = pickColumn(cols, {"MODE", "PAIEMENT"});
    s.statut = pickColumn(cols, {"STATUT"});

    if (s.id.isEmpty()) s.id = "ID_COMMANDE";
    if (s.produit.isEmpty()) s.produit = "PRODUIT_COMMANDE";
    if (s.quantite.isEmpty()) s.quantite = "QUANTITE_COMMANDE";
    if (s.prixU.isEmpty()) s.prixU = "PRIX_UNITAIRE_COMMANDE";
    if (s.total.isEmpty()) s.total = "TOTAL_COMMANDE";
    if (s.dateCmd.isEmpty()) s.dateCmd = "DATE_COMMANDE";
    if (s.dateLiv.isEmpty()) s.dateLiv = "DATE_LIVRAISON_COMMANDE";
    if (s.mode.isEmpty()) s.mode = "MODE_PAIEMENT_COMMANDE";
    if (s.statut.isEmpty()) s.statut = "STATUT_COMMANDE";
    return s;
}

int nextId(const QSqlDatabase &db, const CommandeSchema &s)
{
    QSqlQuery q(db);
    if (!q.exec("SELECT NVL(MAX(" + s.id + "), 0) + 1 FROM " + s.table)) return 1;
    if (!q.next()) return 1;
    return q.value(0).toInt();
}

}

CommandeCrud::CommandeCrud(QObject *parent) : QObject(parent)
{
}

QString CommandeCrud::validateCommande(const CommandeData &c)
{
    if (c.produit.trimmed().isEmpty()) return "Produit obligatoire";
    if (c.quantite <= 0) return "Quantite invalide";
    if (c.prixUnitaire <= 0.0) return "Prix unitaire invalide";
    if (!c.dateCommande.isValid() || !c.dateLivraison.isValid()) return "Dates invalides";
    if (c.dateLivraison < c.dateCommande.addDays(2)) return "La livraison doit etre >= 2 jours apres la commande";
    if (c.modePaiement.trimmed().isEmpty()) return "Mode paiement obligatoire";
    return QString();
}

CommandeData CommandeCrud::fromQuery(QSqlQuery &q)
{
    CommandeData c;
    c.id = q.value(0).toInt();
    c.produit = q.value(1).toString();
    c.quantite = q.value(2).toInt();
    c.prixUnitaire = q.value(3).toDouble();
    c.total = q.value(4).toDouble();
    c.dateCommande = q.value(5).toDate();
    c.dateLivraison = q.value(6).toDate();
    c.modePaiement = q.value(7).toString();
    return c;
}

bool CommandeCrud::addCommande(const CommandeData &commande, QString &error)
{
    if (!Connection::instance()->createConnect()) {
        error = "Connexion base de donnees echouee";
        return false;
    }
    error = validateCommande(commande);
    if (!error.isEmpty()) return false;

    const QSqlDatabase db = Connection::instance()->database();
    const CommandeSchema s = resolveSchema(db);
    const int id = nextId(db, s);

    QSqlQuery q(db);
    q.prepare(
        "INSERT INTO " + s.table + " (" + s.id + ", " + s.produit + ", " + s.quantite + ", " + s.prixU + ", " + s.total + ", " + s.dateCmd + ", " + s.dateLiv + ", " + s.mode + ", " + s.statut + ") "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)"
    );
    q.addBindValue(id);
    q.addBindValue(commande.produit.trimmed());
    q.addBindValue(commande.quantite);
    q.addBindValue(commande.prixUnitaire);
    q.addBindValue(commande.total);
    q.addBindValue(commande.dateCommande);
    q.addBindValue(commande.dateLivraison);
    q.addBindValue(commande.modePaiement.trimmed());
    q.addBindValue("En attente");

    if (!q.exec()) {
        error = q.lastError().text();
        return false;
    }
    return true;
}

bool CommandeCrud::updateCommande(const CommandeData &commande, QString &error)
{
    if (!Connection::instance()->createConnect()) {
        error = "Connexion base de donnees echouee";
        return false;
    }
    if (commande.id <= 0) {
        error = "ID commande invalide";
        return false;
    }

    error = validateCommande(commande);
    if (!error.isEmpty()) return false;

    const QSqlDatabase db = Connection::instance()->database();
    const CommandeSchema s = resolveSchema(db);
    QSqlQuery q(db);
    q.prepare(
        "UPDATE " + s.table + " SET " + s.produit + "=?, " + s.quantite + "=?, " + s.prixU + "=?, " + s.total + "=?, " + s.dateCmd + "=?, " + s.dateLiv + "=?, " + s.mode + "=? "
        "WHERE " + s.id + "=?"
    );
    q.addBindValue(commande.produit.trimmed());
    q.addBindValue(commande.quantite);
    q.addBindValue(commande.prixUnitaire);
    q.addBindValue(commande.total);
    q.addBindValue(commande.dateCommande);
    q.addBindValue(commande.dateLivraison);
    q.addBindValue(commande.modePaiement.trimmed());
    q.addBindValue(commande.id);

    if (!q.exec()) {
        error = q.lastError().text();
        return false;
    }
    return true;
}

bool CommandeCrud::deleteCommande(int id, QString &error)
{
    if (!Connection::instance()->createConnect()) {
        error = "Connexion base de donnees echouee";
        return false;
    }
    if (id <= 0) {
        error = "ID commande invalide";
        return false;
    }

    const QSqlDatabase db = Connection::instance()->database();
    const CommandeSchema s = resolveSchema(db);
    QSqlQuery q(db);
    q.prepare("DELETE FROM " + s.table + " WHERE " + s.id + "=:id");
    q.bindValue(":id", id);
    if (!q.exec()) {
        error = q.lastError().text();
        return false;
    }
    return true;
}

QList<CommandeData> CommandeCrud::getAllCommandes(const QString &orderBy)
{
    QList<CommandeData> out;
    if (!Connection::instance()->createConnect()) return out;
    const QSqlDatabase db = Connection::instance()->database();
    const CommandeSchema s = resolveSchema(db);

    QString order = s.id;
    if (orderBy == "produit") order = s.produit;
    else if (orderBy == "date") order = s.dateCmd + " DESC";
    else if (orderBy == "total") order = s.total + " DESC";

    QSqlQuery q(db);
    const QString sql =
        "SELECT " + s.id + ", " + s.produit + ", " + s.quantite + ", " + s.prixU + ", " + s.total + ", " + s.dateCmd + ", " + s.dateLiv + ", " + s.mode + " "
        "FROM " + s.table + " ORDER BY " + order;
    if (!q.exec(sql)) return out;
    while (q.next()) out.append(fromQuery(q));
    return out;
}

QList<CommandeData> CommandeCrud::searchCommandes(const QString &productText, const QString &orderBy)
{
    if (!Connection::instance()->createConnect()) return {};
    if (productText.trimmed().isEmpty()) return getAllCommandes(orderBy);

    QList<CommandeData> out;
    const QSqlDatabase db = Connection::instance()->database();
    const CommandeSchema s = resolveSchema(db);

    QString order = s.id;
    if (orderBy == "produit") order = s.produit;
    else if (orderBy == "date") order = s.dateCmd + " DESC";
    else if (orderBy == "total") order = s.total + " DESC";

    QSqlQuery q(db);
    q.prepare(
        "SELECT " + s.id + ", " + s.produit + ", " + s.quantite + ", " + s.prixU + ", " + s.total + ", " + s.dateCmd + ", " + s.dateLiv + ", " + s.mode + " "
        "FROM " + s.table + " WHERE LOWER(" + s.produit + ") LIKE :s ORDER BY " + order
    );
    q.bindValue(":s", "%" + productText.toLower() + "%");
    if (!q.exec()) return out;
    while (q.next()) out.append(fromQuery(q));
    return out;
}

QList<QPair<QString, int>> CommandeCrud::topProduits(int limit)
{
    QList<QPair<QString, int>> out;
    if (!Connection::instance()->createConnect()) return out;
    const QSqlDatabase db = Connection::instance()->database();
    const CommandeSchema s = resolveSchema(db);

    QSqlQuery q(db);
    q.prepare(
        "SELECT " + s.produit + ", SUM(" + s.quantite + ") AS QTE FROM " + s.table + " GROUP BY " + s.produit + " ORDER BY QTE DESC"
    );
    if (!q.exec()) return out;

    int count = 0;
    while (q.next() && count < limit) {
        out.append(qMakePair(q.value(0).toString(), q.value(1).toInt()));
        ++count;
    }
    return out;
}
