#include "formunitmanage.h"
#include "ui_formunitmanage.h"

FormUnitManage::FormUnitManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormUnitManage)
{
    ui->setupUi(this);
}

FormUnitManage::~FormUnitManage()
{
    delete ui;
}
