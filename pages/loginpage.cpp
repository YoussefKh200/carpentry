#include "loginpage.h"

#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>

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
    connect(ui_->passwordEdit, &QLineEdit::returnPressed, this, &LoginPage::onLoginClicked);
    connect(ui_->identifierEdit, &QLineEdit::returnPressed, this, &LoginPage::onLoginClicked);
    connect(ui_->forgotPasswordBtn, &QPushButton::clicked, this, [this]() {
        QMessageBox::information(this, "Mot de passe oublie", "La reinitialisation par email sera ajoutee bientot.");
    });
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
