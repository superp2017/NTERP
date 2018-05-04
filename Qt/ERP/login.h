#ifndef NLOGIN_H
#define NLOGIN_H

#include <QDialog>

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

private slots:
    void login();
};

#endif // NLOGIN_H
