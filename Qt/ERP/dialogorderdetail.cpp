#include "dialogorderdetail.h"
#include "ui_dialogorderdetail.h"


DialogOrderDetail::DialogOrderDetail(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogOrderDetail)
{
    ui->setupUi(this);
}

DialogOrderDetail::~DialogOrderDetail()
{
    delete ui;
}

void DialogOrderDetail::init(Order *order)
{
    if(order==NULL){
        ui->label->setText("");
        return;
    }
    ui->label->setText(order->OrderID);
}

void DialogOrderDetail::on_pushButton_ok_clicked()
{
    this->done(123);
}

void DialogOrderDetail::on_pushButton_cancel_clicked()
{
    this->done(-1);
}
