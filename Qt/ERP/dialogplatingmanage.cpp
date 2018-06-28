#include "dialogplatingmanage.h"
#include "ui_dialogplatingmanage.h"
#include <QStringList>
#include "datacenter.h"
#include "platingservice.h"
#include "boost/thread.hpp"
#include <QToolTip>
DialogPlatingManage::DialogPlatingManage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPlatingManage)
{
    ui->setupUi(this);
    ui->listWidget->setEditTriggers(QListView::NoEditTriggers);
    ui->pushButton_del->setEnabled(false);

    connect(ui->listWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(itemClicked(QListWidgetItem*)));
    connect(dataCenter::instance(),SIGNAL(sig_delPlating(QString,bool)),this,SLOT(delPlatingCb(QString,bool)));
    connect(dataCenter::instance(),SIGNAL(sig_newPlating(QString,bool)),this,SLOT(newPlatingCb(QString,bool)));
    initData();
}

DialogPlatingManage::~DialogPlatingManage()
{
    delete ui;
}

void DialogPlatingManage::initData()
{
    ui->listWidget->clear();
    ui->lineEdit_new->clear();
    ui->listWidget->addItems(dataCenter::instance()->pub_Platings().toList());

}

void DialogPlatingManage::on_pushButton_add_clicked()
{
    ui->listWidget->clearSelection();
    ui->pushButton_del->setEnabled(false);
    curPlating = ui->lineEdit_new->text().trimmed();
    if(curPlating.isEmpty()){
        QToolTip::showText(ui->lineEdit_new->mapToGlobal(QPoint(100, 0)), "镀种不能为空!");
        return ;
    }
    if(dataCenter::instance()->pub_checkUnitExist(curPlating)){
        QToolTip::showText(ui->lineEdit_new->mapToGlobal(QPoint(100, 0)), "此镀种已经存在!");
        return ;
    }
    boost::thread (boost::bind(&dataCenter::net_newPlating,dataCenter::instance(),PlatingService::toJsonObject(curPlating))).detach();
    dataCenter::instance()->pub_showLoadding("正在网络请求...",5000,Qt::black);

}

void DialogPlatingManage::on_pushButton_del_clicked()
{
    if(ui->listWidget->currentItem()!=NULL){
        boost::thread t(boost::bind(&dataCenter::net_delPlating,dataCenter::instance(),PlatingService::toJsonObject(ui->listWidget->currentItem()->text())));
        t.detach();
        dataCenter::instance()->pub_showLoadding("正在网络请求...",5000,Qt::black);
    }
}

void DialogPlatingManage::on_pushButton_exit_clicked()
{
    done(-1);
}

void DialogPlatingManage::delPlatingCb(QString unit, bool ok)
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

void DialogPlatingManage::newPlatingCb(QString unit, bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        dataCenter::instance()->pub_showMessage("添加成功!",3000);
        ui->listWidget->addItem(curPlating);
        ui->lineEdit_new->clear();
    }else{
        dataCenter::instance()->pub_showMessage("添加失败!",3000);
    }
}

void DialogPlatingManage::itemClicked(QListWidgetItem *item)
{
    curPlating = ui->listWidget->currentItem()->text();
    ui->pushButton_del->setEnabled(true);
}
