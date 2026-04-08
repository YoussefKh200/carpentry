#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include "basegestionpage.h"

namespace Ui {
class SettingsPageForm;
}

class SettingsPage : public BaseGestionPage
{
    Q_OBJECT

public:
    explicit SettingsPage(QWidget *parent = nullptr);
    ~SettingsPage();

signals:
    void windowTitleChanged(const QString &title);

private slots:
    void onSaveSettings();
    void onClearRememberedLogin();

private:
    void loadSettings();

    Ui::SettingsPageForm *ui_;
};

#endif
