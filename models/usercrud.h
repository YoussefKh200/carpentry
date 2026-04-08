#ifndef USERCRUD_H
#define USERCRUD_H

#include <QList>
#include <QObject>
#include <QString>

struct UserData {
    int id = -1;
    QString nom;
    QString prenom;
    QString tel;
    QString mail;
    QString mdp;
    QString salaire;
    QString role;
    QString etat;
};

class UserCrud : public QObject
{
    Q_OBJECT

public:
    explicit UserCrud(QObject *parent = nullptr);

    bool addUser(const UserData &user, QString &error);
    bool updateUser(const UserData &user, QString &error);
    bool deleteUser(int id, QString &error);

    QList<UserData> getAllUsers(const QString &orderBy = QString());
    QList<UserData> searchUsers(const QString &searchText, const QString &orderBy = QString());

    static QString validateUser(const UserData &user);

private:
    int getNextId(QString &error);
    UserData readUserFromQuery(class QSqlQuery &query);
    QString normalizeEtat(const QString &etat);
    QString normalizeRole(const QString &role);
    QString resolvePasswordForUpdate(const UserData &user, QString &error);
};

#endif
