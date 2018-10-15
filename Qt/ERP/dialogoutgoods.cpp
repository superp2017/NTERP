#include "dialogoutgoods.h"
#include "ui_dialogoutgoods.h"

DialogOutGoods::DialogOutGoods(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogOutGoods)
{
    ui->setupUi(this);
}

DialogOutGoods::~DialogOutGoods()
{
    delete ui;
}

void DialogOutGoods::on_pushButton_ok_clicked()
{

}

void DialogOutGoods::on_pushButton_cancel_clicked()
{

}
