#include "userspage.h"

#include <QBarCategoryAxis>
#include <QBarSeries>
#include <QBarSet>
#include <QChart>
#include <QChartView>
#include <QComboBox>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QIcon>
#include <QFileDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QPainter>
#include <QPdfWriter>
#include <QPushButton>
#include <QTableWidget>
#include <QToolButton>
#include <QVBoxLayout>

#include "../db/connection.h"
#include "ui_userspageform.h"

UsersPage::UsersPage(QWidget *parent)
    : BaseGestionPage("Gestion des Utilisateurs", "CRUD + recherche + tri + statistiques", parent),
      ui_(new Ui::UsersPageForm),
      selectedId_(-1),
      chartView_(nullptr)
{
    auto *pageRoot = new QWidget(this);
    ui_->setupUi(pageRoot);
    contentLayout_->addWidget(pageRoot);

    const bool connected = Connection::instance()->createConnect();

    ui_->roleCombo->addItems({
        "Responsable client",
        "Responsable materiaux",
        "Responsable achats",
        "Responsable commandes",
        "Ouvrier"
    });
    ui_->etatCombo->addItems({"Actif", "Inactif"});
    ui_->mdpEdit->setEchoMode(QLineEdit::Password);
    ui_->mdpEdit->setPlaceholderText("Laisser vide pour conserver le mot de passe actuel");
    ui_->searchEdit->setPlaceholderText("Rechercher nom, prenom, tel, mail...");
    ui_->sortCombo->addItems({"Sans tri", "Par nom", "Par prenom"});

    ui_->table->setColumnCount(9);
    ui_->table->setHorizontalHeaderLabels({"ID", "Nom", "Prenom", "Tel", "Mail", "Salaire", "Role", "Etat", "Action"});
    ui_->table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui_->table->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui_->table->setSelectionMode(QAbstractItemView::SingleSelection);
    ui_->table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    auto *activeSet = new QBarSet("Actifs");
    auto *inactiveSet = new QBarSet("Inactifs");
    *activeSet << 0;
    *inactiveSet << 0;
    auto *series = new QBarSeries(this);
    series->append(activeSet);
    series->append(inactiveSet);

    auto *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Etat des utilisateurs");
    chart->createDefaultAxes();
    auto *axisX = new QBarCategoryAxis();
    axisX->append(QStringList() << "Utilisateurs");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    chartView_ = new QChartView(chart, pageRoot);
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

    connect(ui_->addBtn, &QPushButton::clicked, this, &UsersPage::onAddUser);
    connect(ui_->updateBtn, &QPushButton::clicked, this, &UsersPage::onUpdateUser);
    connect(ui_->deleteBtn, &QPushButton::clicked, this, &UsersPage::onDeleteUser);
    connect(ui_->clearBtn, &QPushButton::clicked, this, &UsersPage::onExportPdf);
    connect(ui_->table, &QTableWidget::itemSelectionChanged, this, &UsersPage::onSelectionChanged);
    connect(ui_->searchEdit, &QLineEdit::textChanged, this, &UsersPage::onFilterChanged);
    connect(ui_->sortCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &UsersPage::onFilterChanged);

    if (!connected) {
        QMessageBox::warning(this, "Connexion BDD", "Connexion Oracle echouee. Verifiez ODBC PROJET / user projet1.");
    }

    refreshTable();
}

UsersPage::~UsersPage()
{
    delete ui_;
}

void UsersPage::onAddUser()
{
    if (selectedId_ > 0) {
        onUpdateUser();
        return;
    }

    UserData user = collectFormData();
    QString error;
    if (!crud_.addUser(user, error)) {
        QMessageBox::critical(this, "Ajout impossible", error);
        return;
    }

    clearForm();
    refreshTable();
}

void UsersPage::onUpdateUser()
{
    if (selectedId_ <= 0) {
        QMessageBox::information(this, "Selection", "Selectionnez un utilisateur a modifier.");
        return;
    }

    UserData user = collectFormData();
    user.id = selectedId_;

    QString error;
    if (!crud_.updateUser(user, error)) {
        QMessageBox::critical(this, "Modification impossible", error);
        return;
    }

    clearForm();
    refreshTable();
}

void UsersPage::onDeleteUser()
{
    if (selectedId_ <= 0) {
        QMessageBox::information(this, "Selection", "Selectionnez un utilisateur a supprimer.");
        return;
    }

    QString error;
    if (!crud_.deleteUser(selectedId_, error)) {
        QMessageBox::critical(this, "Suppression impossible", error);
        return;
    }

    clearForm();
    refreshTable();
}

