#include "clientspage.h"

#include <QChart>
#include <QChartView>
#include <QComboBox>
#include <QFileDialog>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QIcon>
#include <QLineEdit>
#include <QMessageBox>
#include <QPainter>
#include <QPdfWriter>
#include <QPieSeries>
#include <QPushButton>
#include <QTableWidget>
#include <QToolButton>
#include <QVBoxLayout>

#include "ui_clientspageform.h"

ClientsPage::ClientsPage(QWidget *parent)
    : BaseGestionPage("Gestion des Clients", "CRUD + recherche + tri + statistiques par adresse", parent),
      ui_(new Ui::ClientsPageForm),
      selectedId_(-1),
      chartView_(new QChartView(new QChart(), this))
{
    auto *pageRoot = new QWidget(this);
    ui_->setupUi(pageRoot);
    contentLayout_->addWidget(pageRoot);

    ui_->statutCombo->addItems({"Actif", "Inactif"});
    ui_->adresseCombo->addItems({
        "Tunis", "Ariana", "Ben Arous", "Manouba", "Nabeul", "Zaghouan", "Bizerte", "Beja",
        "Jendouba", "Le Kef", "Siliana", "Sousse", "Monastir", "Mahdia", "Sfax", "Kairouan",
        "Kasserine", "Sidi Bouzid", "Gabes", "Medenine", "Tataouine", "Gafsa", "Tozeur", "Kebili"
    });
    ui_->dateEdit->setPlaceholderText("YYYY-MM-DD");
    ui_->searchEdit->setPlaceholderText("Rechercher client...");
    ui_->sortCombo->addItems({"Sans tri", "Par nom", "Par date"});

    ui_->table->setColumnCount(9);
    ui_->table->setHorizontalHeaderLabels({"ID", "Nom", "Prenom", "Tel", "Mail", "Adresse", "Date", "Statut", "Action"});
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
    ui_->updateBtn->setVisible(false);
    ui_->deleteBtn->setVisible(false);
    ui_->clearBtn->setText("Exporter PDF");
    ui_->clearBtn->setStyleSheet("background:#3f74af; color:white; border:1px solid #345f8f; border-radius:11px; font-weight:800;");
    ui_->clearBtn->setVisible(true);

    connect(ui_->addBtn, &QPushButton::clicked, this, &ClientsPage::onAdd);
    connect(ui_->updateBtn, &QPushButton::clicked, this, &ClientsPage::onUpdate);
    connect(ui_->deleteBtn, &QPushButton::clicked, this, &ClientsPage::onDelete);
    connect(ui_->clearBtn, &QPushButton::clicked, this, &ClientsPage::onExportPdf);
    connect(ui_->searchEdit, &QLineEdit::textChanged, this, &ClientsPage::onFilterChanged);
    connect(ui_->sortCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ClientsPage::onFilterChanged);
    connect(ui_->table, &QTableWidget::itemSelectionChanged, this, &ClientsPage::onSelectionChanged);

    refreshTable();
}

ClientsPage::~ClientsPage()
{
    delete ui_;
}

ClientData ClientsPage::collectData() const
{
    ClientData c;
    c.nom = ui_->nomEdit->text();
    c.prenom = ui_->prenomEdit->text();
    c.tel = ui_->telEdit->text();
    c.mail = ui_->mailEdit->text();
    c.adresse = ui_->adresseCombo->currentText();
    c.dateInscription = ui_->dateEdit->text();
    c.statut = ui_->statutCombo->currentText();
    return c;
}

void ClientsPage::onAdd()
{
    if (selectedId_ > 0) {
        onUpdate();
        return;
    }

    ClientData c = collectData();
    QString err;
    if (!crud_.addClient(c, err)) {
        QMessageBox::critical(this, "Ajout client", err);
        return;
    }
    clearForm();
    refreshTable();
}

void ClientsPage::onUpdate()
{
    if (selectedId_ <= 0) {
        QMessageBox::information(this, "Modification", "Selectionnez un client.");
        return;
    }

    ClientData c = collectData();
    c.id = selectedId_;
    QString err;
    if (!crud_.updateClient(c, err)) {
        QMessageBox::critical(this, "Modification client", err);
        return;
    }
    clearForm();
    refreshTable();
}

