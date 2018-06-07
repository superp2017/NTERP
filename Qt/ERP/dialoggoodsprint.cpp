#include "dialoggoodsprint.h"
#include "ui_dialoggoodsprint.h"

DialogGoodsPrint::DialogGoodsPrint(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogGoodsPrint)
{
    ui->setupUi(this);
}

DialogGoodsPrint::~DialogGoodsPrint()
{
    delete ui;
}


void DialogGoodsPrint::initData(QVector<Goods> data)
{

}
