/********************************************************************************
** Form generated from reading UI file 'materiauxpageform.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MATERIAUXPAGEFORM_H
#define UI_MATERIAUXPAGEFORM_H

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
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MateriauxPageForm
{
public:
    QGridLayout *gridLayout;
    QGroupBox *formBox;
    QFormLayout *formLayout;
    QLabel *labelId;
    QSpinBox *idSpin;
    QLabel *labelNom;
    QLineEdit *nomEdit;
    QLabel *labelType;
    QLineEdit *typeEdit;
    QLabel *labelQuantite;
    QSpinBox *quantiteSpin;
    QLabel *labelSeuil;
    QSpinBox *seuilSpin;
    QLabel *labelFournisseur;
    QLineEdit *fournisseurEdit;
    QHBoxLayout *buttonsLayout;
    QPushButton *addBtn;
    QPushButton *updateBtn;
    QPushButton *deleteBtn;
    QPushButton *clearBtn;
    QPushButton *pdfBtn;
    QVBoxLayout *rightLayout;
    QHBoxLayout *filtersLayout;
    QLabel *labelRecherche;
    QLineEdit *searchEdit;
    QLabel *labelTri;
    QComboBox *sortCombo;
    QTableWidget *table;
    QHBoxLayout *chartsLayout;
    QWidget *pieChartHost;
    QWidget *barChartHost;

    void setupUi(QWidget *MateriauxPageForm)
    {
        if (MateriauxPageForm->objectName().isEmpty())
            MateriauxPageForm->setObjectName("MateriauxPageForm");
        gridLayout = new QGridLayout(MateriauxPageForm);
        gridLayout->setObjectName("gridLayout");
        formBox = new QGroupBox(MateriauxPageForm);
        formBox->setObjectName("formBox");
        formLayout = new QFormLayout(formBox);
        formLayout->setObjectName("formLayout");
        labelId = new QLabel(formBox);
        labelId->setObjectName("labelId");

        formLayout->setWidget(0, QFormLayout::LabelRole, labelId);

        idSpin = new QSpinBox(formBox);
        idSpin->setObjectName("idSpin");

        formLayout->setWidget(0, QFormLayout::FieldRole, idSpin);

        labelNom = new QLabel(formBox);
        labelNom->setObjectName("labelNom");

        formLayout->setWidget(1, QFormLayout::LabelRole, labelNom);

        nomEdit = new QLineEdit(formBox);
        nomEdit->setObjectName("nomEdit");

        formLayout->setWidget(1, QFormLayout::FieldRole, nomEdit);

        labelType = new QLabel(formBox);
        labelType->setObjectName("labelType");

        formLayout->setWidget(2, QFormLayout::LabelRole, labelType);

        typeEdit = new QLineEdit(formBox);
        typeEdit->setObjectName("typeEdit");

        formLayout->setWidget(2, QFormLayout::FieldRole, typeEdit);

        labelQuantite = new QLabel(formBox);
        labelQuantite->setObjectName("labelQuantite");

        formLayout->setWidget(3, QFormLayout::LabelRole, labelQuantite);

        quantiteSpin = new QSpinBox(formBox);
        quantiteSpin->setObjectName("quantiteSpin");

        formLayout->setWidget(3, QFormLayout::FieldRole, quantiteSpin);

        labelSeuil = new QLabel(formBox);
        labelSeuil->setObjectName("labelSeuil");

        formLayout->setWidget(4, QFormLayout::LabelRole, labelSeuil);

        seuilSpin = new QSpinBox(formBox);
        seuilSpin->setObjectName("seuilSpin");

        formLayout->setWidget(4, QFormLayout::FieldRole, seuilSpin);

        labelFournisseur = new QLabel(formBox);
        labelFournisseur->setObjectName("labelFournisseur");

        formLayout->setWidget(5, QFormLayout::LabelRole, labelFournisseur);

        fournisseurEdit = new QLineEdit(formBox);
        fournisseurEdit->setObjectName("fournisseurEdit");

        formLayout->setWidget(5, QFormLayout::FieldRole, fournisseurEdit);

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

        pdfBtn = new QPushButton(formBox);
        pdfBtn->setObjectName("pdfBtn");

        buttonsLayout->addWidget(pdfBtn);


        formLayout->setLayout(6, QFormLayout::FieldRole, buttonsLayout);


        gridLayout->addWidget(formBox, 0, 0, 1, 1);

        rightLayout = new QVBoxLayout();
        rightLayout->setObjectName("rightLayout");
        filtersLayout = new QHBoxLayout();
        filtersLayout->setObjectName("filtersLayout");
        labelRecherche = new QLabel(MateriauxPageForm);
        labelRecherche->setObjectName("labelRecherche");

        filtersLayout->addWidget(labelRecherche);

        searchEdit = new QLineEdit(MateriauxPageForm);
        searchEdit->setObjectName("searchEdit");

        filtersLayout->addWidget(searchEdit);

        labelTri = new QLabel(MateriauxPageForm);
        labelTri->setObjectName("labelTri");

        filtersLayout->addWidget(labelTri);

        sortCombo = new QComboBox(MateriauxPageForm);
        sortCombo->setObjectName("sortCombo");

        filtersLayout->addWidget(sortCombo);


        rightLayout->addLayout(filtersLayout);

        table = new QTableWidget(MateriauxPageForm);
        table->setObjectName("table");

        rightLayout->addWidget(table);

        chartsLayout = new QHBoxLayout();
        chartsLayout->setObjectName("chartsLayout");
        pieChartHost = new QWidget(MateriauxPageForm);
        pieChartHost->setObjectName("pieChartHost");

        chartsLayout->addWidget(pieChartHost);

        barChartHost = new QWidget(MateriauxPageForm);
        barChartHost->setObjectName("barChartHost");

        chartsLayout->addWidget(barChartHost);


        rightLayout->addLayout(chartsLayout);


        gridLayout->addLayout(rightLayout, 0, 1, 1, 1);


        retranslateUi(MateriauxPageForm);

        QMetaObject::connectSlotsByName(MateriauxPageForm);
    } // setupUi

    void retranslateUi(QWidget *MateriauxPageForm)
    {
        formBox->setTitle(QCoreApplication::translate("MateriauxPageForm", "Formulaire materiau", nullptr));
        labelId->setText(QCoreApplication::translate("MateriauxPageForm", "ID", nullptr));
        labelNom->setText(QCoreApplication::translate("MateriauxPageForm", "Nom", nullptr));
        labelType->setText(QCoreApplication::translate("MateriauxPageForm", "Type", nullptr));
        labelQuantite->setText(QCoreApplication::translate("MateriauxPageForm", "Quantite", nullptr));
        labelSeuil->setText(QCoreApplication::translate("MateriauxPageForm", "Seuil", nullptr));
        labelFournisseur->setText(QCoreApplication::translate("MateriauxPageForm", "Fournisseur", nullptr));
        addBtn->setText(QCoreApplication::translate("MateriauxPageForm", "Ajouter", nullptr));
        updateBtn->setText(QCoreApplication::translate("MateriauxPageForm", "Modifier", nullptr));
        deleteBtn->setText(QCoreApplication::translate("MateriauxPageForm", "Supprimer", nullptr));
        clearBtn->setText(QCoreApplication::translate("MateriauxPageForm", "Annuler", nullptr));
        pdfBtn->setText(QCoreApplication::translate("MateriauxPageForm", "Exporter PDF", nullptr));
        labelRecherche->setText(QCoreApplication::translate("MateriauxPageForm", "Recherche:", nullptr));
        labelTri->setText(QCoreApplication::translate("MateriauxPageForm", "Tri:", nullptr));
        (void)MateriauxPageForm;
    } // retranslateUi

};

namespace Ui {
    class MateriauxPageForm: public Ui_MateriauxPageForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MATERIAUXPAGEFORM_H
