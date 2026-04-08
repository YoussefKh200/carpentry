/********************************************************************************
** Form generated from reading UI file 'loginpageform.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINPAGEFORM_H
#define UI_LOGINPAGEFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginPageForm
{
public:
    QHBoxLayout *rootLayout;
    QSpacerItem *leftSpacer;
    QFrame *PageCard;
    QVBoxLayout *cardLayout;
    QLabel *PageTitle;
    QLabel *subtitleLabel;
    QLineEdit *identifierEdit;
    QLineEdit *passwordEdit;
    QLabel *errorLabel;
    QHBoxLayout *optionsLayout;
    QCheckBox *rememberMeCheck;
    QSpacerItem *horizontalSpacer;
    QPushButton *forgotPasswordBtn;
    QPushButton *loginBtn;
    QFrame *infoCard;
    QVBoxLayout *infoLayout;
    QLabel *welcomeTitle;
    QLabel *welcomeText;
    QLabel *bullet1;
    QLabel *bullet2;
    QLabel *bullet3;
    QSpacerItem *infoBottomSpacer;
    QSpacerItem *rightSpacer;

    void setupUi(QWidget *LoginPageForm)
    {
        if (LoginPageForm->objectName().isEmpty())
            LoginPageForm->setObjectName("LoginPageForm");
        LoginPageForm->resize(1100, 760);
        rootLayout = new QHBoxLayout(LoginPageForm);
        rootLayout->setSpacing(22);
        rootLayout->setObjectName("rootLayout");
        rootLayout->setContentsMargins(36, 36, 36, 36);
        leftSpacer = new QSpacerItem(80, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        rootLayout->addItem(leftSpacer);

        PageCard = new QFrame(LoginPageForm);
        PageCard->setObjectName("PageCard");
        PageCard->setMinimumSize(QSize(520, 0));
        PageCard->setMaximumSize(QSize(520, 16777215));
        PageCard->setFrameShape(QFrame::StyledPanel);
        cardLayout = new QVBoxLayout(PageCard);
        cardLayout->setSpacing(12);
        cardLayout->setObjectName("cardLayout");
        cardLayout->setContentsMargins(32, 28, 32, 28);
        PageTitle = new QLabel(PageCard);
        PageTitle->setObjectName("PageTitle");
        PageTitle->setAlignment(Qt::AlignCenter);

        cardLayout->addWidget(PageTitle);

        subtitleLabel = new QLabel(PageCard);
        subtitleLabel->setObjectName("subtitleLabel");
        subtitleLabel->setStyleSheet(QString::fromUtf8("color:#000000;"));
        subtitleLabel->setAlignment(Qt::AlignCenter);

        cardLayout->addWidget(subtitleLabel);

        identifierEdit = new QLineEdit(PageCard);
        identifierEdit->setObjectName("identifierEdit");

        cardLayout->addWidget(identifierEdit);

        passwordEdit = new QLineEdit(PageCard);
        passwordEdit->setObjectName("passwordEdit");

        cardLayout->addWidget(passwordEdit);

        errorLabel = new QLabel(PageCard);
        errorLabel->setObjectName("errorLabel");
        errorLabel->setStyleSheet(QString::fromUtf8("color:#a73333; font-weight:700;"));

        cardLayout->addWidget(errorLabel);

        optionsLayout = new QHBoxLayout();
        optionsLayout->setObjectName("optionsLayout");
        rememberMeCheck = new QCheckBox(PageCard);
        rememberMeCheck->setObjectName("rememberMeCheck");
        rememberMeCheck->setStyleSheet(QString::fromUtf8("color:#000000; font-weight:600;"));

        optionsLayout->addWidget(rememberMeCheck);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        optionsLayout->addItem(horizontalSpacer);

        forgotPasswordBtn = new QPushButton(PageCard);
        forgotPasswordBtn->setObjectName("forgotPasswordBtn");
        forgotPasswordBtn->setStyleSheet(QString::fromUtf8("color:#000000; font-weight:700;"));
        forgotPasswordBtn->setFlat(true);

        optionsLayout->addWidget(forgotPasswordBtn);


        cardLayout->addLayout(optionsLayout);

        loginBtn = new QPushButton(PageCard);
        loginBtn->setObjectName("loginBtn");

        cardLayout->addWidget(loginBtn);


        rootLayout->addWidget(PageCard);

        infoCard = new QFrame(LoginPageForm);
        infoCard->setObjectName("infoCard");
        infoCard->setStyleSheet(QString::fromUtf8("QFrame#infoCard { background:#f2eee7; border:1px solid #ddd0bf; border-radius:14px; }"));
        infoCard->setMinimumSize(QSize(360, 0));
        infoCard->setMaximumSize(QSize(420, 16777215));
        infoCard->setFrameShape(QFrame::StyledPanel);
        infoLayout = new QVBoxLayout(infoCard);
        infoLayout->setSpacing(10);
        infoLayout->setObjectName("infoLayout");
        infoLayout->setContentsMargins(28, 28, 28, 28);
        welcomeTitle = new QLabel(infoCard);
        welcomeTitle->setObjectName("welcomeTitle");
        welcomeTitle->setStyleSheet(QString::fromUtf8("color:#000000; font-size:22px; font-weight:800;"));

        infoLayout->addWidget(welcomeTitle);

        welcomeText = new QLabel(infoCard);
        welcomeText->setObjectName("welcomeText");
        welcomeText->setStyleSheet(QString::fromUtf8("color:#000000; font-weight:600;"));
        welcomeText->setWordWrap(true);

        infoLayout->addWidget(welcomeText);

        bullet1 = new QLabel(infoCard);
        bullet1->setObjectName("bullet1");
        bullet1->setStyleSheet(QString::fromUtf8("color:#000000;"));

        infoLayout->addWidget(bullet1);

        bullet2 = new QLabel(infoCard);
        bullet2->setObjectName("bullet2");
        bullet2->setStyleSheet(QString::fromUtf8("color:#000000;"));

        infoLayout->addWidget(bullet2);

        bullet3 = new QLabel(infoCard);
        bullet3->setObjectName("bullet3");
        bullet3->setStyleSheet(QString::fromUtf8("color:#000000;"));

        infoLayout->addWidget(bullet3);

        infoBottomSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        infoLayout->addItem(infoBottomSpacer);


        rootLayout->addWidget(infoCard);

        rightSpacer = new QSpacerItem(80, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        rootLayout->addItem(rightSpacer);


        retranslateUi(LoginPageForm);

        QMetaObject::connectSlotsByName(LoginPageForm);
    } // setupUi

    void retranslateUi(QWidget *LoginPageForm)
    {
        PageTitle->setText(QCoreApplication::translate("LoginPageForm", "Connexion", nullptr));
        subtitleLabel->setText(QCoreApplication::translate("LoginPageForm", "Accedez au dashboard Smart Carpentry", nullptr));
        identifierEdit->setPlaceholderText(QCoreApplication::translate("LoginPageForm", "Nom d'utilisateur", nullptr));
        passwordEdit->setPlaceholderText(QCoreApplication::translate("LoginPageForm", "Mot de passe", nullptr));
        errorLabel->setText(QString());
        rememberMeCheck->setText(QCoreApplication::translate("LoginPageForm", "Se souvenir de moi", nullptr));
        forgotPasswordBtn->setText(QCoreApplication::translate("LoginPageForm", "Mot de passe oublie ?", nullptr));
        loginBtn->setText(QCoreApplication::translate("LoginPageForm", "Se connecter", nullptr));
        welcomeTitle->setText(QCoreApplication::translate("LoginPageForm", "Smart Carpentry", nullptr));
        welcomeText->setText(QCoreApplication::translate("LoginPageForm", "Plateforme centralisee pour gerer utilisateurs, clients, commandes, fournisseurs et materiaux.", nullptr));
        bullet1->setText(QCoreApplication::translate("LoginPageForm", "- Tableau de bord multi-gestions", nullptr));
        bullet2->setText(QCoreApplication::translate("LoginPageForm", "- Visualisations et exports PDF", nullptr));
        bullet3->setText(QCoreApplication::translate("LoginPageForm", "- Navigation fluide via sidebar unique", nullptr));
        (void)LoginPageForm;
    } // retranslateUi

};

namespace Ui {
    class LoginPageForm: public Ui_LoginPageForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINPAGEFORM_H
