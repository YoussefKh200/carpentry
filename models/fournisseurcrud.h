#ifndef FOURNISSEURCRUD_H
#define FOURNISSEURCRUD_H

#include <QDate>
#include <QList>
#include <QObject>
#include <QString>

struct FournisseurData {
    int id = -1;
    QString nom;
    QString prenom;
    QString mail;
    QString adresse;
    QString tel;
    int quantite = 0;
    QString type;
    QDate date;
};

class FournisseurCrud : public QObject
{
    Q_OBJECT

public:
    explicit FournisseurCrud(QObject *parent = nullptr);

    bool addFournisseur(const FournisseurData &f, QString &error);
    bool updateFournisseur(const FournisseurData &f, QString &error);
    bool deleteFournisseur(int id, QString &error);

    QList<FournisseurData> getAllFournisseurs(const QString &orderBy = QString());
    QList<FournisseurData> searchFournisseurs(const QString &typeText, const QString &orderBy = QString());
    QList<QPair<QString, int>> quantiteByType();

    static QString validateFournisseur(const FournisseurData &f);

private:
    FournisseurData fromQuery(class QSqlQuery &q);
};

#endif
