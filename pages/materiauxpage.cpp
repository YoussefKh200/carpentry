#include "materiauxpage.h"

#include <QBarCategoryAxis>
#include <QBarSeries>
#include <QBarSet>
#include <QChart>
#include <QChartView>
#include <QComboBox>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QIcon>
#include <QLineEdit>
#include <QMessageBox>
#include <QPainter>
#include <QPdfWriter>
#include <QPieSeries>
#include <QPushButton>
#include <QSpinBox>
#include <QTableWidget>
#include <QToolButton>
#include <QVBoxLayout>

#include "ui_materiauxpageform.h"

MateriauxPage::MateriauxPage(QWidget *parent)
    : BaseGestionPage("Gestion Materiaux", "CRUD + recherche + tri + export PDF + doubles graphiques", parent),
      ui_(new Ui::MateriauxPageForm),
      selectedId_(-1),
      pieChartView_(new QChartView(new QChart(), this)),
      barChartView_(new QChartView(new QChart(), this))
{
    auto *pageRoot = new QWidget(this);
    ui_->setupUi(pageRoot);
    contentLayout_->addWidget(pageRoot);

    ui_->idSpin->setRange(1, 9999999);
    ui_->quantiteSpin->setRange(0, 1000000);
    ui_->seuilSpin->setRange(0, 1000000);

    ui_->searchEdit->setPlaceholderText("Rechercher materiau...");
    ui_->sortCombo->addItems({"Par ID", "Par nom", "Par quantite"});

    ui_->table->setColumnCount(7);
    ui_->table->setHorizontalHeaderLabels({"ID", "Nom", "Type", "Quantite", "Seuil", "Fournisseur", "Action"});
    ui_->table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui_->table->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui_->table->setSelectionMode(QAbstractItemView::SingleSelection);
    ui_->table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    pieChartView_->setMinimumHeight(260);
    barChartView_->setMinimumHeight(260);

    auto *pieLayout = new QVBoxLayout(ui_->pieChartHost);
    pieLayout->setContentsMargins(0, 0, 0, 0);
    pieLayout->addWidget(pieChartView_);

    auto *barLayout = new QVBoxLayout(ui_->barChartHost);
    barLayout->setContentsMargins(0, 0, 0, 0);
    barLayout->addWidget(barChartView_);

    ui_->addBtn->setText("Valider");
    ui_->addBtn->setStyleSheet("background:#a85a31; color:white; border:1px solid #8f4a26; border-radius:11px; font-weight:800;");
    ui_->pdfBtn->setStyleSheet("background:#3f74af; color:white; border:1px solid #345f8f; border-radius:11px; font-weight:800;");
    ui_->updateBtn->setVisible(false);
    ui_->deleteBtn->setVisible(false);
    ui_->clearBtn->setVisible(false);

    connect(ui_->addBtn, &QPushButton::clicked, this, &MateriauxPage::onAdd);
    connect(ui_->updateBtn, &QPushButton::clicked, this, &MateriauxPage::onUpdate);
    connect(ui_->deleteBtn, &QPushButton::clicked, this, &MateriauxPage::onDelete);
    connect(ui_->clearBtn, &QPushButton::clicked, this, &MateriauxPage::clearForm);
    connect(ui_->pdfBtn, &QPushButton::clicked, this, &MateriauxPage::onExportPdf);
    connect(ui_->searchEdit, &QLineEdit::textChanged, this, &MateriauxPage::onFilterChanged);
    connect(ui_->sortCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MateriauxPage::onFilterChanged);
    connect(ui_->table, &QTableWidget::itemSelectionChanged, this, &MateriauxPage::onSelectionChanged);

    refreshTable();
}

MateriauxPage::~MateriauxPage()
{
    delete ui_;
}

MateriauData MateriauxPage::collectData() const
{
    MateriauData m;
    m.id = ui_->idSpin->value();
    m.nom = ui_->nomEdit->text();
    m.type = ui_->typeEdit->text();
    m.quantite = ui_->quantiteSpin->value();
    m.seuil = ui_->seuilSpin->value();
    m.fournisseur = ui_->fournisseurEdit->text();
    return m;
}

void MateriauxPage::onAdd()
{
    if (selectedId_ > 0) {
        onUpdate();
        return;
    }

    MateriauData m = collectData();
    QString err;
    if (!crud_.addMateriau(m, err)) {
        QMessageBox::critical(this, "Ajout materiau", err);
        return;
    }
    clearForm();
    refreshTable();
}

void MateriauxPage::onUpdate()
{
    if (selectedId_ <= 0) {
        QMessageBox::information(this, "Modification", "Selectionnez un materiau.");
        return;
    }

    MateriauData m = collectData();
    m.id = selectedId_;
    QString err;
    if (!crud_.updateMateriau(m, err)) {
        QMessageBox::critical(this, "Modification materiau", err);
        return;
    }
    clearForm();
    refreshTable();
}

void MateriauxPage::onDelete()
{
    if (selectedId_ <= 0) {
        QMessageBox::information(this, "Suppression", "Selectionnez un materiau.");
        return;
    }

    QString err;
    if (!crud_.deleteMateriau(selectedId_, err)) {
        QMessageBox::critical(this, "Suppression materiau", err);
        return;
    }
    clearForm();
    refreshTable();
}

