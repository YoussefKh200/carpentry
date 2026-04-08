#include "commandespage.h"

#include <QBarCategoryAxis>
#include <QBarSeries>
#include <QBarSet>
#include <QChart>
#include <QChartView>
#include <QComboBox>
#include <QDateEdit>
#include <QDoubleSpinBox>
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

#include "ui_commandespageform.h"

CommandesPage::CommandesPage(QWidget *parent)
    : BaseGestionPage("Gestion des Commandes", "CRUD + total automatique + validation date + top produits", parent),
      ui_(new Ui::CommandesPageForm),
      selectedId_(-1),
      chartView_(new QChartView(new QChart(), this)),
      sortCombo_(new QComboBox(this))
{
    auto *pageRoot = new QWidget(this);
    ui_->setupUi(pageRoot);
    contentLayout_->addWidget(pageRoot);

    ui_->quantiteSpin->setRange(1, 1000000);
    ui_->prixSpin->setRange(0.01, 100000000.0);
    ui_->prixSpin->setDecimals(2);
    ui_->totalEdit->setReadOnly(true);

    ui_->dateCommandeEdit->setCalendarPopup(true);
    ui_->dateLivraisonEdit->setCalendarPopup(true);
    ui_->dateCommandeEdit->setDate(QDate::currentDate());
    ui_->dateLivraisonEdit->setDate(QDate::currentDate().addDays(2));

    ui_->modePaiementCombo->addItems({"Especes", "Carte", "Virement", "Cheque"});

    ui_->searchEdit->setPlaceholderText("Rechercher produit...");
    sortCombo_->addItem("Tri: ID (defaut)", "");
    sortCombo_->addItem("Tri: Produit (A-Z)", "produit");
    sortCombo_->addItem("Tri: Date commande (recent -> ancien)", "date");
    sortCombo_->addItem("Tri: Total (grand -> petit)", "total");
    ui_->filtersLayout->addWidget(new QLabel("Tri:", this));
    ui_->filtersLayout->addWidget(sortCombo_);

    ui_->table->setColumnCount(9);
    ui_->table->setHorizontalHeaderLabels({"ID", "Produit", "Quantite", "Prix U", "Total", "Date Cmd", "Date Liv", "Paiement", "Action"});
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
    ui_->computeBtn->setVisible(false);

    connect(ui_->addBtn, &QPushButton::clicked, this, &CommandesPage::onAdd);
    connect(ui_->updateBtn, &QPushButton::clicked, this, &CommandesPage::onUpdate);
    connect(ui_->deleteBtn, &QPushButton::clicked, this, &CommandesPage::onDelete);
    connect(ui_->clearBtn, &QPushButton::clicked, this, &CommandesPage::onExportPdf);
    connect(ui_->table, &QTableWidget::itemSelectionChanged, this, &CommandesPage::onSelectionChanged);
    connect(ui_->searchEdit, &QLineEdit::textChanged, this, &CommandesPage::onSearchChanged);
    connect(sortCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CommandesPage::onSortChanged);

    onComputeTotal();
    refreshTable();
}

CommandesPage::~CommandesPage()
{
    delete ui_;
}

void CommandesPage::onComputeTotal()
{
    const double total = ui_->quantiteSpin->value() * ui_->prixSpin->value();
    ui_->totalEdit->setText(QString::number(total, 'f', 2));
}

CommandeData CommandesPage::collectData() const
{
    CommandeData c;
    c.produit = ui_->produitEdit->text();
    c.quantite = ui_->quantiteSpin->value();
    c.prixUnitaire = ui_->prixSpin->value();
    c.total = ui_->totalEdit->text().toDouble();
    c.dateCommande = ui_->dateCommandeEdit->date();
    c.dateLivraison = ui_->dateLivraisonEdit->date();
    c.modePaiement = ui_->modePaiementCombo->currentText();
    return c;
}

void CommandesPage::onAdd()
{
    if (selectedId_ > 0) {
        onUpdate();
        return;
    }

    onComputeTotal();
    CommandeData c = collectData();
    QString err;
    if (!crud_.addCommande(c, err)) {
        QMessageBox::critical(this, "Ajout commande", err);
        return;
    }
    clearForm();
    refreshTable();
}

void CommandesPage::onUpdate()
{
    if (selectedId_ <= 0) {
        QMessageBox::information(this, "Modification", "Selectionnez une commande.");
        return;
    }

    onComputeTotal();
    CommandeData c = collectData();
    c.id = selectedId_;
    QString err;
    if (!crud_.updateCommande(c, err)) {
        QMessageBox::critical(this, "Modification commande", err);
        return;
    }
    clearForm();
    refreshTable();
}

void CommandesPage::onDelete()
{
    if (selectedId_ <= 0) {
        QMessageBox::information(this, "Suppression", "Selectionnez une commande.");
        return;
    }

    QString err;
    if (!crud_.deleteCommande(selectedId_, err)) {
        QMessageBox::critical(this, "Suppression commande", err);
        return;
    }
    clearForm();
    refreshTable();
}

