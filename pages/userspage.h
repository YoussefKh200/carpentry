#ifndef USERSPAGE_H
#define USERSPAGE_H

#include "basegestionpage.h"
#include "../models/usercrud.h"

namespace Ui {
class UsersPageForm;
}

class QChartView;

class UsersPage : public BaseGestionPage
{
    Q_OBJECT
public:
    explicit UsersPage(QWidget *parent = nullptr);
    ~UsersPage();

private slots:
    void onAddUser();
    void onUpdateUser();
    void onDeleteUser();
    void onExportPdf();
    void onSelectionChanged();
    void onFilterChanged();

private:
    UserData collectFormData() const;
    void clearForm();
    void refreshTable();
    void fillFormFromRow(int row);
    void updateChart(const QList<UserData> &users);

    Ui::UsersPageForm *ui_;
    UserCrud crud_;
    int selectedId_;
    QChartView *chartView_;
};

#endif