void MateriauxPage::onSelectionChanged()
{
    const int row = ui_->table->currentRow();
    if (row < 0) return;

    selectedId_ = ui_->table->item(row, 0)->text().toInt();
    ui_->idSpin->setValue(selectedId_);
    ui_->nomEdit->setText(ui_->table->item(row, 1)->text());
    ui_->typeEdit->setText(ui_->table->item(row, 2)->text());
    ui_->quantiteSpin->setValue(ui_->table->item(row, 3)->text().toInt());
    ui_->seuilSpin->setValue(ui_->table->item(row, 4)->text().toInt());
    ui_->fournisseurEdit->setText(ui_->table->item(row, 5)->text());
}

void MateriauxPage::onFilterChanged()
{
    refreshTable();
}

void MateriauxPage::clearForm()
{
    selectedId_ = -1;
    ui_->idSpin->setValue(1);
    ui_->nomEdit->clear();
    ui_->typeEdit->clear();
    ui_->quantiteSpin->setValue(0);
    ui_->seuilSpin->setValue(0);
    ui_->fournisseurEdit->clear();
    ui_->table->clearSelection();
}

void MateriauxPage::refreshTable()
{
    const QString orderBy = (ui_->sortCombo->currentIndex() == 1) ? "nom"
                          : (ui_->sortCombo->currentIndex() == 2) ? "quantite"
                          : QString();

    const QList<MateriauData> list = crud_.searchByNom(ui_->searchEdit->text(), orderBy);

    ui_->table->setRowCount(list.size());
    for (int i = 0; i < list.size(); ++i) {
        const MateriauData &m = list[i];
        ui_->table->setItem(i, 0, new QTableWidgetItem(QString::number(m.id)));
        ui_->table->setItem(i, 1, new QTableWidgetItem(m.nom));
        ui_->table->setItem(i, 2, new QTableWidgetItem(m.type));
        ui_->table->setItem(i, 3, new QTableWidgetItem(QString::number(m.quantite)));
        ui_->table->setItem(i, 4, new QTableWidgetItem(QString::number(m.seuil)));
        ui_->table->setItem(i, 5, new QTableWidgetItem(m.fournisseur));

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

        ui_->table->setCellWidget(i, 6, actions);
    }

    refreshCharts();
}

void MateriauxPage::refreshCharts()
{
    {
        auto *series = new QPieSeries(this);
        const auto counts = crud_.countByNom();
        for (const auto &c : counts) {
            const QString label = c.first.isEmpty() ? "Sans nom" : c.first;
            series->append(label, c.second);
        }

        auto *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Repartition des materiaux (par nom)");
        pieChartView_->setChart(chart);
    }

    {
        auto *set = new QBarSet("Quantite totale");
        QStringList labels;
        const auto totals = crud_.quantiteByType();
        for (const auto &t : totals) {
            labels << (t.first.isEmpty() ? "Sans type" : t.first);
            *set << t.second;
        }

        auto *series = new QBarSeries(this);
        series->append(set);

        auto *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Quantite par type");
        chart->createDefaultAxes();

        auto *axis = new QBarCategoryAxis();
        axis->append(QStringList(labels));
        chart->addAxis(axis, Qt::AlignBottom);
        series->attachAxis(axis);

        barChartView_->setChart(chart);
    }
}

void MateriauxPage::onExportPdf()
{
    if (ui_->table->rowCount() == 0) {
        QMessageBox::warning(this, "Export PDF", "Aucune donnee a exporter.");
        return;
    }

    const QString fileName = QFileDialog::getSaveFileName(this, "Exporter PDF", "materiaux.pdf", "PDF (*.pdf)");
    if (fileName.isEmpty()) return;

    QPdfWriter writer(fileName);
    writer.setPageSize(QPageSize::A4);

    QPainter p(&writer);
    p.setRenderHint(QPainter::Antialiasing, true);

    p.setFont(QFont("Arial", 14, QFont::Bold));
    p.drawText(120, 140, "Liste des Materiaux");

    p.setFont(QFont("Arial", 9));

    int y = 220;
    p.drawText(50, y, "ID");
    p.drawText(120, y, "Nom");
    p.drawText(260, y, "Type");
    p.drawText(360, y, "Quantite");
    p.drawText(450, y, "Seuil");
    p.drawText(520, y, "Fournisseur");
    y += 20;

    for (int r = 0; r < ui_->table->rowCount(); ++r) {
        if (y > writer.height() - 80) {
            writer.newPage();
            y = 80;
        }

        p.drawText(50, y, ui_->table->item(r, 0)->text());
        p.drawText(120, y, ui_->table->item(r, 1)->text());
        p.drawText(260, y, ui_->table->item(r, 2)->text());
        p.drawText(360, y, ui_->table->item(r, 3)->text());
        p.drawText(450, y, ui_->table->item(r, 4)->text());
        p.drawText(520, y, ui_->table->item(r, 5)->text());
        y += 18;
    }

    p.end();
    QMessageBox::information(this, "Export PDF", "Export termine avec succes.");
}
