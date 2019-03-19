#include "formcustommanage.h"
#include "ui_formcustommanage.h"
#include "datacenter.h"
#include <QMessageBox>
#include "boost/thread.hpp"
#include <QFileDialog>
#include "customerservice.h"
#include "dialognewcustom.h"

FormCustommanage::FormCustommanage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormCustommanage)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(10);

    //设置表头内容
    QStringList header;
    header<<tr("客户编号")<<tr("客户姓名")<<tr("公司地址")\
         <<tr("联系人")<<tr("联系电话")<<tr("开户行")<<tr("银行卡号")\
        <<tr("支行名称") <<tr("税号")<<tr("备注");
    ui->tableWidget->setHorizontalHeaderLabels(header);


    connect(this,SIGNAL(sig_exportCb(bool)),this,SLOT(exportCb(bool)));
    connect(ui->checkBox_check_all,SIGNAL(clicked(bool)),this,SLOT(checkAll()));
    connect(dataCenter::instance(),SIGNAL(sig_delCustomer(QString,bool)),this,SLOT(delCustomerCb(QString,bool)));


    ui->pushButton_del->setEnabled(false);
    ui->pushButton_mod->setEnabled(false);

    connect(ui->radioButton_ave,SIGNAL(clicked(bool)),this,SLOT(changeCol()));
    connect(ui->radioButton_content,SIGNAL(clicked(bool)),this,SLOT(changeCol()));
    connect(ui->radioButton_manu,SIGNAL(clicked(bool)),this,SLOT(changeCol()));

    changeCol();
}


FormCustommanage::~FormCustommanage()
{
    delete ui;
}

void FormCustommanage::closeAllStatus()
{
    ui->tableWidget->clearSelection();
}

void FormCustommanage::on_pushButton_new_clicked()
{
    DialogNewCustom cus(this);
    cus.setMode(true);
    if(cus.exec()==123){
        appendOne(cus.getCurCustom());
    }
}

void FormCustommanage::delCustomerCb(QString cu, bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        removeOne(cu);
        dataCenter::instance()->pub_showMessage("删除成功!",4000);
    }else{
        dataCenter::instance()->pub_showMessage("删除失败!",4000);
    }
}

void FormCustommanage::changeCol()
{
    if(ui->radioButton_ave->isChecked()){
        ui->tableWidget->setHeaderColModel(QHeaderView::Stretch);
    }
    if(ui->radioButton_content->isChecked()){
        ui->tableWidget->setHeaderColModel(QHeaderView::ResizeToContents);

    }
    if(ui->radioButton_manu->isChecked()){
        ui->tableWidget->setHeaderColModel(QHeaderView::Interactive);
    }
    closeAllStatus();
}




void FormCustommanage::on_pushButton_del_clicked()
{
    Customer c;
    bool ok = false;
    for(QCheckBox *bo:m_boxs){
        if(bo->isChecked()){
            c = dataCenter::instance()->pub_getCustomer(bo->text(),ok);
            break;
        }
    }
    if(!ok){
        return;
    }
    boost::thread t(boost::bind(&dataCenter::net_delCustomer,dataCenter::instance(),CustomerService::toJsonObject(c)));
    t.detach();
    dataCenter::instance()->pub_showLoadding("正在网络请求...",5000,Qt::black);

}

void FormCustommanage::on_pushButton_mod_clicked()
{
    Customer c;
    bool ok = false;
    for(QCheckBox *bo:m_boxs){
        if(bo->isChecked()){
            c = dataCenter::instance()->pub_getCustomer(bo->text(),ok);
            break;
        }
    }
    if(!ok){
        return;
    }

    DialogNewCustom cus(this);
    cus.initCustomer(c);
    cus.setMode(false);
    if(cus.exec()==123){
        modOne(cus.getCurCustom());
    }
}

void FormCustommanage::on_pushButton_exit_clicked()
{
    this->hide();
}


void FormCustommanage::on_pushButton_export_clicked()
{
    QVector<Customer> ls;
    int count = ui->tableWidget->rowCount();
    for(int i=0;i<count;++i){
        ui->tableWidget->cellWidget(i,0);
        QWidget *w = ui->tableWidget->cellWidget(i,0);
        if(w!=NULL){
            QCheckBox *box = reinterpret_cast<QCheckBox *>(w);
            if(box->isChecked()){
                bool ok=false;
                Customer m =  dataCenter::instance()->pub_getCustomer(box->text(),ok);
                if(ok){
                    ls.push_back(m);
                }
            }
        }
    }

    if(ls.size()==0){
        QMessageBox::information(this,"提示","请至少选择一个客户...");
        return;
    }

    QString filepath= QFileDialog::getSaveFileName(NULL,"Save orders",".","Microsoft Office 2007 (*.xlsx)");//获取保存路径
    if(!filepath.isEmpty()){
        boost::thread t(boost::bind(&FormCustommanage::doExport,this,ls,filepath));
        t.detach();
        dataCenter::instance()->pub_showLoadding("正在导出...",30000);
    }else{
        dataCenter::instance()->pub_showMessage("保存路径为空!",3000);
    }
}


void FormCustommanage::checkBox()
{
    bool check = true;
 //   bool check_one = false;
    int checkSize = 0;
    for(QCheckBox* ch:m_boxs){
        check    &= ch->isChecked();
      //  check_one|= ch->isChecked();
        if(ch->isChecked()){
            checkSize++;
        }
    }
    if(check){
        ui->checkBox_check_all->setCheckState(Qt::Checked);
    }else{
//        if(check_one)
//            ui->checkBox_check_all->setCheckState(Qt::PartiallyChecked);
//        else{
            ui->checkBox_check_all->setCheckState(Qt::Unchecked);
      //  }
    }

    ui->pushButton_del->setEnabled(checkSize==1);
    ui->pushButton_mod->setEnabled(checkSize==1);
}

