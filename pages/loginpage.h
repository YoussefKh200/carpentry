#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QWidget>

#include "../models/usercrud.h"
#include "../services/faceloginservice.h"

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
    void onFaceLoginClicked();

private:
    Ui::LoginPageForm *ui_;
    UserCrud crud_;
    FaceLoginService faceService_;
};

#endif
