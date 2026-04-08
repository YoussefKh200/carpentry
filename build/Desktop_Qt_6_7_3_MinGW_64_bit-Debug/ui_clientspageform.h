/********************************************************************************
** Form generated from reading UI file 'clientspageform.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIENTSPAGEFORM_H
#define UI_CLIENTSPAGEFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ClientsPageForm
{
public:
    QGridLayout *gridLayout;
    QGroupBox *formBox;
    QFormLayout *formLayout;
    QLabel *labelNom;
    QLineEdit *nomEdit;
    QLabel *labelPrenom;
    QLineEdit *prenomEdit;
    QLabel *labelTel;
    QLineEdit *telEdit;
    QLabel *labelMail;
    QLineEdit *mailEdit;
    QLabel *labelAdresse;
    QComboBox *adresseCombo;
    QLabel *labelDate;
    QLineEdit *dateEdit;
    QLabel *labelStatut;
    QComboBox *statutCombo;
    QHBoxLayout *buttonsLayout;
    QPushButton *addBtn;
    QPushButton *updateBtn;
    QPushButton *deleteBtn;
    QPushButton *clearBtn;
    QVBoxLayout *rightLayout;
    QHBoxLayout *filtersLayout;
    QLabel *labelRecherche;
    QLineEdit *searchEdit;
    QLabel *labelTri;
    QComboBox *sortCombo;
    QTableWidget *table;
    QWidget *chartHost;

    void setupUi(QWidget *ClientsPageForm)
    {
        if (ClientsPageForm->objectName().isEmpty())
            ClientsPageForm->setObjectName("ClientsPageForm");
        gridLayout = new QGridLayout(ClientsPageForm);
        gridLayout->setObjectName("gridLayout");
        formBox = new QGroupBox(ClientsPageForm);
        formBox->setObjectName("formBox");
        formLayout = new QFormLayout(formBox);
        formLayout->setObjectName("formLayout");
        labelNom = new QLabel(formBox);
        labelNom->setObjectName("labelNom");

        formLayout->setWidget(0, QFormLayout::LabelRole, labelNom);

        nomEdit = new QLineEdit(formBox);
        nomEdit->setObjectName("nomEdit");

        formLayout->setWidget(0, QFormLayout::FieldRole, nomEdit);

        labelPrenom = new QLabel(formBox);
        labelPrenom->setObjectName("labelPrenom");

        formLayout->setWidget(1, QFormLayout::LabelRole, labelPrenom);

        prenomEdit = new QLineEdit(formBox);
        prenomEdit->setObjectName("prenomEdit");

        formLayout->setWidget(1, QFormLayout::FieldRole, prenomEdit);

        labelTel = new QLabel(formBox);
        labelTel->setObjectName("labelTel");

        formLayout->setWidget(2, QFormLayout::LabelRole, labelTel);

        telEdit = new QLineEdit(formBox);
        telEdit->setObjectName("telEdit");

        formLayout->setWidget(2, QFormLayout::FieldRole, telEdit);

        labelMail = new QLabel(formBox);
        labelMail->setObjectName("labelMail");

        formLayout->setWidget(3, QFormLayout::LabelRole, labelMail);

        mailEdit = new QLineEdit(formBox);
        mailEdit->setObjectName("mailEdit");

        formLayout->setWidget(3, QFormLayout::FieldRole, mailEdit);

        labelAdresse = new QLabel(formBox);
        labelAdresse->setObjectName("labelAdresse");

        formLayout->setWidget(4, QFormLayout::LabelRole, labelAdresse);

        adresseCombo = new QComboBox(formBox);
        adresseCombo->setObjectName("adresseCombo");

        formLayout->setWidget(4, QFormLayout::FieldRole, adresseCombo);

        labelDate = new QLabel(formBox);
        labelDate->setObjectName("labelDate");

        formLayout->setWidget(5, QFormLayout::LabelRole, labelDate);

        dateEdit = new QLineEdit(formBox);
        dateEdit->setObjectName("dateEdit");

        formLayout->setWidget(5, QFormLayout::FieldRole, dateEdit);

        labelStatut = new QLabel(formBox);
        labelStatut->setObjectName("labelStatut");

        formLayout->setWidget(6, QFormLayout::LabelRole, labelStatut);

        statutCombo = new QComboBox(formBox);
        statutCombo->setObjectName("statutCombo");

        formLayout->setWidget(6, QFormLayout::FieldRole, statutCombo);

        buttonsLayout = new QHBoxLayout();
        buttonsLayout->setObjectName("buttonsLayout");
        addBtn = new QPushButton(formBox);
        addBtn->setObjectName("addBtn");

        buttonsLayout->addWidget(addBtn);

        updateBtn = new QPushButton(formBox);
        updateBtn->setObjectName("updateBtn");

        buttonsLayout->addWidget(updateBtn);

        deleteBtn = new QPushButton(formBox);
        deleteBtn->setObjectName("deleteBtn");

        buttonsLayout->addWidget(deleteBtn);

        clearBtn = new QPushButton(formBox);
        clearBtn->setObjectName("clearBtn");

        buttonsLayout->addWidget(clearBtn);


        formLayout->setLayout(7, QFormLayout::FieldRole, buttonsLayout);


        gridLayout->addWidget(formBox, 0, 0, 1, 1);

        rightLayout = new QVBoxLayout();
        rightLayout->setObjectName("rightLayout");
        filtersLayout = new QHBoxLayout();
        filtersLayout->setObjectName("filtersLayout");
        labelRecherche = new QLabel(ClientsPageForm);
        labelRecherche->setObjectName("labelRecherche");

        filtersLayout->addWidget(labelRecherche);

        searchEdit = new QLineEdit(ClientsPageForm);
        searchEdit->setObjectName("searchEdit");

        filtersLayout->addWidget(searchEdit);

        labelTri = new QLabel(ClientsPageForm);
        labelTri->setObjectName("labelTri");

        filtersLayout->addWidget(labelTri);

        sortCombo = new QComboBox(ClientsPageForm);
        sortCombo->setObjectName("sortCombo");

        filtersLayout->addWidget(sortCombo);


        rightLayout->addLayout(filtersLayout);

        table = new QTableWidget(ClientsPageForm);
        table->setObjectName("table");

        rightLayout->addWidget(table);

        chartHost = new QWidget(ClientsPageForm);
        chartHost->setObjectName("chartHost");

        rightLayout->addWidget(chartHost);


        gridLayout->addLayout(rightLayout, 0, 1, 1, 1);


        retranslateUi(ClientsPageForm);

        QMetaObject::connectSlotsByName(ClientsPageForm);
    } // setupUi

    void retranslateUi(QWidget *ClientsPageForm)
    {
        formBox->setTitle(QCoreApplication::translate("ClientsPageForm", "Formulaire client", nullptr));
        labelNom->setText(QCoreApplication::translate("ClientsPageForm", "Nom", nullptr));
        labelPrenom->setText(QCoreApplication::translate("ClientsPageForm", "Prenom", nullptr));
        labelTel->setText(QCoreApplication::translate("ClientsPageForm", "Telephone", nullptr));
        labelMail->setText(QCoreApplication::translate("ClientsPageForm", "Email", nullptr));
        labelAdresse->setText(QCoreApplication::translate("ClientsPageForm", "Adresse", nullptr));
        labelDate->setText(QCoreApplication::translate("ClientsPageForm", "Date inscription", nullptr));
        labelStatut->setText(QCoreApplication::translate("ClientsPageForm", "Statut", nullptr));
        addBtn->setText(QCoreApplication::translate("ClientsPageForm", "Ajouter", nullptr));
        updateBtn->setText(QCoreApplication::translate("ClientsPageForm", "Modifier", nullptr));
        deleteBtn->setText(QCoreApplication::translate("ClientsPageForm", "Supprimer", nullptr));
        clearBtn->setText(QCoreApplication::translate("ClientsPageForm", "Annuler", nullptr));
        labelRecherche->setText(QCoreApplication::translate("ClientsPageForm", "Recherche:", nullptr));
        labelTri->setText(QCoreApplication::translate("ClientsPageForm", "Tri:", nullptr));
        (void)ClientsPageForm;
    } // retranslateUi

};

namespace Ui {
    class ClientsPageForm: public Ui_ClientsPageForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIENTSPAGEFORM_H
