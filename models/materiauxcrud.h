#ifndef MATERIAUXCRUD_H
#define MATERIAUXCRUD_H

#include <QList>
#include <QObject>
#include <QString>

struct MateriauData {
    int id = -1;
    QString nom;
    QString type;
    int quantite = 0;
    int seuil = 0;
    QString fournisseur;
};

class MateriauxCrud : public QObject
{
    Q_OBJECT

public:
    explicit MateriauxCrud(QObject *parent = nullptr);

    bool addMateriau(const MateriauData &m, QString &error);
    bool updateMateriau(const MateriauData &m, QString &error);
    bool deleteMateriau(int id, QString &error);

    QList<MateriauData> getAll(const QString &orderBy = QString());
    QList<MateriauData> searchByNom(const QString &text, const QString &orderBy = QString());
    QList<QPair<QString, int>> countByNom();
    QList<QPair<QString, int>> quantiteByType();

    static QString validateMateriau(const MateriauData &m);

private:
    MateriauData fromQuery(class QSqlQuery &q);
};

#endif
