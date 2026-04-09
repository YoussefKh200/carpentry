#ifndef SMTPCONFIG_H
#define SMTPCONFIG_H

#include "smtpclient.h"

inline SmtpConfig defaultSmtpConfig()
{
    SmtpConfig cfg;
    // TODO: Replace these hardcoded values with your SMTP provider credentials
    cfg.host = "smtp.gmail.com";
    cfg.port = 465;
    cfg.username = "youssefkhriji8@gmail.com";
    cfg.password = "qqjm jxqy qzfc hcui";
    cfg.from = "noreply@gmail.com";
    return cfg;
}

#endif // SMTPCONFIG_H
