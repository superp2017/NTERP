#include "formwarehousemanage.h"
#include "ui_formwarehousemanage.h"

FormWarehouseManage::FormWarehouseManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormWarehouseManage)
{
    ui->setupUi(this);
}

FormWarehouseManage::~FormWarehouseManage()
{
    delete ui;
}
