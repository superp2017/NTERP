#include "dialognewcustom.h"
#include "ui_dialognewcustom.h"

DialogNewCustom::DialogNewCustom(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewCustom)
{
    ui->setupUi(this);
}

DialogNewCustom::~DialogNewCustom()
{
    delete ui;
}

void DialogNewCustom::on_pushButton_ok_clicked()
{
    QJsonObject  para;
    curCustom.Name = ui->lineEdit_customName->text();
    done(123);
}

void DialogNewCustom::on_pushButton_cancel_clicked()
{
    done(-1);
}

void DialogNewCustom::on_pushButton_select_cert_clicked()
{

}

void DialogNewCustom::on_pushButton_select_logo_clicked()
{

}

Customer DialogNewCustom::getCurCustom() const
{
    return curCustom;
}


