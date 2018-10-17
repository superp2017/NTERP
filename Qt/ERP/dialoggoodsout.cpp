#include "dialoggoodsout.h"
#include "ui_dialoggoodsout.h"

DialogGoodsOut::DialogGoodsOut(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogGoodsOut)
{
    ui->setupUi(this);
}

DialogGoodsOut::~DialogGoodsOut()
{
    delete ui;
}

void DialogGoodsOut::on_pushButton_out_clicked()
{

}
