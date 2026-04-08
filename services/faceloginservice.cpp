#include "faceloginservice.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include <QSettings>
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>

FaceLoginService::FaceLoginService(QObject *parent) : QObject(parent)
{
}

FaceLoginResult FaceLoginService::authenticateFromCamera()
{
    QSettings settings;
    const QString repoPath = settings.value("face/repo_path", "C:/Users/youss/Documents/reco/OpenCV-Face-RCGN").toString();
    const int threshold = settings.value("face/threshold", 120).toInt();
    const int timeoutSec = settings.value("face/timeout_sec", 10).toInt();

    FaceLoginResult result;
    QProcess process;
    QString scriptPath = QDir::cleanPath(QCoreApplication::applicationDirPath() + "/../scripts/face_login.py");
    if (!QFileInfo::exists(scriptPath)) {
        scriptPath = QDir::cleanPath(QCoreApplication::applicationDirPath() + "/../../scripts/face_login.py");
    }
    if (!QFileInfo::exists(scriptPath)) {
        scriptPath = QDir::cleanPath(QCoreApplication::applicationDirPath() + "/../../../scripts/face_login.py");
    }
    QStringList args;
    args << scriptPath
         << "--repo" << repoPath
         << "--threshold" << QString::number(threshold)
         << "--timeout" << QString::number(timeoutSec);

    process.start("python", args);
    if (!process.waitForStarted(8000)) {
        result.error = "Impossible de lancer Python pour la reconnaissance faciale.";
        return result;
    }
    if (!process.waitForFinished((timeoutSec + 8) * 1000)) {
        process.kill();
        result.error = "Timeout reconnaissance faciale.";
        return result;
    }

    const QByteArray out = process.readAllStandardOutput().trimmed();
    const QByteArray err = process.readAllStandardError().trimmed();
    if (out.isEmpty()) {
        result.error = err.isEmpty() ? "Aucune reponse du module face." : QString::fromUtf8(err);
        return result;
    }

    QJsonParseError parseErr;
    const QJsonDocument doc = QJsonDocument::fromJson(out, &parseErr);
    if (parseErr.error != QJsonParseError::NoError || !doc.isObject()) {
        result.error = "Reponse invalide du module face: " + QString::fromUtf8(out);
        return result;
    }

    const QJsonObject obj = doc.object();
    result.ok = obj.value("ok").toBool(false);
    result.name = obj.value("name").toString();
    result.confidence = obj.value("confidence").toDouble();
    result.error = obj.value("error").toString();
    return result;
}
