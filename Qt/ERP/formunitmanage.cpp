#include "formunitmanage.h"
#include "ui_formunitmanage.h"
#include <QStringList>
#include "datacenter.h"
#include "unitservice.h"
#include "boost/thread.hpp"
#include <QToolTip>

FormUnitManage::FormUnitManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormUnitManage)
{
    ui->setupUi(this);
    ui->listWidget->setEditTriggers(QListView::NoEditTriggers);
    ui->pushButton_del->setEnabled(false);

    connect(ui->listWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(itemClicked(QListWidgetItem*)));
    connect(dataCenter::instance(),SIGNAL(sig_delUnit(QString,bool)),this,SLOT(delUnitCb(QString,bool)));
    connect(dataCenter::instance(),SIGNAL(sig_newUnit(QString,bool)),this,SLOT(newUnitCb(QString,bool)));
}

FormUnitManage::~FormUnitManage()
{
    delete ui;
}

void FormUnitManage::itemClicked(QListWidgetItem *item)
{
      curunit = ui->listWidget->currentItem()->text();
    ui->pushButton_del->setEnabled(true);
}

void FormUnitManage::initData()
{
    ui->listWidget->clear();
    ui->lineEdit_new->clear();
    ui->listWidget->addItems(dataCenter::instance()->pub_Units().toList());
}


void FormUnitManage::on_pushButton_exit_clicked()
{
    this->hide();
}

void FormUnitManage::on_pushButton_del_clicked()
{
    if(ui->listWidget->currentItem()!=NULL){
        boost::thread t(boost::bind(&dataCenter::net_delUnit,dataCenter::instance(),UnitService::toJsonObject(ui->listWidget->currentItem()->text())));
        t.detach();
        dataCenter::instance()->pub_showLoadding("正在网络请求...",5000,Qt::black);
    }
}

void FormUnitManage::on_pushButton_add_clicked()
{
    ui->listWidget->clearSelection();
    ui->pushButton_del->setEnabled(false);
    curunit = ui->lineEdit_new->text().trimmed();
    if(curunit.isEmpty()){
        QToolTip::showText(ui->lineEdit_new->mapToGlobal(QPoint(100, 0)), "单位不能为空!");
        return ;
    }
    if(dataCenter::instance()->pub_checkUnitExist(curunit)){
        QToolTip::showText(ui->lineEdit_new->mapToGlobal(QPoint(100, 0)), "此单位已经存在!");
        return ;
    }
    boost::thread (boost::bind(&dataCenter::net_newUnit,dataCenter::instance(),UnitService::toJsonObject(curunit))).detach();
    dataCenter::instance()->pub_showLoadding("正在网络请求...",5000,Qt::black);
}

void FormUnitManage::newUnitCb(QString unit, bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        dataCenter::instance()->pub_showMessage("添加成功!",3000);
        ui->listWidget->addItem(curunit);
        ui->lineEdit_new->clear();
    }else{
        dataCenter::instance()->pub_showMessage("添加失败!",3000);
    }
}



void FormUnitManage::delUnitCb(QString unit, bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        if(ui->listWidget->currentItem()!=NULL){
            //            ui->listWidget->removeItemWidget(ui->listWidget->currentItem());
            ui->listWidget->takeItem(ui->listWidget->currentRow());
            dataCenter::instance()->pub_showMessage("删除成功!",4000);
        }
    }else{
        dataCenter::instance()->pub_showMessage("删除失败!",4000);
    }
}

