#ifndef CLIENTCRUD_H
#define CLIENTCRUD_H

#include <QList>
#include <QObject>
#include <QString>

struct ClientData {
    int id = -1;
    QString nom;
    QString prenom;
    QString tel;
    QString mail;
    QString adresse;
    QString dateInscription;
    QString statut;
};

class ClientCrud : public QObject
{
    Q_OBJECT

public:
    explicit ClientCrud(QObject *parent = nullptr);

    bool addClient(const ClientData &client, QString &error);
    bool updateClient(const ClientData &client, QString &error);
    bool deleteClient(int id, QString &error);

    QList<ClientData> getAllClients(const QString &orderBy = QString());
    QList<ClientData> searchClients(const QString &text, const QString &orderBy = QString());
    QList<QPair<QString, int>> clientsByAddress();

    static QString validateClient(const ClientData &client);

private:
    ClientData fromQuery(class QSqlQuery &q);
};

#endif
