/********************************************************************************
** Form generated from reading UI file 'settingspageform.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGSPAGEFORM_H
#define UI_SETTINGSPAGEFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SettingsPageForm
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *introLabel;
    QGroupBox *generalGroup;
    QFormLayout *formLayout;
    QLabel *windowTitleLabel;
    QLineEdit *windowTitleEdit;
    QLabel *defaultPageLabel;
    QComboBox *defaultPageCombo;
    QGroupBox *experienceGroup;
    QVBoxLayout *verticalLayout_3;
    QCheckBox *tipsCheck;
    QCheckBox *confirmLogoutCheck;
    QGroupBox *securityGroup;
    QVBoxLayout *verticalLayout_2;
    QPushButton *clearRememberBtn;
    QHBoxLayout *actionsLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *saveBtn;

    void setupUi(QWidget *SettingsPageForm)
    {
        if (SettingsPageForm->objectName().isEmpty())
            SettingsPageForm->setObjectName("SettingsPageForm");
        SettingsPageForm->resize(860, 520);
        verticalLayout = new QVBoxLayout(SettingsPageForm);
        verticalLayout->setSpacing(14);
        verticalLayout->setObjectName("verticalLayout");
        introLabel = new QLabel(SettingsPageForm);
        introLabel->setObjectName("introLabel");
        introLabel->setStyleSheet(QString::fromUtf8("color:#000000; font-weight:700;"));
        introLabel->setWordWrap(true);

        verticalLayout->addWidget(introLabel);

        generalGroup = new QGroupBox(SettingsPageForm);
        generalGroup->setObjectName("generalGroup");
        formLayout = new QFormLayout(generalGroup);
        formLayout->setObjectName("formLayout");
        windowTitleLabel = new QLabel(generalGroup);
        windowTitleLabel->setObjectName("windowTitleLabel");

        formLayout->setWidget(0, QFormLayout::LabelRole, windowTitleLabel);

        windowTitleEdit = new QLineEdit(generalGroup);
        windowTitleEdit->setObjectName("windowTitleEdit");

        formLayout->setWidget(0, QFormLayout::FieldRole, windowTitleEdit);

        defaultPageLabel = new QLabel(generalGroup);
        defaultPageLabel->setObjectName("defaultPageLabel");

        formLayout->setWidget(1, QFormLayout::LabelRole, defaultPageLabel);

        defaultPageCombo = new QComboBox(generalGroup);
        defaultPageCombo->setObjectName("defaultPageCombo");

        formLayout->setWidget(1, QFormLayout::FieldRole, defaultPageCombo);


        verticalLayout->addWidget(generalGroup);

        experienceGroup = new QGroupBox(SettingsPageForm);
        experienceGroup->setObjectName("experienceGroup");
        verticalLayout_3 = new QVBoxLayout(experienceGroup);
        verticalLayout_3->setObjectName("verticalLayout_3");
        tipsCheck = new QCheckBox(experienceGroup);
        tipsCheck->setObjectName("tipsCheck");
        tipsCheck->setStyleSheet(QString::fromUtf8("color:#000000;"));

        verticalLayout_3->addWidget(tipsCheck);

        confirmLogoutCheck = new QCheckBox(experienceGroup);
        confirmLogoutCheck->setObjectName("confirmLogoutCheck");
        confirmLogoutCheck->setStyleSheet(QString::fromUtf8("color:#000000;"));

        verticalLayout_3->addWidget(confirmLogoutCheck);


        verticalLayout->addWidget(experienceGroup);

        securityGroup = new QGroupBox(SettingsPageForm);
        securityGroup->setObjectName("securityGroup");
        verticalLayout_2 = new QVBoxLayout(securityGroup);
        verticalLayout_2->setObjectName("verticalLayout_2");
        clearRememberBtn = new QPushButton(securityGroup);
        clearRememberBtn->setObjectName("clearRememberBtn");
        clearRememberBtn->setStyleSheet(QString::fromUtf8("background:#f3f8ff; color:#1f3f66; border:1px solid #b8cde8; border-radius:10px; font-weight:700;"));

        verticalLayout_2->addWidget(clearRememberBtn);


        verticalLayout->addWidget(securityGroup);

        actionsLayout = new QHBoxLayout();
        actionsLayout->setObjectName("actionsLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        actionsLayout->addItem(horizontalSpacer);

        saveBtn = new QPushButton(SettingsPageForm);
        saveBtn->setObjectName("saveBtn");
        saveBtn->setStyleSheet(QString::fromUtf8("background:#a85a31; color:#ffffff; border:1px solid #8f4a26; border-radius:11px; font-weight:800;"));

        actionsLayout->addWidget(saveBtn);


        verticalLayout->addLayout(actionsLayout);


        retranslateUi(SettingsPageForm);

        QMetaObject::connectSlotsByName(SettingsPageForm);
    } // setupUi

    void retranslateUi(QWidget *SettingsPageForm)
    {
        introLabel->setText(QCoreApplication::translate("SettingsPageForm", "Personnalisez votre espace Smart Carpentry: titre de la fenetre, page de demarrage et options de connexion.", nullptr));
        generalGroup->setTitle(QCoreApplication::translate("SettingsPageForm", "General", nullptr));
        windowTitleLabel->setText(QCoreApplication::translate("SettingsPageForm", "Titre fenetre", nullptr));
        defaultPageLabel->setText(QCoreApplication::translate("SettingsPageForm", "Page par defaut apres login", nullptr));
        experienceGroup->setTitle(QCoreApplication::translate("SettingsPageForm", "Experience utilisateur", nullptr));
        tipsCheck->setText(QCoreApplication::translate("SettingsPageForm", "Afficher des conseils rapides sur les pages", nullptr));
        confirmLogoutCheck->setText(QCoreApplication::translate("SettingsPageForm", "Demander confirmation avant deconnexion", nullptr));
        securityGroup->setTitle(QCoreApplication::translate("SettingsPageForm", "Connexion", nullptr));
        clearRememberBtn->setText(QCoreApplication::translate("SettingsPageForm", "Effacer l'identifiant memorise (Remember me)", nullptr));
        saveBtn->setText(QCoreApplication::translate("SettingsPageForm", "Enregistrer", nullptr));
        (void)SettingsPageForm;
    } // retranslateUi

};

namespace Ui {
    class SettingsPageForm: public Ui_SettingsPageForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGSPAGEFORM_H
