#include "settingspage.h"

#include <QMessageBox>
#include <QPushButton>
#include <QSettings>

#include "ui_settingspageform.h"

SettingsPage::SettingsPage(QWidget *parent)
    : BaseGestionPage("Parametre", "Configuration application", parent),
      ui_(new Ui::SettingsPageForm)
{
    auto *pageRoot = new QWidget(this);
    ui_->setupUi(pageRoot);
    contentLayout_->addWidget(pageRoot);

    ui_->defaultPageCombo->addItems({
        "Gestion Utilisateurs",
        "Gestion Clients",
        "Gestion Commandes",
        "Gestion Fournisseurs",
        "Gestion Materiaux",
        "Parametre"
    });

    connect(ui_->saveBtn, &QPushButton::clicked, this, &SettingsPage::onSaveSettings);
    connect(ui_->clearRememberBtn, &QPushButton::clicked, this, &SettingsPage::onClearRememberedLogin);

    loadSettings();
}

SettingsPage::~SettingsPage()
{
    delete ui_;
}

void SettingsPage::loadSettings()
{
    QSettings settings;
    const QString defaultTitle = "Smart Carpentry Management";
    ui_->windowTitleEdit->setText(settings.value("app/window_title", defaultTitle).toString());
    ui_->defaultPageCombo->setCurrentIndex(settings.value("app/default_page", 0).toInt());
    ui_->tipsCheck->setChecked(settings.value("app/show_tips", true).toBool());
    ui_->confirmLogoutCheck->setChecked(settings.value("app/confirm_logout", true).toBool());
    ui_->smtpHostEdit->setText(settings.value("smtp/host").toString());
    ui_->smtpPortSpin->setValue(settings.value("smtp/port", 465).toInt());
    ui_->smtpUsernameEdit->setText(settings.value("smtp/username").toString());
    ui_->smtpPasswordEdit->setText(settings.value("smtp/password").toString());
    ui_->smtpFromEdit->setText(settings.value("smtp/from").toString());
    ui_->faceRepoPathEdit->setText(settings.value("face/repo_path", "C:/Users/youss/Documents/reco/OpenCV-Face-RCGN").toString());
    ui_->faceThresholdSpin->setValue(settings.value("face/threshold", 120).toInt());
    ui_->faceTimeoutSpin->setValue(settings.value("face/timeout_sec", 10).toInt());
}

void SettingsPage::onSaveSettings()
{
    QSettings settings;
    const QString title = ui_->windowTitleEdit->text().trimmed().isEmpty()
                              ? QString("Smart Carpentry Management")
                              : ui_->windowTitleEdit->text().trimmed();

    settings.setValue("app/window_title", title);
    settings.setValue("app/default_page", ui_->defaultPageCombo->currentIndex());
    settings.setValue("app/show_tips", ui_->tipsCheck->isChecked());
    settings.setValue("app/confirm_logout", ui_->confirmLogoutCheck->isChecked());
    settings.setValue("smtp/host", ui_->smtpHostEdit->text().trimmed());
    settings.setValue("smtp/port", ui_->smtpPortSpin->value());
    settings.setValue("smtp/username", ui_->smtpUsernameEdit->text().trimmed());
    settings.setValue("smtp/password", ui_->smtpPasswordEdit->text());
    settings.setValue("smtp/from", ui_->smtpFromEdit->text().trimmed());
    settings.setValue("face/repo_path", ui_->faceRepoPathEdit->text().trimmed());
    settings.setValue("face/threshold", ui_->faceThresholdSpin->value());
    settings.setValue("face/timeout_sec", ui_->faceTimeoutSpin->value());

    emit windowTitleChanged(title);
    QMessageBox::information(this, "Parametre", "Parametres enregistres avec succes.");
}

void SettingsPage::onClearRememberedLogin()
{
    QSettings settings;
    settings.remove("login/remembered_name");
    settings.setValue("login/remember_me", false);
    QMessageBox::information(this, "Parametre", "Identifiant memorise supprime.");
}
