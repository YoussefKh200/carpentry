#ifndef CLIENTSPAGE_H
#define CLIENTSPAGE_H

#include "basegestionpage.h"
#include "../models/clientcrud.h"

namespace Ui {
class ClientsPageForm;
}

class QChartView;

class ClientsPage : public BaseGestionPage
{
    Q_OBJECT

public:
    explicit ClientsPage(QWidget *parent = nullptr);
    ~ClientsPage();

private slots:
    void onAdd();
    void onUpdate();
    void onDelete();
    void onExportPdf();
    void onFilterChanged();
    void onSelectionChanged();

private:
    ClientData collectData() const;
    void clearForm();
    void refreshTable();
    void updateChart();

    Ui::ClientsPageForm *ui_;
    ClientCrud crud_;
    int selectedId_;
    QChartView *chartView_;
};

#endif
