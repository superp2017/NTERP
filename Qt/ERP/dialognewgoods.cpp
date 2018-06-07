#include "dialognewgoods.h"
#include "ui_dialognewgoods.h"

DialogNewGoods::DialogNewGoods(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewGoods)
{
    ui->setupUi(this);
}

DialogNewGoods::~DialogNewGoods()
{
    delete ui;
}