void CommandesPage::onSelectionChanged()
{
    const int row = ui_->table->currentRow();
    if (row < 0) return;

    selectedId_ = ui_->table->item(row, 0)->text().toInt();
    ui_->produitEdit->setText(ui_->table->item(row, 1)->text());
    ui_->quantiteSpin->setValue(ui_->table->item(row, 2)->text().toInt());
    ui_->prixSpin->setValue(ui_->table->item(row, 3)->text().toDouble());
    ui_->totalEdit->setText(ui_->table->item(row, 4)->text());
    ui_->dateCommandeEdit->setDate(QDate::fromString(ui_->table->item(row, 5)->text(), Qt::ISODate));
    ui_->dateLivraisonEdit->setDate(QDate::fromString(ui_->table->item(row, 6)->text(), Qt::ISODate));

    const int idx = ui_->modePaiementCombo->findText(ui_->table->item(row, 7)->text());
    if (idx >= 0) ui_->modePaiementCombo->setCurrentIndex(idx);
}

void CommandesPage::onSearchChanged()
{
    refreshTable();
}

void CommandesPage::onSortChanged()
{
    refreshTable();
}

void CommandesPage::clearForm()
{
    selectedId_ = -1;
    ui_->produitEdit->clear();
    ui_->quantiteSpin->setValue(1);
    ui_->prixSpin->setValue(0.01);
    ui_->dateCommandeEdit->setDate(QDate::currentDate());
    ui_->dateLivraisonEdit->setDate(QDate::currentDate().addDays(2));
    ui_->modePaiementCombo->setCurrentIndex(0);
    onComputeTotal();
    ui_->table->clearSelection();
}

void CommandesPage::refreshTable()
{
    const QString sortKey = sortCombo_->currentData().toString();
    const QList<CommandeData> list = crud_.searchCommandes(ui_->searchEdit->text(), sortKey);

    ui_->table->setRowCount(list.size());
    for (int i = 0; i < list.size(); ++i) {
        const CommandeData &c = list[i];
        ui_->table->setItem(i, 0, new QTableWidgetItem(QString::number(c.id)));
        ui_->table->setItem(i, 1, new QTableWidgetItem(c.produit));
        ui_->table->setItem(i, 2, new QTableWidgetItem(QString::number(c.quantite)));
        ui_->table->setItem(i, 3, new QTableWidgetItem(QString::number(c.prixUnitaire, 'f', 2)));
        ui_->table->setItem(i, 4, new QTableWidgetItem(QString::number(c.total, 'f', 2)));
        ui_->table->setItem(i, 5, new QTableWidgetItem(c.dateCommande.toString(Qt::ISODate)));
        ui_->table->setItem(i, 6, new QTableWidgetItem(c.dateLivraison.toString(Qt::ISODate)));
        ui_->table->setItem(i, 7, new QTableWidgetItem(c.modePaiement));

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

        ui_->table->setCellWidget(i, 8, actions);
    }

    refreshChart();
}

void CommandesPage::refreshChart()
{
    const QList<QPair<QString, int>> tops = crud_.topProduits(5);

    auto *set = new QBarSet("Qte vendue");
    QStringList labels;
    for (const auto &p : tops) {
        labels << p.first;
        *set << p.second;
    }

    auto *series = new QBarSeries(this);
    series->append(set);

    auto *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Top produits commandes");
    chart->createDefaultAxes();

    auto *axis = new QBarCategoryAxis();
    axis->append(labels);
    chart->addAxis(axis, Qt::AlignBottom);
    series->attachAxis(axis);

    chartView_->setChart(chart);
}

void CommandesPage::onExportPdf()
{
    if (ui_->table->rowCount() == 0) {
        QMessageBox::warning(this, "Export PDF", "Aucune donnee a exporter.");
        return;
    }
    const QString fileName = QFileDialog::getSaveFileName(this, "Exporter PDF", "commandes.pdf", "PDF (*.pdf)");
    if (fileName.isEmpty()) return;

    QPdfWriter writer(fileName);
    writer.setPageSize(QPageSize::A4);
    QPainter p(&writer);
    p.setFont(QFont("Arial", 10));
    int y = 120;
    for (int c = 0; c < ui_->table->columnCount() - 1; ++c) {
        p.drawText(35 + c * 75, y, ui_->table->horizontalHeaderItem(c)->text());
    }
    y += 25;
    for (int r = 0; r < ui_->table->rowCount(); ++r) {
        if (y > writer.height() - 60) {
            writer.newPage();
            y = 80;
        }
        for (int c = 0; c < ui_->table->columnCount() - 1; ++c) {
            auto *item = ui_->table->item(r, c);
            p.drawText(35 + c * 75, y, item ? item->text() : "");
        }
        y += 20;
    }
    p.end();
}
