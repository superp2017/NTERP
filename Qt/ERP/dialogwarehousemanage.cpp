#include "dialogwarehousemanage.h"
#include "ui_dialogwarehousemanage.h"

DialogWarehouseManage::DialogWarehouseManage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogWarehouseManage)
{
    ui->setupUi(this);
}

DialogWarehouseManage::~DialogWarehouseManage()
{
    delete ui;
}
