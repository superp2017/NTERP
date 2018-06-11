#include "formdepartmentmanage.h"
#include "ui_formdepartmentmanage.h"
#include <QStringList>
#include "datacenter.h"
#include "userservice.h"
#include "boost/thread.hpp"
#include <QToolTip>
FormDepartmentManage::FormDepartmentManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormDepartmentManage)
{
    ui->setupUi(this);
    ui->listWidget->setEditTriggers(QListView::NoEditTriggers);
    ui->pushButton_del->setEnabled(false);

    connect(ui->listWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(itemClicked(QListWidgetItem*)));
    connect(dataCenter::instance(),SIGNAL(sig_newDepartment(QString,bool)),this,SLOT(newDepartment(QString,bool)));
    connect(dataCenter::instance(),SIGNAL(sig_delDepartment(QString,bool)),this,SLOT(delDepartment(QString,bool)));

}

FormDepartmentManage::~FormDepartmentManage()
{
    delete ui;
}

void FormDepartmentManage::initData()
{
    ui->listWidget->clear();
    ui->lineEdit_new->clear();
    ui->listWidget->addItems(dataCenter::instance()->pub_getDepartments().toList());

}

void FormDepartmentManage::on_pushButton_add_clicked()
{
    ui->listWidget->clearSelection();
    ui->pushButton_del->setEnabled(false);
    curDepartment = ui->lineEdit_new->text().trimmed();
    if(curDepartment.isEmpty()){
        QToolTip::showText(ui->lineEdit_new->mapToGlobal(QPoint(100, 0)), "单位不能为空!");
        return ;
    }
    if(dataCenter::instance()->pub_checkUnitExist(curDepartment)){
        QToolTip::showText(ui->lineEdit_new->mapToGlobal(QPoint(100, 0)), "此单位已经存在!");
        return ;
    }
    QJsonObject obj;
    obj.insert("Department",curDepartment);
    boost::thread (boost::bind(&dataCenter::net_newDepartment,dataCenter::instance(),obj)).detach();
    dataCenter::instance()->pub_showLoadding("正在网络请求...",5000,Qt::black);

}

void FormDepartmentManage::on_pushButton_del_clicked()
{
    if(ui->listWidget->currentItem()!=NULL){
        QString de =ui->listWidget->currentItem()->text();
        if(de.isEmpty()) return;
        QJsonObject obj;
        obj.insert("Department",de);
        boost::thread t(boost::bind(&dataCenter::net_delDepartment,dataCenter::instance(),obj));
        t.detach();
        dataCenter::instance()->pub_showLoadding("正在网络请求...",5000,Qt::black);
    }
}

void FormDepartmentManage::on_pushButton_exit_clicked()
{
    this->hide();
}

void FormDepartmentManage::newDepartment(QString de, bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        dataCenter::instance()->pub_showMessage("添加成功!",3000);
        ui->listWidget->addItem(curDepartment);
        ui->lineEdit_new->clear();
    }else{
        dataCenter::instance()->pub_showMessage("添加失败!",3000);
    }
}

void FormDepartmentManage::delDepartment(QString de, bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        if(ui->listWidget->currentItem()!=NULL){
            ui->listWidget->takeItem(ui->listWidget->currentRow());
            dataCenter::instance()->pub_showMessage("删除成功!",4000);
        }
    }else{
        dataCenter::instance()->pub_showMessage("删除失败!",4000);
    }
}

void FormDepartmentManage::itemClicked(QListWidgetItem *item)
{
    curDepartment = ui->listWidget->currentItem()->text();
    ui->pushButton_del->setEnabled(true);
}
