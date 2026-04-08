#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QWidget>

#include "../models/usercrud.h"

namespace Ui {
class LoginPageForm;
}

class LoginPage : public QWidget
{
    Q_OBJECT

public:
    explicit LoginPage(QWidget *parent = nullptr);
    ~LoginPage();

signals:
    void loginSucceeded(const UserData &user);

private slots:
    void onLoginClicked();

private:
    Ui::LoginPageForm *ui_;
    UserCrud crud_;
};

#endif
