#include "fournisseurspage.h"

#include <QBarCategoryAxis>
#include <QBarSeries>
#include <QBarSet>
#include <QChart>
#include <QChartView>
#include <QComboBox>
#include <QDateEdit>
#include <QFileDialog>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QIcon>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include <QPainter>
#include <QPdfWriter>
#include <QPushButton>
#include <QSpinBox>
#include <QTableWidget>
#include <QToolButton>
#include <QVBoxLayout>

#include "ui_fournisseurspageform.h"

FournisseursPage::FournisseursPage(QWidget *parent)
    : BaseGestionPage("Gestion des Fournisseurs", "CRUD + recherche type + statistiques quantite", parent),
      ui_(new Ui::FournisseursPageForm),
      selectedId_(-1),
      chartView_(new QChartView(new QChart(), this)),
      sortCombo_(new QComboBox(this))
{
    auto *pageRoot = new QWidget(this);
    ui_->setupUi(pageRoot);
    contentLayout_->addWidget(pageRoot);

    ui_->quantiteSpin->setRange(0, 1000000);
    ui_->dateEdit->setCalendarPopup(true);
    ui_->dateEdit->setDate(QDate::currentDate());

    ui_->searchEdit->setPlaceholderText("Rechercher par type...");
    sortCombo_->addItem("Tri: ID (defaut)", "");
    sortCombo_->addItem("Tri: Nom (A-Z)", "nom");
    sortCombo_->addItem("Tri: Type (A-Z)", "type");
    sortCombo_->addItem("Tri: Quantite (grand -> petit)", "quantite");
    sortCombo_->addItem("Tri: Date (recent -> ancien)", "date");
    ui_->filtersLayout->addWidget(new QLabel("Tri:", this));
    ui_->filtersLayout->addWidget(sortCombo_);

    ui_->table->setColumnCount(10);
    ui_->table->setHorizontalHeaderLabels({"ID", "Nom", "Prenom", "Mail", "Adresse", "Tel", "Quantite", "Type", "Date", "Action"});
    ui_->table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui_->table->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui_->table->setSelectionMode(QAbstractItemView::SingleSelection);
    ui_->table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    chartView_->setMinimumHeight(260);
    auto *chartLayout = new QVBoxLayout(ui_->chartHost);
    chartLayout->setContentsMargins(0, 0, 0, 0);
    chartLayout->addWidget(chartView_);

    ui_->addBtn->setText("Valider");
    ui_->addBtn->setStyleSheet("background:#a85a31; color:white; border:1px solid #8f4a26; border-radius:11px; font-weight:800;");
    ui_->addBtn->setMinimumHeight(36);
    ui_->updateBtn->setVisible(false);
    ui_->deleteBtn->setVisible(false);
    ui_->clearBtn->setText("Exporter PDF");
    ui_->clearBtn->setStyleSheet("background:#3f74af; color:white; border:1px solid #345f8f; border-radius:11px; font-weight:800;");
    ui_->clearBtn->setMinimumHeight(36);
    ui_->clearBtn->setVisible(true);

    connect(ui_->addBtn, &QPushButton::clicked, this, &FournisseursPage::onAdd);
    connect(ui_->updateBtn, &QPushButton::clicked, this, &FournisseursPage::onUpdate);
    connect(ui_->deleteBtn, &QPushButton::clicked, this, &FournisseursPage::onDelete);
    connect(ui_->clearBtn, &QPushButton::clicked, this, &FournisseursPage::onExportPdf);
    connect(ui_->searchEdit, &QLineEdit::textChanged, this, &FournisseursPage::onSearchChanged);
    connect(sortCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FournisseursPage::onSortChanged);
    connect(ui_->table, &QTableWidget::itemSelectionChanged, this, &FournisseursPage::onSelectionChanged);

    refreshTable();
}

FournisseursPage::~FournisseursPage()
{
    delete ui_;
}

FournisseurData FournisseursPage::collectData() const
{
    FournisseurData f;
    f.nom = ui_->nomEdit->text();
    f.prenom = ui_->prenomEdit->text();
    f.mail = ui_->mailEdit->text();
    f.adresse = ui_->adresseEdit->text();
    f.tel = ui_->telEdit->text();
    f.quantite = ui_->quantiteSpin->value();
    f.type = ui_->typeEdit->text();
    f.date = ui_->dateEdit->date();
    return f;
}

void FournisseursPage::onAdd()
{
    if (selectedId_ > 0) {
        onUpdate();
        return;
    }

    FournisseurData f = collectData();
    QString err;
    if (!crud_.addFournisseur(f, err)) {
        QMessageBox::critical(this, "Ajout fournisseur", err);
        return;
    }
    clearForm();
    refreshTable();
}

void FournisseursPage::onUpdate()
{
    if (selectedId_ <= 0) {
        QMessageBox::information(this, "Modification", "Selectionnez un fournisseur.");
        return;
    }

    FournisseurData f = collectData();
    f.id = selectedId_;
    QString err;
    if (!crud_.updateFournisseur(f, err)) {
        QMessageBox::critical(this, "Modification fournisseur", err);
        return;
    }
    clearForm();
    refreshTable();
}

void FournisseursPage::onDelete()
{
    if (selectedId_ <= 0) {
        QMessageBox::information(this, "Suppression", "Selectionnez un fournisseur.");
        return;
    }

    QString err;
    if (!crud_.deleteFournisseur(selectedId_, err)) {
        QMessageBox::critical(this, "Suppression fournisseur", err);
        return;
    }
    clearForm();
    refreshTable();
}

