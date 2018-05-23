#include "dialogsuppliermanage.h"
#include "ui_dialogsuppliermanage.h"

DialogSupplierManage::DialogSupplierManage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSupplierManage)
{
    ui->setupUi(this);
}

DialogSupplierManage::~DialogSupplierManage()
{
    delete ui;
}