void FormCustommanage::exportCb(bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        dataCenter::instance()->pub_showMessage("导出成功!",3000);
    }else{
        dataCenter::instance()->pub_showMessage("导出失败!",3000);
    }
}

void FormCustommanage::checkAll()
{
    for(QCheckBox* box:m_boxs){
        box->blockSignals(true);
        box->setChecked(ui->checkBox_check_all->isChecked());
        box->blockSignals(false);
    }
    checkBox();
}

void FormCustommanage::initData()
{
    m_boxs.clear();
    ui->checkBox_check_all->setChecked(false);
    ui->tableWidget->removeAllRow();
    closeAllStatus();
    QVector<Customer>ls = dataCenter::instance()->pub_Customers();
    for(Customer m:ls){
        appendOne(m);
    }
}


void FormCustommanage::removeOne(QString ma)
{
    int count = ui->tableWidget->rowCount();
    for(int i=0;i<count;++i){
        QWidget *w = ui->tableWidget->cellWidget(i,0);
        if(w!=NULL){
            QCheckBox *box = reinterpret_cast<QCheckBox *>(w);
            if(box->text()==ma){
                ui->tableWidget->removeRow(i);
                break;
            }
        }
    }
}

void FormCustommanage::appendOne(Customer ma)
{
    ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
    int row= ui->tableWidget->rowCount()-1;
    setRowData(ma,row);
}

void FormCustommanage::modOne(Customer ma)
{
    int count = ui->tableWidget->rowCount();
    for(int i=0;i<count;++i){
        QWidget *w = ui->tableWidget->cellWidget(i,0);
        if(w!=NULL){
            QCheckBox *box = reinterpret_cast<QCheckBox *>(w);
            if(box->text()==ma.CID){
                setRowData(ma,i);
                break;
            }
        }
    }
}

void FormCustommanage::doExport(QVector<Customer> ls, QString filepath)
{
    bool ok = CustomerService::exportCustomer(ls,filepath,true);
    emit sig_exportCb(ok);
}


void FormCustommanage::setRowData(Customer ma, int row)
{
    QCheckBox *box ;
    QWidget *w= ui->tableWidget->cellWidget(row,0);
    if(w ==NULL){
        box  = new QCheckBox(ma.CID);
        m_boxs.push_back(box);
        ui->tableWidget->setCellWidget(row,0,box);
        connect(box,SIGNAL(clicked(bool)),this,SLOT(checkBox()));
    }else{
        box = reinterpret_cast<QCheckBox *>(w);
    }
    if(box!=NULL){
        box->setText(ma.CID);
    }

    QTableWidgetItem *item1 = ui->tableWidget->item(row,1);
    QTableWidgetItem *item2 = ui->tableWidget->item(row,2);
    QTableWidgetItem *item3 = ui->tableWidget->item(row,3);
    QTableWidgetItem *item4 = ui->tableWidget->item(row,4);
    QTableWidgetItem *item5 = ui->tableWidget->item(row,5);
    QTableWidgetItem *item6 = ui->tableWidget->item(row,6);
    QTableWidgetItem *item7 = ui->tableWidget->item(row,7);
    QTableWidgetItem *item8 = ui->tableWidget->item(row,8);
    QTableWidgetItem *item9 = ui->tableWidget->item(row,9);

    if(item1==NULL){
        item1 = new QTableWidgetItem();
        ui->tableWidget->setItem(row,1,item1);
    }
    if(item2==NULL){
        item2 = new QTableWidgetItem();
        ui->tableWidget->setItem(row,2,item2);
    }
    if(item3==NULL){
        item3 = new QTableWidgetItem();
        ui->tableWidget->setItem(row,3,item3);
    }
    if(item4==NULL){
        item4 = new QTableWidgetItem();
        ui->tableWidget->setItem(row,4,item4);
    }
    if(item5==NULL){
        item5 = new QTableWidgetItem();
        ui->tableWidget->setItem(row,5,item5);
    }
    if(item6==NULL){
        item6 = new QTableWidgetItem();
        ui->tableWidget->setItem(row,6,item6);
    }
    if(item7==NULL){
        item7 = new QTableWidgetItem();
        ui->tableWidget->setItem(row,7,item7);
    }
    if(item8==NULL){
        item8 = new QTableWidgetItem();
        ui->tableWidget->setItem(row,8,item8);
    }
    if(item9==NULL){
        item9 = new QTableWidgetItem();
        ui->tableWidget->setItem(row,9,item9);
    }

    item1->setText(ma.Name);
    item2->setText(ma.Addr);
    item3->setText(ma.ContactName);
    item4->setText(ma.ContactCell);
    item5->setText(ma.BankName);
    item6->setText(ma.BankNumber);
    item7->setText(ma.Bankbranch);
    item8->setText(ma.CertificatesNum);
    item9->setText(ma.Note);

    item1->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    item2->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    item3->setTextAlignment(Qt::AlignCenter);
    item4->setTextAlignment(Qt::AlignCenter);
    item5->setTextAlignment(Qt::AlignCenter);
    item6->setTextAlignment(Qt::AlignCenter);
    item7->setTextAlignment(Qt::AlignCenter);
    item8->setTextAlignment(Qt::AlignCenter);
    item9->setTextAlignment(Qt::AlignCenter);

}



