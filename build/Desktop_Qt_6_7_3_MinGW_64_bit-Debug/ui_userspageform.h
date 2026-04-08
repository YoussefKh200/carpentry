/********************************************************************************
** Form generated from reading UI file 'userspageform.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USERSPAGEFORM_H
#define UI_USERSPAGEFORM_H

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

class Ui_UsersPageForm
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
    QLabel *labelMdp;
    QLineEdit *mdpEdit;
    QLabel *labelSalaire;
    QLineEdit *salaireEdit;
    QLabel *labelRole;
    QComboBox *roleCombo;
    QLabel *labelEtat;
    QComboBox *etatCombo;
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

    void setupUi(QWidget *UsersPageForm)
    {
        if (UsersPageForm->objectName().isEmpty())
            UsersPageForm->setObjectName("UsersPageForm");
        gridLayout = new QGridLayout(UsersPageForm);
        gridLayout->setObjectName("gridLayout");
        formBox = new QGroupBox(UsersPageForm);
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

        labelMdp = new QLabel(formBox);
        labelMdp->setObjectName("labelMdp");

        formLayout->setWidget(4, QFormLayout::LabelRole, labelMdp);

        mdpEdit = new QLineEdit(formBox);
        mdpEdit->setObjectName("mdpEdit");

        formLayout->setWidget(4, QFormLayout::FieldRole, mdpEdit);

        labelSalaire = new QLabel(formBox);
        labelSalaire->setObjectName("labelSalaire");

        formLayout->setWidget(5, QFormLayout::LabelRole, labelSalaire);

        salaireEdit = new QLineEdit(formBox);
        salaireEdit->setObjectName("salaireEdit");

        formLayout->setWidget(5, QFormLayout::FieldRole, salaireEdit);

        labelRole = new QLabel(formBox);
        labelRole->setObjectName("labelRole");

        formLayout->setWidget(6, QFormLayout::LabelRole, labelRole);

        roleCombo = new QComboBox(formBox);
        roleCombo->setObjectName("roleCombo");

        formLayout->setWidget(6, QFormLayout::FieldRole, roleCombo);

        labelEtat = new QLabel(formBox);
        labelEtat->setObjectName("labelEtat");

        formLayout->setWidget(7, QFormLayout::LabelRole, labelEtat);

        etatCombo = new QComboBox(formBox);
        etatCombo->setObjectName("etatCombo");

        formLayout->setWidget(7, QFormLayout::FieldRole, etatCombo);

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
        labelRecherche = new QLabel(UsersPageForm);
        labelRecherche->setObjectName("labelRecherche");

        filtersLayout->addWidget(labelRecherche);

        searchEdit = new QLineEdit(UsersPageForm);
        searchEdit->setObjectName("searchEdit");

        filtersLayout->addWidget(searchEdit);

        labelTri = new QLabel(UsersPageForm);
        labelTri->setObjectName("labelTri");

        filtersLayout->addWidget(labelTri);

        sortCombo = new QComboBox(UsersPageForm);
        sortCombo->setObjectName("sortCombo");

        filtersLayout->addWidget(sortCombo);


        rightLayout->addLayout(filtersLayout);

        table = new QTableWidget(UsersPageForm);
        table->setObjectName("table");

        rightLayout->addWidget(table);

        chartHost = new QWidget(UsersPageForm);
        chartHost->setObjectName("chartHost");

        rightLayout->addWidget(chartHost);


        gridLayout->addLayout(rightLayout, 0, 1, 1, 1);


        retranslateUi(UsersPageForm);

        QMetaObject::connectSlotsByName(UsersPageForm);
    } // setupUi

    void retranslateUi(QWidget *UsersPageForm)
    {
        formBox->setTitle(QCoreApplication::translate("UsersPageForm", "Formulaire utilisateur", nullptr));
        labelNom->setText(QCoreApplication::translate("UsersPageForm", "Nom", nullptr));
        labelPrenom->setText(QCoreApplication::translate("UsersPageForm", "Prenom", nullptr));
        labelTel->setText(QCoreApplication::translate("UsersPageForm", "Telephone", nullptr));
        labelMail->setText(QCoreApplication::translate("UsersPageForm", "Email", nullptr));
        labelMdp->setText(QCoreApplication::translate("UsersPageForm", "Mot de passe", nullptr));
        labelSalaire->setText(QCoreApplication::translate("UsersPageForm", "Salaire", nullptr));
        labelRole->setText(QCoreApplication::translate("UsersPageForm", "Role", nullptr));
        labelEtat->setText(QCoreApplication::translate("UsersPageForm", "Etat", nullptr));
        addBtn->setText(QCoreApplication::translate("UsersPageForm", "Ajouter", nullptr));
        updateBtn->setText(QCoreApplication::translate("UsersPageForm", "Modifier", nullptr));
        deleteBtn->setText(QCoreApplication::translate("UsersPageForm", "Supprimer", nullptr));
        clearBtn->setText(QCoreApplication::translate("UsersPageForm", "Annuler", nullptr));
        labelRecherche->setText(QCoreApplication::translate("UsersPageForm", "Recherche:", nullptr));
        labelTri->setText(QCoreApplication::translate("UsersPageForm", "Tri:", nullptr));
        (void)UsersPageForm;
    } // retranslateUi

};

namespace Ui {
    class UsersPageForm: public Ui_UsersPageForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERSPAGEFORM_H
