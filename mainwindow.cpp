#include "mainwindow.h"

#include <QHBoxLayout>
#include <QSettings>
#include <QStackedWidget>
#include <QVBoxLayout>

#include "widgets/sidebarwidget.h"
#include "pages/userspage.h"
#include "pages/clientspage.h"
#include "pages/commandespage.h"
#include "pages/fournisseurspage.h"
#include "pages/loginpage.h"
#include "pages/materiauxpage.h"
#include "pages/settingspage.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      stack_(new QStackedWidget(this)),
      sidebar_(new SidebarWidget(this)),
      loginPage_(new LoginPage(this)),
      usersPage_(new UsersPage(this)),
      clientsPage_(new ClientsPage(this)),
      commandesPage_(new CommandesPage(this)),
      fournisseursPage_(new FournisseursPage(this)),
      materiauxPage_(new MateriauxPage(this)),
      settingsPage_(new SettingsPage(this))
{
    ui->setupUi(this);
    QSettings settings;
    setWindowTitle(settings.value("app/window_title", "Smart Carpentry Management").toString());
    resize(1400, 900);

    setupLayout();
    setupStyle();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupLayout()
{
    auto *sidebarLayout = new QVBoxLayout(ui->sidebarHost);
    sidebarLayout->setContentsMargins(0, 0, 0, 0);
    sidebarLayout->setSpacing(0);
    sidebarLayout->addWidget(sidebar_);

    auto *contentLayout = new QVBoxLayout(ui->contentHost);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(0);
    contentLayout->addWidget(stack_);

    stack_->addWidget(loginPage_);
    stack_->addWidget(usersPage_);
    stack_->addWidget(clientsPage_);
    stack_->addWidget(commandesPage_);
    stack_->addWidget(fournisseursPage_);
    stack_->addWidget(materiauxPage_);
    stack_->addWidget(settingsPage_);

    ui->sidebarHost->hide();
    stack_->setCurrentIndex(0);

    connect(sidebar_, &SidebarWidget::navigateTo, stack_, [this](int index) {
        if (index >= 0) stack_->setCurrentIndex(index + 1);
    });
    connect(stack_, &QStackedWidget::currentChanged, this, [this](int index) {
        if (index > 0) sidebar_->setActiveIndex(index - 1);
    });
    connect(loginPage_, &LoginPage::loginSucceeded, this, &MainWindow::onLoginSucceeded);
    connect(sidebar_, &SidebarWidget::logoutRequested, this, [this]() {
        ui->sidebarHost->hide();
        stack_->setCurrentIndex(0);
    });
    connect(settingsPage_, &SettingsPage::windowTitleChanged, this, [this](const QString &title) {
        setWindowTitle(title);
    });
}

void MainWindow::onLoginSucceeded(const UserData &user)
{
    Q_UNUSED(user);
    ui->sidebarHost->show();
    QSettings settings;
    int defaultPage = settings.value("app/default_page", 0).toInt();
    if (defaultPage < 0) defaultPage = 0;
    if (defaultPage > 5) defaultPage = 5;
    stack_->setCurrentIndex(defaultPage + 1);
    sidebar_->setActiveIndex(defaultPage);
}

void MainWindow::setupStyle()
{
    setStyleSheet(
        "QMainWindow { background: #ffffff; }"
        "QWidget#centralwidget { background: #ffffff; }"
        "QWidget#contentHost { background: #ffffff; }"
        "QStackedWidget, QStackedWidget > QWidget { background: #ffffff; }"
        "QFrame#PageCard { background: #ffffff; border-radius: 16px; border: 1px solid #d9c8b2; }"
        "QGroupBox { background: #f8f2e9; border: 1px solid #d7c6af; border-radius: 14px; margin-top: 14px; font-weight: 700; color: #3b2b1c; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 12px; padding: 0 6px; }"
        "QLabel { color: #3d2f22; font-weight: 600; }"
        "QLabel#PageTitle { font-size: 28px; font-weight: 800; color: #2f2318; letter-spacing: 0.5px; }"
        "QLineEdit, QComboBox, QSpinBox, QDoubleSpinBox, QDateEdit {"
        "  background: #ffffff; border: 1px solid #cfbda5; border-radius: 10px; padding: 7px 10px; color: #3d2f22; min-height: 24px;"
        "}"
        "QLineEdit:focus, QComboBox:focus, QSpinBox:focus, QDoubleSpinBox:focus, QDateEdit:focus {"
        "  border: 1px solid #a35a2f; background: #fffdf9;"
        "}"
        "QComboBox QAbstractItemView { background:#ffffff; color:#000000; selection-background-color:#f2d9bd; }"
        "QPushButton {"
        "  background: qlineargradient(x1:0,y1:0,x2:0,y2:1, stop:0 #ba6b3f, stop:1 #9f542c);"
        "  color: #ffffff; border: 1px solid #8d4724; border-radius: 11px; padding: 8px 16px; font-weight: 700; min-height: 28px;"
        "}"
        "QPushButton:hover { background: #b46034; }"
        "QPushButton:pressed { background: #8f4825; }"
        "QPushButton#primaryAction {"
        "  background: qlineargradient(x1:0,y1:0,x2:0,y2:1, stop:0 #c67445, stop:1 #a85a31);"
        "  color: #ffffff; border: 1px solid #8f4a26; font-weight: 800;"
        "}"
        "QPushButton#primaryAction:hover { background: #b86638; }"
        "QPushButton#secondaryAction {"
        "  background: qlineargradient(x1:0,y1:0,x2:0,y2:1, stop:0 #5c8ec6, stop:1 #3f74af);"
        "  color: #ffffff; border: 1px solid #345f8f; font-weight: 800;"
        "}"
        "QPushButton#secondaryAction:hover { background: #4d80ba; }"
        "QToolButton { background: #f3ece1; border: 1px solid #d2bda0; border-radius: 8px; padding: 3px; }"
        "QToolButton:hover { background: #eadfcf; }"
        "QTableWidget {"
        "  background: #fffefc; alternate-background-color: #f8f2e9; gridline-color: #e5d7c6;"
        "  border: 1px solid #d4c3ad; border-radius: 12px; color: #000000;"
        "}"
        "QHeaderView::section {"
        "  background: #e8dccb; color: #000000; border: none; border-bottom: 1px solid #d1bea7; padding: 8px; font-weight: 700;"
        "}"
        "QTableWidget::item { color: #000000; }"
        "QTableWidget::item:selected { background: #f2d9bd; color: #000000; }"
        "QChartView { background: #fffdf9; border: 1px solid #d5c2a9; border-radius: 12px; }"
        "QScrollArea { border: none; background: transparent; }"
        "QScrollBar:vertical { background: #eadfce; width: 11px; margin: 0; border-radius: 5px; }"
        "QScrollBar::handle:vertical { background: #c8ae8d; min-height: 30px; border-radius: 5px; }"
        "QScrollBar::handle:vertical:hover { background: #b99971; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }"
    );
}
