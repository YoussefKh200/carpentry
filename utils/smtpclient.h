#ifndef SMTPCLIENT_H
#define SMTPCLIENT_H

#include <QString>

struct SmtpConfig {
    QString host;
    int port = 465;
    QString username;
    QString password;
    QString from;
};

class SmtpClient
{
public:
    static bool sendMail(const SmtpConfig &config,
                         const QString &to,
                         const QString &subject,
                         const QString &body,
                         QString &error);
};

#endif