void ClientsPage::onDelete()
{
    if (selectedId_ <= 0) {
        QMessageBox::information(this, "Suppression", "Selectionnez un client.");
        return;
    }

    QString err;
    if (!crud_.deleteClient(selectedId_, err)) {
        QMessageBox::critical(this, "Suppression client", err);
        return;
    }
    clearForm();
    refreshTable();
}

void ClientsPage::onFilterChanged()
{
    refreshTable();
}

void ClientsPage::onSelectionChanged()
{
    const int row = ui_->table->currentRow();
    if (row < 0) return;

    selectedId_ = ui_->table->item(row, 0)->text().toInt();
    ui_->nomEdit->setText(ui_->table->item(row, 1)->text());
    ui_->prenomEdit->setText(ui_->table->item(row, 2)->text());
    ui_->telEdit->setText(ui_->table->item(row, 3)->text());
    ui_->mailEdit->setText(ui_->table->item(row, 4)->text());
    ui_->adresseCombo->setCurrentText(ui_->table->item(row, 5)->text());
    ui_->dateEdit->setText(ui_->table->item(row, 6)->text());

    const int s = ui_->statutCombo->findText(ui_->table->item(row, 7)->text());
    if (s >= 0) ui_->statutCombo->setCurrentIndex(s);
}

void ClientsPage::clearForm()
{
    selectedId_ = -1;
    ui_->nomEdit->clear();
    ui_->prenomEdit->clear();
    ui_->telEdit->clear();
    ui_->mailEdit->clear();
    ui_->adresseCombo->setCurrentIndex(0);
    ui_->dateEdit->clear();
    ui_->statutCombo->setCurrentIndex(0);
    ui_->table->clearSelection();
}

void ClientsPage::refreshTable()
{
    const QString sortKey = (ui_->sortCombo->currentIndex() == 1) ? "nom"
                        : (ui_->sortCombo->currentIndex() == 2) ? "date"
                        : QString();
    const QList<ClientData> data = crud_.searchClients(ui_->searchEdit->text(), sortKey);

    ui_->table->setRowCount(data.size());
    for (int i = 0; i < data.size(); ++i) {
        const ClientData &c = data[i];
        ui_->table->setItem(i, 0, new QTableWidgetItem(QString::number(c.id)));
        ui_->table->setItem(i, 1, new QTableWidgetItem(c.nom));
        ui_->table->setItem(i, 2, new QTableWidgetItem(c.prenom));
        ui_->table->setItem(i, 3, new QTableWidgetItem(c.tel));
        ui_->table->setItem(i, 4, new QTableWidgetItem(c.mail));
        ui_->table->setItem(i, 5, new QTableWidgetItem(c.adresse));
        ui_->table->setItem(i, 6, new QTableWidgetItem(c.dateInscription));
        ui_->table->setItem(i, 7, new QTableWidgetItem(c.statut));

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

    updateChart();
}

void ClientsPage::updateChart()
{
    auto *series = new QPieSeries(this);
    const QList<QPair<QString, int>> stats = crud_.clientsByAddress();

    for (const auto &entry : stats) {
        const QString label = entry.first.isEmpty() ? "Sans adresse" : entry.first;
        series->append(label, entry.second);
    }

    auto *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Repartition des clients par adresse");

    chartView_->setChart(chart);
}

void ClientsPage::onExportPdf()
{
    if (ui_->table->rowCount() == 0) {
        QMessageBox::warning(this, "Export PDF", "Aucune donnee a exporter.");
        return;
    }

    const QString fileName = QFileDialog::getSaveFileName(this, "Exporter PDF", "clients.pdf", "PDF (*.pdf)");
    if (fileName.isEmpty()) return;

    QPdfWriter writer(fileName);
    writer.setPageSize(QPageSize::A4);
    QPainter p(&writer);
    p.setFont(QFont("Arial", 10));

    int y = 120;
    for (int c = 0; c < ui_->table->columnCount() - 1; ++c) {
        p.drawText(40 + c * 80, y, ui_->table->horizontalHeaderItem(c)->text());
    }
    y += 25;
    for (int r = 0; r < ui_->table->rowCount(); ++r) {
        if (y > writer.height() - 60) {
            writer.newPage();
            y = 80;
        }
        for (int c = 0; c < ui_->table->columnCount() - 1; ++c) {
            auto *item = ui_->table->item(r, c);
            p.drawText(40 + c * 80, y, item ? item->text() : "");
        }
        y += 20;
    }
    p.end();
}