void UsersPage::onExportPdf()
{
    if (ui_->table->rowCount() == 0) {
        QMessageBox::warning(this, "Export PDF", "Aucune donnee a exporter.");
        return;
    }

    const QString fileName = QFileDialog::getSaveFileName(this, "Exporter PDF", "users.pdf", "PDF (*.pdf)");
    if (fileName.isEmpty()) return;

    QPdfWriter writer(fileName);
    writer.setPageSize(QPageSize::A4);
    QPainter p(&writer);
    p.setFont(QFont("Arial", 10));

    int y = 120;
    for (int c = 0; c < ui_->table->columnCount() - 1; ++c) {
        p.drawText(40 + c * 95, y, ui_->table->horizontalHeaderItem(c)->text());
    }
    y += 25;

    for (int r = 0; r < ui_->table->rowCount(); ++r) {
        if (y > writer.height() - 60) {
            writer.newPage();
            y = 80;
        }
        for (int c = 0; c < ui_->table->columnCount() - 1; ++c) {
            auto *item = ui_->table->item(r, c);
            p.drawText(40 + c * 95, y, item ? item->text() : "");
        }
        y += 20;
    }
    p.end();
}

void UsersPage::onSelectionChanged()
{
    const int row = ui_->table->currentRow();
    if (row < 0) return;
    fillFormFromRow(row);
}

void UsersPage::onFilterChanged()
{
    refreshTable();
}

UserData UsersPage::collectFormData() const
{
    UserData u;
    u.nom = ui_->nomEdit->text();
    u.prenom = ui_->prenomEdit->text();
    u.tel = ui_->telEdit->text();
    u.mail = ui_->mailEdit->text();
    u.mdp = ui_->mdpEdit->text();
    u.salaire = ui_->salaireEdit->text();
    u.role = ui_->roleCombo->currentText();
    u.etat = ui_->etatCombo->currentText();
    return u;
}

void UsersPage::clearForm()
{
    selectedId_ = -1;
    ui_->nomEdit->clear();
    ui_->prenomEdit->clear();
    ui_->telEdit->clear();
    ui_->mailEdit->clear();
    ui_->mdpEdit->clear();
    ui_->salaireEdit->clear();
    ui_->roleCombo->setCurrentIndex(0);
    ui_->etatCombo->setCurrentIndex(0);
    ui_->table->clearSelection();
}

void UsersPage::refreshTable()
{
    const QString sortKey = (ui_->sortCombo->currentIndex() == 1) ? "nom"
                        : (ui_->sortCombo->currentIndex() == 2) ? "prenom"
                        : QString();

    const QList<UserData> users = crud_.searchUsers(ui_->searchEdit->text(), sortKey);

    ui_->table->setRowCount(users.size());
    for (int row = 0; row < users.size(); ++row) {
        const UserData &u = users[row];
        ui_->table->setItem(row, 0, new QTableWidgetItem(QString::number(u.id)));
        ui_->table->setItem(row, 1, new QTableWidgetItem(u.nom));
        ui_->table->setItem(row, 2, new QTableWidgetItem(u.prenom));
        ui_->table->setItem(row, 3, new QTableWidgetItem(u.tel));
        ui_->table->setItem(row, 4, new QTableWidgetItem(u.mail));
        ui_->table->setItem(row, 5, new QTableWidgetItem(u.salaire));
        ui_->table->setItem(row, 6, new QTableWidgetItem(u.role));
        ui_->table->setItem(row, 7, new QTableWidgetItem(u.etat));

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

        connect(modifyBtn, &QToolButton::clicked, this, [this, row]() {
            ui_->table->selectRow(row);
            onSelectionChanged();
        });
        connect(deleteBtn, &QToolButton::clicked, this, [this, row]() {
            ui_->table->selectRow(row);
            onSelectionChanged();
            onDeleteUser();
        });

        ui_->table->setCellWidget(row, 8, actions);
    }

    updateChart(users);
}

void UsersPage::fillFormFromRow(int row)
{
    selectedId_ = ui_->table->item(row, 0)->text().toInt();
    ui_->nomEdit->setText(ui_->table->item(row, 1)->text());
    ui_->prenomEdit->setText(ui_->table->item(row, 2)->text());
    ui_->telEdit->setText(ui_->table->item(row, 3)->text());
    ui_->mailEdit->setText(ui_->table->item(row, 4)->text());
    ui_->salaireEdit->setText(ui_->table->item(row, 5)->text());
    ui_->mdpEdit->clear();

    const int roleIndex = ui_->roleCombo->findText(ui_->table->item(row, 6)->text());
    if (roleIndex >= 0) ui_->roleCombo->setCurrentIndex(roleIndex);

    const int etatIndex = ui_->etatCombo->findText(ui_->table->item(row, 7)->text());
    if (etatIndex >= 0) ui_->etatCombo->setCurrentIndex(etatIndex);
}

void UsersPage::updateChart(const QList<UserData> &users)
{
    int active = 0;
    int inactive = 0;
    for (const UserData &u : users) {
        const QString e = u.etat.trimmed().toLower();
        if (e == "actif" || e == "a") {
            ++active;
        } else {
            ++inactive;
        }
    }

    QChart *chart = chartView_->chart();
    if (!chart || chart->series().isEmpty()) return;

    auto *series = qobject_cast<QBarSeries *>(chart->series().first());
    if (!series || series->barSets().size() < 2) return;

    *series->barSets().at(0) << 0;
    *series->barSets().at(1) << 0;
    series->barSets().at(0)->remove(0);
    series->barSets().at(1)->remove(0);
    *series->barSets().at(0) << active;
    *series->barSets().at(1) << inactive;
}
