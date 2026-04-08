#ifndef FORGOTPASSWORDDIALOG_H
#define FORGOTPASSWORDDIALOG_H

#include <QDateTime>
#include <QDialog>

#include "../models/usercrud.h"

class QLineEdit;
class QLabel;
class QPushButton;

class ForgotPasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ForgotPasswordDialog(QWidget *parent = nullptr);

private slots:
    void onSendCode();
    void onResetPassword();

private:
    UserCrud crud_;
    QString currentEmail_;
    QString otp_;
    QDateTime otpExpiry_;

    QLineEdit *emailEdit_;
    QLineEdit *otpEdit_;
    QLineEdit *newPasswordEdit_;
    QLineEdit *confirmPasswordEdit_;
    QLabel *statusLabel_;
    QPushButton *sendCodeBtn_;
    QPushButton *resetBtn_;
};

#endif
