#include "formsuppliermanage.h"
#include "ui_formsuppliermanage.h"

FormSupplierManage::FormSupplierManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormSupplierManage)
{
    ui->setupUi(this);
}

FormSupplierManage::~FormSupplierManage()
{
    delete ui;
}
