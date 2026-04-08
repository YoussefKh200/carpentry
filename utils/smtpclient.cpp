#include "smtpclient.h"

#include <QByteArray>
#include <QSslSocket>

namespace {

bool readResponse(QSslSocket &socket, const QList<QByteArray> &expectedCodes, QString &error)
{
    if (!socket.waitForReadyRead(10000)) {
        error = "Timeout lecture SMTP";
        return false;
    }
    const QByteArray response = socket.readAll();
    for (const QByteArray &code : expectedCodes) {
        if (response.startsWith(code)) return true;
    }
    error = QString::fromUtf8(response);
    return false;
}

bool sendLine(QSslSocket &socket, const QByteArray &line, QString &error)
{
    if (socket.write(line + "\r\n") == -1) {
        error = "Echec ecriture SMTP";
        return false;
    }
    if (!socket.waitForBytesWritten(10000)) {
        error = "Timeout ecriture SMTP";
        return false;
    }
    return true;
}

}

bool SmtpClient::sendMail(const SmtpConfig &config,
                          const QString &to,
                          const QString &subject,
                          const QString &body,
                          QString &error)
{
    if (config.host.trimmed().isEmpty() || config.username.trimmed().isEmpty() ||
        config.password.trimmed().isEmpty() || config.from.trimmed().isEmpty()) {
        error = "Configuration SMTP incomplete";
        return false;
    }

    QSslSocket socket;
    socket.connectToHostEncrypted(config.host.trimmed(), config.port);
    if (!socket.waitForEncrypted(10000)) {
        error = "Connexion SMTP TLS echouee: " + socket.errorString();
        return false;
    }
    if (!readResponse(socket, {"220"}, error)) return false;

    if (!sendLine(socket, "EHLO smart-carpentry", error)) return false;
    if (!readResponse(socket, {"250"}, error)) return false;

    if (!sendLine(socket, "AUTH LOGIN", error)) return false;
    if (!readResponse(socket, {"334"}, error)) return false;
    if (!sendLine(socket, config.username.toUtf8().toBase64(), error)) return false;
    if (!readResponse(socket, {"334"}, error)) return false;
    if (!sendLine(socket, config.password.toUtf8().toBase64(), error)) return false;
    if (!readResponse(socket, {"235"}, error)) return false;

    if (!sendLine(socket, "MAIL FROM:<" + config.from.toUtf8() + ">", error)) return false;
    if (!readResponse(socket, {"250"}, error)) return false;
    if (!sendLine(socket, "RCPT TO:<" + to.trimmed().toUtf8() + ">", error)) return false;
    if (!readResponse(socket, {"250", "251"}, error)) return false;

    if (!sendLine(socket, "DATA", error)) return false;
    if (!readResponse(socket, {"354"}, error)) return false;

    QByteArray message;
    message += "From: Smart Carpentry <" + config.from.toUtf8() + ">\r\n";
    message += "To: <" + to.trimmed().toUtf8() + ">\r\n";
    message += "Subject: " + subject.toUtf8() + "\r\n";
    message += "Content-Type: text/plain; charset=UTF-8\r\n";
    message += "\r\n";
    message += body.toUtf8() + "\r\n.\r\n";

    if (socket.write(message) == -1 || !socket.waitForBytesWritten(10000)) {
        error = "Echec envoi contenu email";
        return false;
    }
    if (!readResponse(socket, {"250"}, error)) return false;

    sendLine(socket, "QUIT", error);
    socket.disconnectFromHost();
    return true;
}
