#ifndef MATERIAUXPAGE_H
#define MATERIAUXPAGE_H

#include "basegestionpage.h"
#include "../models/materiauxcrud.h"

namespace Ui {
class MateriauxPageForm;
}

class QChartView;

class MateriauxPage : public BaseGestionPage
{
    Q_OBJECT

public:
    explicit MateriauxPage(QWidget *parent = nullptr);
    ~MateriauxPage();

private slots:
    void onAdd();
    void onUpdate();
    void onDelete();
    void onSelectionChanged();
    void onFilterChanged();
    void onExportPdf();

private:
    MateriauData collectData() const;
    void clearForm();
    void refreshTable();
    void refreshCharts();

    Ui::MateriauxPageForm *ui_;
    MateriauxCrud crud_;
    int selectedId_;
    QChartView *pieChartView_;
    QChartView *barChartView_;
};

#endif
