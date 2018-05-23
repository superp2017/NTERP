#include "dialogmaterialmanage.h"
#include "ui_dialogmaterialmanage.h"

DialogMaterialManage::DialogMaterialManage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMaterialManage)
{
    ui->setupUi(this);
}

DialogMaterialManage::~DialogMaterialManage()
{
    delete ui;
}
