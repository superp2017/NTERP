#include "dialognewcustom.h"
#include "ui_dialognewcustom.h"

DialogNewCustom::DialogNewCustom(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewCustom)
{
    ui->setupUi(this);
}

DialogNewCustom::~DialogNewCustom()
{
    delete ui;
}
