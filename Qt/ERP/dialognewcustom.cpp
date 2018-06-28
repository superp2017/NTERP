#include "dialognewcustom.h"
#include "ui_dialognewcustom.h"
#include <QFileDialog>
#include "datacenter.h"
#include "customerservice.h"
#include "boost/thread.hpp"
#include <QToolTip>
#include <QIntValidator>

DialogNewCustom::DialogNewCustom(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewCustom)
{
    ui->setupUi(this);
    m_isNewMod = true;
    connect(dataCenter::instance(),SIGNAL(sig_newCustomer(Customer,bool)),this,SLOT(newCustomerCb(Customer,bool)));
    connect(dataCenter::instance(),SIGNAL(sig_modCustomer(Customer,bool)),this,SLOT(modCustomerCb(Customer,bool)));

    QRegExp rx("[0-9\.]+$");
    QRegExpValidator *validator = new QRegExpValidator(rx, this);
    ui->lineEdit_bank_number->setValidator(validator);
    ui->lineEdit_cert_num->setValidator(validator);
    ui->lineEdit_contact_cell->setValidator(validator);
}

DialogNewCustom::~DialogNewCustom()
{
    delete ui;
}

void DialogNewCustom::on_pushButton_ok_clicked()
{
    Customer  cus       = curCustom;
    cus.Addr            = ui->lineEdit_addr->text();
    cus.BankName        = ui->lineEdit_bank->text();
    cus.Bankbranch      = ui->lineEdit_bank_banch->text();
    cus.BankNumber      = ui->lineEdit_bank_number->text();
    cus.Certificates    = ui->lineEdit_cart->text();
    cus.CertificatesNum = ui->lineEdit_cert_num->text();
    cus.ContactName     = ui->lineEdit_contact->text();
    cus.ContactCell     = ui->lineEdit_contact_cell->text();
    cus.Name            = ui->lineEdit_customName->text();
    cus.Note            = ui->textEdit->toPlainText();
    if(cus.Name==""){
        QToolTip::showText(ui->lineEdit_customName->mapToGlobal(QPoint(100, 0)), "客户姓名不能为空!");
        return;
    }

    QJsonObject para = CustomerService::toJsonObject(cus);
    if(m_isNewMod){
        boost::thread t(boost::bind(&dataCenter::net_newCustomer,dataCenter::instance(),para));
        t.detach();
    }else{
        boost::thread t(boost::bind(&dataCenter::net_modCustomer,dataCenter::instance(),para));
        t.detach();
    }
    dataCenter::instance()->pub_showLoadding("正在网络请求...",5000,Qt::black);
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


void DialogNewCustom::modCustomerCb(Customer c, bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        dataCenter::instance()->pub_showMessage("修改成功!",4000);
        curCustom = c;
        done(123);
    }else{
        dataCenter::instance()->pub_showMessage("修改失败!",4000);
        curCustom = c;
        done(0);
    }
}

void DialogNewCustom::newCustomerCb(Customer c, bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        dataCenter::instance()->pub_showMessage("新增成功!",4000);
        curCustom = c;
        done(123);
    }else{
        dataCenter::instance()->pub_showMessage("增加失败!",4000);
        curCustom = c;
        done(0);
    }
}



Customer DialogNewCustom::getCurCustom() const
{
    return curCustom;
}

void DialogNewCustom::initCustomer(Customer cus)
{
    ui->lineEdit_addr->setText(cus.Addr);
    ui->lineEdit_bank->setText(cus.BankName);
    ui->lineEdit_bank_banch->setText(cus.Bankbranch);
    ui->lineEdit_bank_number->setText(cus.BankNumber);
    ui->lineEdit_cart->setText(cus.Certificates);
    ui->lineEdit_cert_num->setText(cus.CertificatesNum);
    ui->lineEdit_contact->setText(cus.ContactName);
    ui->lineEdit_contact_cell->setText(cus.ContactCell);
    ui->lineEdit_customName->setText(cus.Name);
    ui->textEdit->setText(cus.Note);
    curCustom = cus;
}

void DialogNewCustom::setMode(bool isNew)
{
    m_isNewMod = isNew;
    if(m_isNewMod)
    {
        this->setWindowTitle("新增客户");
        ui->pushButton_ok->setText("新增");
    }else{
        this->setWindowTitle("修改客户");
        ui->pushButton_ok->setText("修改");
    }
}


