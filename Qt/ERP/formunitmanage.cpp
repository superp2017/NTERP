#include "formunitmanage.h"
#include "ui_formunitmanage.h"
#include <QStringList>
#include "datacenter.h"
#include "dialognewunit.h"
#include "unitservice.h"
#include "boost/thread.hpp"

FormUnitManage::FormUnitManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormUnitManage)
{
    ui->setupUi(this);
    ui->listWidget->setEditTriggers(QListView::NoEditTriggers);
    ui->pushButton_del->setEnabled(false);

    connect(ui->listWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(itemClicked(QListWidgetItem*)));
    connect(dataCenter::instance(),SIGNAL(sig_delUnit(QString,bool)),this,SLOT(delUnitCb(QString,bool)));
    initItem();
}

FormUnitManage::~FormUnitManage()
{
    delete ui;
}

void FormUnitManage::itemClicked(QListWidgetItem *item)
{
    ui->pushButton_del->setEnabled(true);
}

void FormUnitManage::initItem()
{
    ui->listWidget->addItems(dataCenter::instance()->Units().toList());
}



void FormUnitManage::on_pushButton_exit_clicked()
{
    this->hide();
}

void FormUnitManage::on_pushButton_del_clicked()
{
    if(ui->listWidget->currentItem()!=NULL){
        boost::thread t(boost::bind(&dataCenter::delUnit,dataCenter::instance(),UnitService::toJsonObject(ui->listWidget->currentItem()->text())));
        t.detach();
        dataCenter::instance()->showLoadding("正在网络请求...",5000,Qt::black);
    }
}

void FormUnitManage::on_pushButton_add_clicked()
{
    ui->listWidget->clearSelection();
    ui->pushButton_del->setEnabled(false);
    DialogNewUnit unit;
    if(unit.exec()==123){
        ui->listWidget->addItem(unit.getUnit());
    }
}

void FormUnitManage::delUnitCb(QString unit, bool ok)
{
    dataCenter::instance()->hideLoadding();
    if(ok){
        if(ui->listWidget->currentItem()!=NULL){
//            ui->listWidget->removeItemWidget(ui->listWidget->currentItem());
            ui->listWidget->takeItem(ui->listWidget->currentRow());
            dataCenter::instance()->showMessage("删除成功!",4000);
        }
    }else{
        dataCenter::instance()->showMessage("删除失败!",4000);
    }
}
