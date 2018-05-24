#ifndef DIALOGNEWCUSTOM_H
#define DIALOGNEWCUSTOM_H

#include <QDialog>
#include <QJsonObject>
#include "service_global.h"
#pragma execution_character_set("utf-8")


namespace Ui {
class DialogNewCustom;
}

class DialogNewCustom : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewCustom(QWidget *parent = 0);
    ~DialogNewCustom();

    Customer getCurCustom() const;

    void initCustomer(Customer cus);
    void setMode(bool isNew);
private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton_cancel_clicked();

    void on_pushButton_select_cert_clicked();

    void on_pushButton_select_logo_clicked();

    void modCustomerCb(Customer c,bool ok);
    void newCustomerCb(Customer c,bool ok);

private:
    Ui::DialogNewCustom *ui;
    Customer curCustom;
    bool m_isNewMod;

};

#endif // DIALOGNEWCUSTOM_H
