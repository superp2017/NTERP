#include "formcustommanage.h"
#include "ui_formcustommanage.h"

FormCustommanage::FormCustommanage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormCustommanage)
{
    ui->setupUi(this);
}

FormCustommanage::~FormCustommanage()
{
    delete ui;
}

void FormCustommanage::on_pushButton_del_clicked()
{

}

void FormCustommanage::on_pushButton_mod_clicked()
{

}

void FormCustommanage::on_pushButton_exit_clicked()
{

}
