#include "loginpage.h"

#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>

#include "forgotpassworddialog.h"
#include "ui_loginpageform.h"

LoginPage::LoginPage(QWidget *parent)
    : QWidget(parent),
      ui_(new Ui::LoginPageForm)
{
    ui_->setupUi(this);
    ui_->passwordEdit->setEchoMode(QLineEdit::Password);
    ui_->errorLabel->clear();

    QSettings settings;
    const bool remember = settings.value("login/remember_me", false).toBool();
    ui_->rememberMeCheck->setChecked(remember);
    if (remember) {
        ui_->identifierEdit->setText(settings.value("login/remembered_name").toString());
    }

    connect(ui_->loginBtn, &QPushButton::clicked, this, &LoginPage::onLoginClicked);
    connect(ui_->faceLoginBtn, &QPushButton::clicked, this, &LoginPage::onFaceLoginClicked);
    connect(ui_->passwordEdit, &QLineEdit::returnPressed, this, &LoginPage::onLoginClicked);
    connect(ui_->identifierEdit, &QLineEdit::returnPressed, this, &LoginPage::onLoginClicked);
    connect(ui_->forgotPasswordBtn, &QPushButton::clicked, this, [this]() {
        ForgotPasswordDialog dialog(this);
        dialog.exec();
    });
}

void LoginPage::onFaceLoginClicked()
{
    ui_->errorLabel->setStyleSheet("color:#2d6b3d; font-weight:700;");
    ui_->errorLabel->setText("Reconnaissance faciale en cours...");

    const FaceLoginResult face = faceService_.authenticateFromCamera();
    if (!face.ok) {
        ui_->errorLabel->setStyleSheet("color:#a73333; font-weight:700;");
        ui_->errorLabel->setText(face.error.isEmpty() ? "Echec reconnaissance faciale." : face.error);
        return;
    }

    UserData user;
    QString err;
    if (!crud_.authenticateFaceUser(face.name, user, err)) {
        ui_->errorLabel->setStyleSheet("color:#a73333; font-weight:700;");
        ui_->errorLabel->setText(err);
        return;
    }

    ui_->errorLabel->clear();
    emit loginSucceeded(user);
}

LoginPage::~LoginPage()
{
    delete ui_;
}

void LoginPage::onLoginClicked()
{
    UserData user;
    QString err;
    if (!crud_.authenticateUser(ui_->identifierEdit->text(), ui_->passwordEdit->text(), user, err)) {
        ui_->errorLabel->setText(err);
        return;
    }

    QSettings settings;
    settings.setValue("login/remember_me", ui_->rememberMeCheck->isChecked());
    if (ui_->rememberMeCheck->isChecked()) {
        settings.setValue("login/remembered_name", ui_->identifierEdit->text().trimmed());
    } else {
        settings.remove("login/remembered_name");
    }

    ui_->errorLabel->clear();
    ui_->passwordEdit->clear();
    emit loginSucceeded(user);
}
