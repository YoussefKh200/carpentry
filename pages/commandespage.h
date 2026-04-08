#ifndef COMMANDESPAGE_H
#define COMMANDESPAGE_H

#include "basegestionpage.h"
#include "../models/commandecrud.h"

namespace Ui {
class CommandesPageForm;
}

class QChartView;
class QComboBox;

class CommandesPage : public BaseGestionPage
{
    Q_OBJECT

public:
    explicit CommandesPage(QWidget *parent = nullptr);
    ~CommandesPage();

private slots:
    void onAdd();
    void onUpdate();
    void onDelete();
    void onExportPdf();
    void onSelectionChanged();
    void onSearchChanged();
    void onSortChanged();
    void onComputeTotal();

private:
    CommandeData collectData() const;
    void clearForm();
    void refreshTable();
    void refreshChart();

    Ui::CommandesPageForm *ui_;
    CommandeCrud crud_;
    int selectedId_;
    QChartView *chartView_;
    QComboBox *sortCombo_;
};

#endif
