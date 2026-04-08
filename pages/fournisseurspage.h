#ifndef FOURNISSEURSPAGE_H
#define FOURNISSEURSPAGE_H

#include "basegestionpage.h"
#include "../models/fournisseurcrud.h"

namespace Ui {
class FournisseursPageForm;
}

class QChartView;
class QComboBox;

class FournisseursPage : public BaseGestionPage
{
    Q_OBJECT

public:
    explicit FournisseursPage(QWidget *parent = nullptr);
    ~FournisseursPage();

private slots:
    void onAdd();
    void onUpdate();
    void onDelete();
    void onExportPdf();
    void onSelectionChanged();
    void onSearchChanged();
    void onSortChanged();

private:
    FournisseurData collectData() const;
    void clearForm();
    void refreshTable();
    void refreshChart();

    Ui::FournisseursPageForm *ui_;
    FournisseurCrud crud_;
    int selectedId_;
    QChartView *chartView_;
    QComboBox *sortCombo_;
};

#endif
