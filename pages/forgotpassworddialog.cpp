#include "forgotpassworddialog.h"

#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QRandomGenerator>
#include "../utils/smtpconfig.h"
#include <QVBoxLayout>

#include "../utils/smtpclient.h"

ForgotPasswordDialog::ForgotPasswordDialog(QWidget *parent)
    : QDialog(parent),
      emailEdit_(new QLineEdit(this)),
      otpEdit_(new QLineEdit(this)),
      newPasswordEdit_(new QLineEdit(this)),
      confirmPasswordEdit_(new QLineEdit(this)),
      statusLabel_(new QLabel(this)),
      sendCodeBtn_(new QPushButton("Envoyer code", this)),
      resetBtn_(new QPushButton("Reinitialiser mot de passe", this))
{
    setWindowTitle("Mot de passe oublie");
    setModal(true);
    resize(520, 300);

    emailEdit_->setPlaceholderText("Email du compte");
    otpEdit_->setPlaceholderText("Code OTP recu par email");
    newPasswordEdit_->setPlaceholderText("Nouveau mot de passe (min 8)");
    confirmPasswordEdit_->setPlaceholderText("Confirmer le mot de passe");
    newPasswordEdit_->setEchoMode(QLineEdit::Password);
    confirmPasswordEdit_->setEchoMode(QLineEdit::Password);
    statusLabel_->setStyleSheet("color:#8b2f2f; font-weight:700;");

    auto *form = new QFormLayout();
    form->addRow("Email", emailEdit_);
    form->addRow("OTP", otpEdit_);
    form->addRow("Nouveau mot de passe", newPasswordEdit_);
    form->addRow("Confirmer", confirmPasswordEdit_);

    auto *buttons = new QHBoxLayout();
    buttons->addWidget(sendCodeBtn_);
    buttons->addWidget(resetBtn_);

    auto *root = new QVBoxLayout(this);
    root->addWidget(new QLabel("Saisissez votre email pour recevoir un code OTP."));
    root->addLayout(form);
    root->addWidget(statusLabel_);
    root->addLayout(buttons);

    connect(sendCodeBtn_, &QPushButton::clicked, this, &ForgotPasswordDialog::onSendCode);
    connect(resetBtn_, &QPushButton::clicked, this, &ForgotPasswordDialog::onResetPassword);
}

void ForgotPasswordDialog::onSendCode()
{
    QString err;
    const QString email = emailEdit_->text().trimmed();
    if (!crud_.userExistsByEmail(email, err)) {
        const auto reply = QMessageBox::question(this, "Envoyer quand meme?",
                                                 err + "\n\nEnvoyer le code OTP a cette adresse pour tests?",
                                                 QMessageBox::Yes | QMessageBox::No);
        if (reply != QMessageBox::Yes) {
            statusLabel_->setText(err);
            return;
        }
        // Continue even if no account is associated — useful for SMTP testing with temp emails.
    }

    otp_ = QString::number(QRandomGenerator::global()->bounded(100000, 1000000));
    otpExpiry_ = QDateTime::currentDateTime().addSecs(300);
    currentEmail_ = email;

    SmtpConfig cfg = defaultSmtpConfig();

    const QString body =
        "Bonjour,\n\n"
        "Votre code OTP Smart Carpentry est: " + otp_ + "\n"
        "Ce code expire dans 5 minutes.\n\n"
        "Si vous n'avez pas demande cette operation, ignorez cet email.";

    if (!SmtpClient::sendMail(cfg, email, "Smart Carpentry - Code OTP", body, err)) {
        statusLabel_->setText("Envoi email echoue: " + err);
        return;
    }

    statusLabel_->setStyleSheet("color:#2d6b3d; font-weight:700;");
    statusLabel_->setText("Code OTP envoye. Verifiez votre boite email.");
}

void ForgotPasswordDialog::onResetPassword()
{
    if (currentEmail_.isEmpty()) {
        statusLabel_->setStyleSheet("color:#8b2f2f; font-weight:700;");
        statusLabel_->setText("Commencez par envoyer le code OTP.");
        return;
    }
    if (QDateTime::currentDateTime() > otpExpiry_) {
        statusLabel_->setStyleSheet("color:#8b2f2f; font-weight:700;");
        statusLabel_->setText("Code OTP expire. Renvoyez un nouveau code.");
        return;
    }
    if (otpEdit_->text().trimmed() != otp_) {
        statusLabel_->setStyleSheet("color:#8b2f2f; font-weight:700;");
        statusLabel_->setText("Code OTP incorrect.");
        return;
    }
    if (newPasswordEdit_->text() != confirmPasswordEdit_->text()) {
        statusLabel_->setStyleSheet("color:#8b2f2f; font-weight:700;");
        statusLabel_->setText("Les mots de passe ne correspondent pas.");
        return;
    }

    QString err;
    if (!crud_.updatePasswordByEmail(currentEmail_, newPasswordEdit_->text(), err)) {
        statusLabel_->setStyleSheet("color:#8b2f2f; font-weight:700;");
        statusLabel_->setText(err);
        return;
    }

    QMessageBox::information(this, "Succes", "Mot de passe reinitialise avec succes.");
    accept();
}
