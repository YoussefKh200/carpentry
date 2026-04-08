#ifndef COMMANDECRUD_H
#define COMMANDECRUD_H

#include <QDate>
#include <QList>
#include <QObject>
#include <QString>

struct CommandeData {
    int id = -1;
    QString produit;
    int quantite = 0;
    double prixUnitaire = 0.0;
    double total = 0.0;
    QDate dateCommande;
    QDate dateLivraison;
    QString modePaiement;
};

class CommandeCrud : public QObject
{
    Q_OBJECT

public:
    explicit CommandeCrud(QObject *parent = nullptr);

    bool addCommande(const CommandeData &commande, QString &error);
    bool updateCommande(const CommandeData &commande, QString &error);
    bool deleteCommande(int id, QString &error);

    QList<CommandeData> getAllCommandes(const QString &orderBy = QString());
    QList<CommandeData> searchCommandes(const QString &productText, const QString &orderBy = QString());
    QList<QPair<QString, int>> topProduits(int limit = 5);

    static QString validateCommande(const CommandeData &commande);

private:
    CommandeData fromQuery(class QSqlQuery &q);
};

#endif
