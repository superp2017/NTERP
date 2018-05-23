#include "dialogcustommanage.h"
#include "ui_dialogcustommanage.h"

DialogCustomManage::DialogCustomManage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCustomManage)
{
    ui->setupUi(this);
}

DialogCustomManage::~DialogCustomManage()
{
    delete ui;
}
