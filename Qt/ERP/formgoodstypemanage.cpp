#include "formgoodstypemanage.h"
#include "ui_formgoodstypemanage.h"
#include "datacenter.h"
#include "boost/thread.hpp"
#include <QToolTip>

FormGoodsTypeManage::FormGoodsTypeManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormGoodsTypeManage)
{
    ui->setupUi(this);

    ui->pushButton_del->setEnabled(false);
    ui->listWidget->setEditTriggers(QListView::NoEditTriggers);

    connect(dataCenter::instance(),SIGNAL(sig_newGoodsType(QString,bool)),this,SLOT(newTypeCb(QString,bool)));
    connect(dataCenter::instance(),SIGNAL(sig_delGoodsType(QString,bool)),this,SLOT(delTypeCb(QString,bool)));
    connect(ui->listWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(itemClicked(QListWidgetItem*)));


}


void FormGoodsTypeManage::initData()
{
    ui->listWidget->clear();
    ui->lineEdit_new->clear();
    ui->listWidget->addItems(dataCenter::instance()->pub_goodsType().toList());
}


FormGoodsTypeManage::~FormGoodsTypeManage()
{
    delete ui;
}


void FormGoodsTypeManage::on_pushButton_add_clicked()
{
    ui->listWidget->clearSelection();
    ui->pushButton_del->setEnabled(false);
    curType = ui->lineEdit_new->text().trimmed();
    if(curType.isEmpty()){
        QToolTip::showText(ui->lineEdit_new->mapToGlobal(QPoint(100, 0)), "分类不能为空!");
        return ;
    }
    if(dataCenter::instance()->pub_checkTypeExist(curType)){
        QToolTip::showText(ui->lineEdit_new->mapToGlobal(QPoint(100, 0)), "此分类已经存在!");
        return ;
    }
    QJsonObject obj;
    obj.insert("Type",curType);
    boost::thread(boost::bind(&dataCenter::net_newGoodsType,dataCenter::instance(),obj)).detach();
    dataCenter::instance()->pub_showLoadding("正在网络请求...",5000,Qt::black);
}

void FormGoodsTypeManage::on_pushButton_del_clicked()
{
    if(ui->listWidget->currentItem()!=NULL){
        QString type = ui->listWidget->currentItem()->text();
        QJsonObject obj;
        obj.insert("Type",type);
        boost::thread t(boost::bind(&dataCenter::net_delGoodsType,dataCenter::instance(),obj));
        t.detach();
        dataCenter::instance()->pub_showLoadding("正在网络请求...",5000,Qt::black);
    }
}

void FormGoodsTypeManage::on_pushButton_exit_clicked()
{
    this->hide();
}

void FormGoodsTypeManage::newTypeCb(QString type, bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        dataCenter::instance()->pub_showMessage("添加成功!",3000);
        ui->listWidget->addItem(curType);
        ui->lineEdit_new->clear();
    }else{
        dataCenter::instance()->pub_showMessage("添加失败!",3000);
    }
}

void FormGoodsTypeManage::delTypeCb(QString type, bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        dataCenter::instance()->pub_showMessage("删除成功!",3000);
        if(ui->listWidget->currentItem()!=NULL){
            ui->listWidget->takeItem(ui->listWidget->currentRow());
        }
    }else{
        dataCenter::instance()->pub_showMessage("删除失败!",3000);
    }
}

void FormGoodsTypeManage::itemClicked(QListWidgetItem *)
{
    curType = ui->listWidget->currentItem()->text();
    ui->pushButton_del->setEnabled(true);
}
