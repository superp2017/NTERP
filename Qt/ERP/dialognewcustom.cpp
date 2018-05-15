#include "dialognewcustom.h"
#include "ui_dialognewcustom.h"
#include <QFileDialog>

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

void DialogNewCustom::on_pushButton_ok_clicked()
{
    QJsonObject  para;
    curCustom.Name = ui->lineEdit_customName->text();
    done(123);
}

void DialogNewCustom::on_pushButton_cancel_clicked()
{
    done(-1);
}

void DialogNewCustom::on_pushButton_select_cert_clicked()
{
   QString fileName = QFileDialog::getOpenFileName(this,
         tr("上传营业执照"), ".", tr("Image Files (*.png *.jpg *.bmp)"));
   ui->lineEdit_cart->setText(fileName);
}

void DialogNewCustom::on_pushButton_select_logo_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
          tr("上传公司logo"), ".", tr("Image Files (*.png *.jpg *.bmp)"));
    ui->lineEdit_logo->setText(fileName);
}

Customer DialogNewCustom::getCurCustom() const
{
    return curCustom;
}


