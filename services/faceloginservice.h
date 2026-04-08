#ifndef FACELOGINSERVICE_H
#define FACELOGINSERVICE_H

#include <QObject>
#include <QString>

struct FaceLoginResult {
    bool ok = false;
    QString name;
    double confidence = 0.0;
    QString error;
};

class FaceLoginService : public QObject
{
    Q_OBJECT
public:
    explicit FaceLoginService(QObject *parent = nullptr);
    FaceLoginResult authenticateFromCamera();
};

#endif
