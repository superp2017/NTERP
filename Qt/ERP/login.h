#ifndef NLOGIN_H
#define NLOGIN_H

#include <QDialog>
#pragma execution_character_set("utf-8")

namespace Ui {
class NLogin;
}

class NLogin : public QDialog
{
    Q_OBJECT

public:
    explicit NLogin(QWidget *parent = 0);
    ~NLogin();

private:
    Ui::NLogin *ui;
    void do_login(QString acc,QString pwd);
private slots:
    void loginCb(bool ok);
    void login();
};

#endif // NLOGIN_H
