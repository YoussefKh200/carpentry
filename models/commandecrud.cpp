#include "commandecrud.h"

#include <QSqlError>
#include <QSqlQuery>

#include "../db/connection.h"

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

    QSqlQuery q(Connection::instance()->database());
    q.prepare("INSERT INTO COMMANDE (PRODUIT_COMMANDE, QUANTITE_COMMANDE, PRIX_UNITAIRE_COMMANDE, TOTAL_COMMANDE, DATE_COMMANDE, DATE_LIVRAISON_COMMANDE, MODE_PAIEMENT_COMMANDE) VALUES (:p, :qte, :pu, :tot, :dc, :dl, :mode)");
    q.bindValue(":p", commande.produit.trimmed());
    q.bindValue(":qte", commande.quantite);
    q.bindValue(":pu", commande.prixUnitaire);
    q.bindValue(":tot", commande.total);
    q.bindValue(":dc", commande.dateCommande);
    q.bindValue(":dl", commande.dateLivraison);
    q.bindValue(":mode", commande.modePaiement.trimmed());

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

    QSqlQuery q(Connection::instance()->database());
    q.prepare("UPDATE COMMANDE SET PRODUIT_COMMANDE=:p, QUANTITE_COMMANDE=:qte, PRIX_UNITAIRE_COMMANDE=:pu, TOTAL_COMMANDE=:tot, DATE_COMMANDE=:dc, DATE_LIVRAISON_COMMANDE=:dl, MODE_PAIEMENT_COMMANDE=:mode WHERE ID_COMMANDE=:id");
    q.bindValue(":id", commande.id);
    q.bindValue(":p", commande.produit.trimmed());
    q.bindValue(":qte", commande.quantite);
    q.bindValue(":pu", commande.prixUnitaire);
    q.bindValue(":tot", commande.total);
    q.bindValue(":dc", commande.dateCommande);
    q.bindValue(":dl", commande.dateLivraison);
    q.bindValue(":mode", commande.modePaiement.trimmed());

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

    QSqlQuery q(Connection::instance()->database());
    q.prepare("DELETE FROM COMMANDE WHERE ID_COMMANDE=:id");
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

    QString order = "ID_COMMANDE";
    if (orderBy == "produit") order = "PRODUIT_COMMANDE";
    else if (orderBy == "date") order = "DATE_COMMANDE DESC";
    else if (orderBy == "total") order = "TOTAL_COMMANDE DESC";

    QSqlQuery q(Connection::instance()->database());
    const QString sql =
        "SELECT ID_COMMANDE, PRODUIT_COMMANDE, QUANTITE_COMMANDE, PRIX_UNITAIRE_COMMANDE, TOTAL_COMMANDE, DATE_COMMANDE, DATE_LIVRAISON_COMMANDE, MODE_PAIEMENT_COMMANDE "
        "FROM COMMANDE ORDER BY " + order;
    if (!q.exec(sql)) {
        return out;
    }

    while (q.next()) out.append(fromQuery(q));
    return out;
}

QList<CommandeData> CommandeCrud::searchCommandes(const QString &productText, const QString &orderBy)
{
    if (!Connection::instance()->createConnect()) return {};
    if (productText.trimmed().isEmpty()) return getAllCommandes(orderBy);

    QList<CommandeData> out;
    QString order = "ID_COMMANDE";
    if (orderBy == "produit") order = "PRODUIT_COMMANDE";
    else if (orderBy == "date") order = "DATE_COMMANDE DESC";
    else if (orderBy == "total") order = "TOTAL_COMMANDE DESC";

    QSqlQuery q(Connection::instance()->database());
    q.prepare(
        "SELECT ID_COMMANDE, PRODUIT_COMMANDE, QUANTITE_COMMANDE, PRIX_UNITAIRE_COMMANDE, TOTAL_COMMANDE, DATE_COMMANDE, DATE_LIVRAISON_COMMANDE, MODE_PAIEMENT_COMMANDE "
        "FROM COMMANDE WHERE LOWER(PRODUIT_COMMANDE) LIKE :s ORDER BY " + order
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
    QSqlQuery q(Connection::instance()->database());
    q.prepare("SELECT PRODUIT_COMMANDE, SUM(QUANTITE_COMMANDE) AS QTE FROM COMMANDE GROUP BY PRODUIT_COMMANDE ORDER BY QTE DESC");
    if (!q.exec()) return out;

    int count = 0;
    while (q.next() && count < limit) {
        out.append(qMakePair(q.value(0).toString(), q.value(1).toInt()));
        ++count;
    }
    return out;
}
