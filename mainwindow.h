#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "models/usercrud.h"

namespace Ui {
class MainWindow;
}

class SidebarWidget;
class QStackedWidget;
class UsersPage;
class ClientsPage;
class CommandesPage;
class FournisseursPage;
class MateriauxPage;
class LoginPage;
class SettingsPage;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void setupLayout();
    void setupStyle();
    void onLoginSucceeded(const UserData &user);

    Ui::MainWindow *ui;
    QStackedWidget *stack_;
    SidebarWidget *sidebar_;
    LoginPage *loginPage_;

    UsersPage *usersPage_;
    ClientsPage *clientsPage_;
    CommandesPage *commandesPage_;
    FournisseursPage *fournisseursPage_;
    MateriauxPage *materiauxPage_;
    SettingsPage *settingsPage_;
};

#endif