void FournisseursPage::onSelectionChanged()
{
    const int row = ui_->table->currentRow();
    if (row < 0) return;

    selectedId_ = ui_->table->item(row, 0)->text().toInt();
    ui_->nomEdit->setText(ui_->table->item(row, 1)->text());
    ui_->prenomEdit->setText(ui_->table->item(row, 2)->text());
    ui_->mailEdit->setText(ui_->table->item(row, 3)->text());
    ui_->adresseEdit->setText(ui_->table->item(row, 4)->text());
    ui_->telEdit->setText(ui_->table->item(row, 5)->text());
    ui_->quantiteSpin->setValue(ui_->table->item(row, 6)->text().toInt());
    ui_->typeEdit->setText(ui_->table->item(row, 7)->text());
    ui_->dateEdit->setDate(QDate::fromString(ui_->table->item(row, 8)->text(), Qt::ISODate));
}

void FournisseursPage::onSearchChanged()
{
    refreshTable();
}

void FournisseursPage::onSortChanged()
{
    refreshTable();
}

void FournisseursPage::clearForm()
{
    selectedId_ = -1;
    ui_->nomEdit->clear();
    ui_->prenomEdit->clear();
    ui_->mailEdit->clear();
    ui_->adresseEdit->clear();
    ui_->telEdit->clear();
    ui_->quantiteSpin->setValue(0);
    ui_->typeEdit->clear();
    ui_->dateEdit->setDate(QDate::currentDate());
    ui_->table->clearSelection();
}

void FournisseursPage::refreshTable()
{
    const QString sortKey = sortCombo_->currentData().toString();
    const QList<FournisseurData> list = crud_.searchFournisseurs(ui_->searchEdit->text(), sortKey);

    ui_->table->setRowCount(list.size());
    for (int i = 0; i < list.size(); ++i) {
        const FournisseurData &f = list[i];
        ui_->table->setItem(i, 0, new QTableWidgetItem(QString::number(f.id)));
        ui_->table->setItem(i, 1, new QTableWidgetItem(f.nom));
        ui_->table->setItem(i, 2, new QTableWidgetItem(f.prenom));
        ui_->table->setItem(i, 3, new QTableWidgetItem(f.mail));
        ui_->table->setItem(i, 4, new QTableWidgetItem(f.adresse));
        ui_->table->setItem(i, 5, new QTableWidgetItem(f.tel));
        ui_->table->setItem(i, 6, new QTableWidgetItem(QString::number(f.quantite)));
        ui_->table->setItem(i, 7, new QTableWidgetItem(f.type));
        ui_->table->setItem(i, 8, new QTableWidgetItem(f.date.toString(Qt::ISODate)));

        auto *actions = new QWidget(ui_->table);
        auto *actionsLayout = new QHBoxLayout(actions);
        actionsLayout->setContentsMargins(0, 0, 0, 0);
        actionsLayout->setSpacing(6);

        auto *modifyBtn = new QToolButton(actions);
        modifyBtn->setIcon(QIcon(":/img/modify.png"));
        modifyBtn->setToolTip("Modifier");
        modifyBtn->setAutoRaise(true);

        auto *deleteBtn = new QToolButton(actions);
        deleteBtn->setIcon(QIcon(":/img/delete.png"));
        deleteBtn->setToolTip("Supprimer");
        deleteBtn->setAutoRaise(true);

        actionsLayout->addWidget(modifyBtn);
        actionsLayout->addWidget(deleteBtn);

        connect(modifyBtn, &QToolButton::clicked, this, [this, i]() {
            ui_->table->selectRow(i);
            onSelectionChanged();
        });
        connect(deleteBtn, &QToolButton::clicked, this, [this, i]() {
            ui_->table->selectRow(i);
            onSelectionChanged();
            onDelete();
        });

        ui_->table->setCellWidget(i, 9, actions);
    }

    refreshChart();
}

void FournisseursPage::refreshChart()
{
    const QList<QPair<QString, int>> stats = crud_.quantiteByType();

    auto *set = new QBarSet("Quantite");
    QStringList labels;
    for (const auto &s : stats) {
        labels << s.first;
        *set << s.second;
    }

    auto *series = new QBarSeries(this);
    series->append(set);

    auto *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Quantite par type fournisseur");
    chart->createDefaultAxes();

    auto *axis = new QBarCategoryAxis();
    axis->append(labels);
    chart->addAxis(axis, Qt::AlignBottom);
    series->attachAxis(axis);

    chartView_->setChart(chart);
}

void FournisseursPage::onExportPdf()
{
    if (ui_->table->rowCount() == 0) {
        QMessageBox::warning(this, "Export PDF", "Aucune donnee a exporter.");
        return;
    }
    const QString fileName = QFileDialog::getSaveFileName(this, "Exporter PDF", "fournisseurs.pdf", "PDF (*.pdf)");
    if (fileName.isEmpty()) return;

    QPdfWriter writer(fileName);
    writer.setPageSize(QPageSize::A4);
    QPainter p(&writer);
    p.setFont(QFont("Arial", 10));
    int y = 120;
    for (int c = 0; c < ui_->table->columnCount() - 1; ++c) {
        p.drawText(30 + c * 68, y, ui_->table->horizontalHeaderItem(c)->text());
    }
    y += 25;
    for (int r = 0; r < ui_->table->rowCount(); ++r) {
        if (y > writer.height() - 60) {
            writer.newPage();
            y = 80;
        }
        for (int c = 0; c < ui_->table->columnCount() - 1; ++c) {
            auto *item = ui_->table->item(r, c);
            p.drawText(30 + c * 68, y, item ? item->text() : "");
        }
        y += 20;
    }
    p.end();
}
