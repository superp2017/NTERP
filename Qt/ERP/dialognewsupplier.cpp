#include "dialognewsupplier.h"
#include "ui_dialognewsupplier.h"
#include <QFileDialog>
#include "datacenter.h"
#include "supplierservice.h"
#include "boost/thread.hpp"
#include <QToolTip>
#include <QRegExp>

DialogNewSupplier::DialogNewSupplier(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewSupplier)
{
    ui->setupUi(this);
    m_isNewMod = false;
    connect(dataCenter::instance(),SIGNAL(sig_newSupplier(Supplier,bool)),this,SLOT(newSupplierCb(Supplier,bool)));
    connect(dataCenter::instance(),SIGNAL(sig_modSUpplier(Supplier,bool)),this,SLOT(modSupplierCb(Supplier,bool)));
}

DialogNewSupplier::~DialogNewSupplier()
{
    delete ui;
}

void DialogNewSupplier::on_pushButton_cancel_clicked()
{
    done(-1);
}

void DialogNewSupplier::on_pushButton_ok_clicked()
{
    Supplier  cus = cursupplier;
    cus.Addr            = ui->lineEdit_addr->text();
    cus.BankName        = ui->lineEdit_bank->text();
    cus.Bankbranch      = ui->lineEdit_bank_banch->text();
    cus.BankNumber      = ui->lineEdit_bank_number->text();
    cus.CertificatesNum = ui->lineEdit_cert_num->text();
    cus.ContactName     = ui->lineEdit_contact->text();
    cus.ContactCell     = ui->lineEdit_contact_cell->text();
    cus.Name            = ui->lineEdit_customName->text();
    cus.Tel             = ui->lineEdit_tel->text();
    cus.Note            = ui->textEdit_note->toPlainText();  
    cus.Goods         = ui->lineEdit_goods->text();

    if(cus.Name==""){
        QToolTip::showText(ui->lineEdit_customName->mapToGlobal(QPoint(100, 0)), "供应商姓名不能为空!");
        return;
    }
    if(cus.Tel==""){
        QToolTip::showText(ui->lineEdit_tel->mapToGlobal(QPoint(100, 0)), "供应商联系方式不能为空!");
        return;
    }

    QJsonObject para = SupplierService::toJsonObject(cus);
    if(m_isNewMod){
        boost::thread t(boost::bind(&dataCenter::net_newSupplier,dataCenter::instance(),para));
        t.detach();
    }else{
        boost::thread t(boost::bind(&dataCenter::net_modSupplier,dataCenter::instance(),para));
        t.detach();
    }
    dataCenter::instance()->pub_showLoadding("正在网络请求...",5000,Qt::black);

}

void DialogNewSupplier::modSupplierCb(Supplier c, bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        dataCenter::instance()->pub_showMessage("修改成功!",4000);
        cursupplier = c;
        done(123);
    }else{
        dataCenter::instance()->pub_showMessage("修改失败!",4000);
        cursupplier = c;
        done(0);
    }
}

void DialogNewSupplier::newSupplierCb(Supplier c, bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        dataCenter::instance()->pub_showMessage("新增成功!",4000);
        cursupplier = c;
        done(123);
    }else{
        dataCenter::instance()->pub_showMessage("增加失败!",4000);
        cursupplier = c;
        done(0);
    }
}

Supplier DialogNewSupplier::getCursupplier() const
{
    return cursupplier;
}

void DialogNewSupplier::setMode(bool isNew)
{
    m_isNewMod = isNew;
    if(m_isNewMod)
    {
        this->setWindowTitle("新增供应商");
        ui->pushButton_ok->setText("新增");
    }else{
        this->setWindowTitle("修改供应商");
        ui->pushButton_ok->setText("修改");
    }
}

void DialogNewSupplier::initSupplier(Supplier sup)
{
    ui->lineEdit_addr->setText(sup.Addr);
    ui->lineEdit_bank->setText(sup.BankName);
    ui->lineEdit_bank_banch->setText(sup.Bankbranch);
    ui->lineEdit_bank_number->setText(sup.BankNumber);
    ui->lineEdit_cert_num->setText(sup.CertificatesNum);
    ui->lineEdit_contact->setText(sup.ContactName);
    ui->lineEdit_contact_cell->setText(sup.ContactCell);
    ui->lineEdit_customName->setText(sup.Name);
    ui->lineEdit_tel->setText(sup.Tel);
    ui->textEdit_note->setText(sup.Note);
    ui->lineEdit_goods->setText(sup.Goods);
    cursupplier = sup;
}
