/********************************************************************************
** Form generated from reading UI file 'commandespageform.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COMMANDESPAGEFORM_H
#define UI_COMMANDESPAGEFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDoubleSpinBox>
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

class Ui_CommandesPageForm
{
public:
    QGridLayout *gridLayout;
    QGroupBox *formBox;
    QFormLayout *formLayout;
    QLabel *labelProduit;
    QLineEdit *produitEdit;
    QLabel *labelQuantite;
    QSpinBox *quantiteSpin;
    QLabel *labelPrix;
    QDoubleSpinBox *prixSpin;
    QLabel *labelTotal;
    QLineEdit *totalEdit;
    QLabel *labelDateCommande;
    QDateEdit *dateCommandeEdit;
    QLabel *labelDateLivraison;
    QDateEdit *dateLivraisonEdit;
    QLabel *labelPaiement;
    QComboBox *modePaiementCombo;
    QHBoxLayout *buttonsLayout;
    QPushButton *computeBtn;
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

    void setupUi(QWidget *CommandesPageForm)
    {
        if (CommandesPageForm->objectName().isEmpty())
            CommandesPageForm->setObjectName("CommandesPageForm");
        gridLayout = new QGridLayout(CommandesPageForm);
        gridLayout->setObjectName("gridLayout");
        formBox = new QGroupBox(CommandesPageForm);
        formBox->setObjectName("formBox");
        formLayout = new QFormLayout(formBox);
        formLayout->setObjectName("formLayout");
        labelProduit = new QLabel(formBox);
        labelProduit->setObjectName("labelProduit");

        formLayout->setWidget(0, QFormLayout::LabelRole, labelProduit);

        produitEdit = new QLineEdit(formBox);
        produitEdit->setObjectName("produitEdit");

        formLayout->setWidget(0, QFormLayout::FieldRole, produitEdit);

        labelQuantite = new QLabel(formBox);
        labelQuantite->setObjectName("labelQuantite");

        formLayout->setWidget(1, QFormLayout::LabelRole, labelQuantite);

        quantiteSpin = new QSpinBox(formBox);
        quantiteSpin->setObjectName("quantiteSpin");

        formLayout->setWidget(1, QFormLayout::FieldRole, quantiteSpin);

        labelPrix = new QLabel(formBox);
        labelPrix->setObjectName("labelPrix");

        formLayout->setWidget(2, QFormLayout::LabelRole, labelPrix);

        prixSpin = new QDoubleSpinBox(formBox);
        prixSpin->setObjectName("prixSpin");

        formLayout->setWidget(2, QFormLayout::FieldRole, prixSpin);

        labelTotal = new QLabel(formBox);
        labelTotal->setObjectName("labelTotal");

        formLayout->setWidget(3, QFormLayout::LabelRole, labelTotal);

        totalEdit = new QLineEdit(formBox);
        totalEdit->setObjectName("totalEdit");

        formLayout->setWidget(3, QFormLayout::FieldRole, totalEdit);

        labelDateCommande = new QLabel(formBox);
        labelDateCommande->setObjectName("labelDateCommande");

        formLayout->setWidget(4, QFormLayout::LabelRole, labelDateCommande);

        dateCommandeEdit = new QDateEdit(formBox);
        dateCommandeEdit->setObjectName("dateCommandeEdit");

        formLayout->setWidget(4, QFormLayout::FieldRole, dateCommandeEdit);

        labelDateLivraison = new QLabel(formBox);
        labelDateLivraison->setObjectName("labelDateLivraison");

        formLayout->setWidget(5, QFormLayout::LabelRole, labelDateLivraison);

        dateLivraisonEdit = new QDateEdit(formBox);
        dateLivraisonEdit->setObjectName("dateLivraisonEdit");

        formLayout->setWidget(5, QFormLayout::FieldRole, dateLivraisonEdit);

        labelPaiement = new QLabel(formBox);
        labelPaiement->setObjectName("labelPaiement");

        formLayout->setWidget(6, QFormLayout::LabelRole, labelPaiement);

        modePaiementCombo = new QComboBox(formBox);
        modePaiementCombo->setObjectName("modePaiementCombo");

        formLayout->setWidget(6, QFormLayout::FieldRole, modePaiementCombo);

        buttonsLayout = new QHBoxLayout();
        buttonsLayout->setObjectName("buttonsLayout");
        computeBtn = new QPushButton(formBox);
        computeBtn->setObjectName("computeBtn");

        buttonsLayout->addWidget(computeBtn);

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
        labelRecherche = new QLabel(CommandesPageForm);
        labelRecherche->setObjectName("labelRecherche");

        filtersLayout->addWidget(labelRecherche);

        searchEdit = new QLineEdit(CommandesPageForm);
        searchEdit->setObjectName("searchEdit");

        filtersLayout->addWidget(searchEdit);


        rightLayout->addLayout(filtersLayout);

        table = new QTableWidget(CommandesPageForm);
        table->setObjectName("table");

        rightLayout->addWidget(table);

        chartHost = new QWidget(CommandesPageForm);
        chartHost->setObjectName("chartHost");

        rightLayout->addWidget(chartHost);


        gridLayout->addLayout(rightLayout, 0, 1, 1, 1);


        retranslateUi(CommandesPageForm);

        QMetaObject::connectSlotsByName(CommandesPageForm);
    } // setupUi

    void retranslateUi(QWidget *CommandesPageForm)
    {
        formBox->setTitle(QCoreApplication::translate("CommandesPageForm", "Formulaire commande", nullptr));
        labelProduit->setText(QCoreApplication::translate("CommandesPageForm", "Produit", nullptr));
        labelQuantite->setText(QCoreApplication::translate("CommandesPageForm", "Quantite", nullptr));
        labelPrix->setText(QCoreApplication::translate("CommandesPageForm", "Prix unitaire", nullptr));
        labelTotal->setText(QCoreApplication::translate("CommandesPageForm", "Total", nullptr));
        labelDateCommande->setText(QCoreApplication::translate("CommandesPageForm", "Date commande", nullptr));
        labelDateLivraison->setText(QCoreApplication::translate("CommandesPageForm", "Date livraison", nullptr));
        labelPaiement->setText(QCoreApplication::translate("CommandesPageForm", "Mode paiement", nullptr));
        computeBtn->setText(QCoreApplication::translate("CommandesPageForm", "Calculer", nullptr));
        addBtn->setText(QCoreApplication::translate("CommandesPageForm", "Ajouter", nullptr));
        updateBtn->setText(QCoreApplication::translate("CommandesPageForm", "Modifier", nullptr));
        deleteBtn->setText(QCoreApplication::translate("CommandesPageForm", "Supprimer", nullptr));
        clearBtn->setText(QCoreApplication::translate("CommandesPageForm", "Annuler", nullptr));
        labelRecherche->setText(QCoreApplication::translate("CommandesPageForm", "Recherche:", nullptr));
        (void)CommandesPageForm;
    } // retranslateUi

};

namespace Ui {
    class CommandesPageForm: public Ui_CommandesPageForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMMANDESPAGEFORM_H
