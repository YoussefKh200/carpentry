/********************************************************************************
** Form generated from reading UI file 'fournisseurspageform.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FOURNISSEURSPAGEFORM_H
#define UI_FOURNISSEURSPAGEFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FournisseursPageForm
{
public:
    QGridLayout *gridLayout;
    QGroupBox *formBox;
    QFormLayout *formLayout;
    QLabel *labelNom;
    QLineEdit *nomEdit;
    QLabel *labelPrenom;
    QLineEdit *prenomEdit;
    QLabel *labelMail;
    QLineEdit *mailEdit;
    QLabel *labelAdresse;
    QLineEdit *adresseEdit;
    QLabel *labelTel;
    QLineEdit *telEdit;
    QLabel *labelQuantite;
    QSpinBox *quantiteSpin;
    QLabel *labelType;
    QLineEdit *typeEdit;
    QLabel *labelDate;
    QDateEdit *dateEdit;
    QHBoxLayout *buttonsLayout;
    QPushButton *addBtn;
    QPushButton *updateBtn;
    QPushButton *deleteBtn;
    QPushButton *clearBtn;
    QVBoxLayout *rightLayout;
    QHBoxLayout *filtersLayout;
    QLabel *labelRecherche;
    QLineEdit *searchEdit;
    QTableWidget *table;
    QWidget *chartHost;

    void setupUi(QWidget *FournisseursPageForm)
    {
        if (FournisseursPageForm->objectName().isEmpty())
            FournisseursPageForm->setObjectName("FournisseursPageForm");
        gridLayout = new QGridLayout(FournisseursPageForm);
        gridLayout->setObjectName("gridLayout");
        formBox = new QGroupBox(FournisseursPageForm);
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

        labelMail = new QLabel(formBox);
        labelMail->setObjectName("labelMail");

        formLayout->setWidget(2, QFormLayout::LabelRole, labelMail);

        mailEdit = new QLineEdit(formBox);
        mailEdit->setObjectName("mailEdit");

        formLayout->setWidget(2, QFormLayout::FieldRole, mailEdit);

        labelAdresse = new QLabel(formBox);
        labelAdresse->setObjectName("labelAdresse");

        formLayout->setWidget(3, QFormLayout::LabelRole, labelAdresse);

        adresseEdit = new QLineEdit(formBox);
        adresseEdit->setObjectName("adresseEdit");

        formLayout->setWidget(3, QFormLayout::FieldRole, adresseEdit);

        labelTel = new QLabel(formBox);
        labelTel->setObjectName("labelTel");

        formLayout->setWidget(4, QFormLayout::LabelRole, labelTel);

        telEdit = new QLineEdit(formBox);
        telEdit->setObjectName("telEdit");

        formLayout->setWidget(4, QFormLayout::FieldRole, telEdit);

        labelQuantite = new QLabel(formBox);
        labelQuantite->setObjectName("labelQuantite");

        formLayout->setWidget(5, QFormLayout::LabelRole, labelQuantite);

        quantiteSpin = new QSpinBox(formBox);
        quantiteSpin->setObjectName("quantiteSpin");

        formLayout->setWidget(5, QFormLayout::FieldRole, quantiteSpin);

        labelType = new QLabel(formBox);
        labelType->setObjectName("labelType");

        formLayout->setWidget(6, QFormLayout::LabelRole, labelType);

        typeEdit = new QLineEdit(formBox);
        typeEdit->setObjectName("typeEdit");

        formLayout->setWidget(6, QFormLayout::FieldRole, typeEdit);

        labelDate = new QLabel(formBox);
        labelDate->setObjectName("labelDate");

        formLayout->setWidget(7, QFormLayout::LabelRole, labelDate);

        dateEdit = new QDateEdit(formBox);
        dateEdit->setObjectName("dateEdit");

        formLayout->setWidget(7, QFormLayout::FieldRole, dateEdit);

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


        formLayout->setLayout(8, QFormLayout::FieldRole, buttonsLayout);


        gridLayout->addWidget(formBox, 0, 0, 1, 1);

        rightLayout = new QVBoxLayout();
        rightLayout->setObjectName("rightLayout");
        filtersLayout = new QHBoxLayout();
        filtersLayout->setObjectName("filtersLayout");
        labelRecherche = new QLabel(FournisseursPageForm);
        labelRecherche->setObjectName("labelRecherche");

        filtersLayout->addWidget(labelRecherche);

        searchEdit = new QLineEdit(FournisseursPageForm);
        searchEdit->setObjectName("searchEdit");

        filtersLayout->addWidget(searchEdit);


        rightLayout->addLayout(filtersLayout);

        table = new QTableWidget(FournisseursPageForm);
        table->setObjectName("table");

        rightLayout->addWidget(table);

        chartHost = new QWidget(FournisseursPageForm);
        chartHost->setObjectName("chartHost");

        rightLayout->addWidget(chartHost);


        gridLayout->addLayout(rightLayout, 0, 1, 1, 1);


        retranslateUi(FournisseursPageForm);

        QMetaObject::connectSlotsByName(FournisseursPageForm);
    } // setupUi

    void retranslateUi(QWidget *FournisseursPageForm)
    {
        formBox->setTitle(QCoreApplication::translate("FournisseursPageForm", "Formulaire fournisseur", nullptr));
        labelNom->setText(QCoreApplication::translate("FournisseursPageForm", "Nom", nullptr));
        labelPrenom->setText(QCoreApplication::translate("FournisseursPageForm", "Prenom", nullptr));
        labelMail->setText(QCoreApplication::translate("FournisseursPageForm", "Mail", nullptr));
        labelAdresse->setText(QCoreApplication::translate("FournisseursPageForm", "Adresse", nullptr));
        labelTel->setText(QCoreApplication::translate("FournisseursPageForm", "Telephone", nullptr));
        labelQuantite->setText(QCoreApplication::translate("FournisseursPageForm", "Quantite", nullptr));
        labelType->setText(QCoreApplication::translate("FournisseursPageForm", "Type", nullptr));
        labelDate->setText(QCoreApplication::translate("FournisseursPageForm", "Date", nullptr));
        addBtn->setText(QCoreApplication::translate("FournisseursPageForm", "Ajouter", nullptr));
        updateBtn->setText(QCoreApplication::translate("FournisseursPageForm", "Modifier", nullptr));
        deleteBtn->setText(QCoreApplication::translate("FournisseursPageForm", "Supprimer", nullptr));
        clearBtn->setText(QCoreApplication::translate("FournisseursPageForm", "Annuler", nullptr));
        labelRecherche->setText(QCoreApplication::translate("FournisseursPageForm", "Recherche:", nullptr));
        (void)FournisseursPageForm;
    } // retranslateUi

};

namespace Ui {
    class FournisseursPageForm: public Ui_FournisseursPageForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FOURNISSEURSPAGEFORM_H
