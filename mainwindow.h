#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void setupLayout();
    void setupStyle();

    Ui::MainWindow *ui;
    QStackedWidget *stack_;
    SidebarWidget *sidebar_;

    UsersPage *usersPage_;
    ClientsPage *clientsPage_;
    CommandesPage *commandesPage_;
    FournisseursPage *fournisseursPage_;
    MateriauxPage *materiauxPage_;
};

#endif
