#include "dialogunitmanage.h"
#include "ui_dialogunitmanage.h"

DialogUnitManage::DialogUnitManage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogUnitManage)
{
    ui->setupUi(this);
}

DialogUnitManage::~DialogUnitManage()
{
    delete ui;
}
