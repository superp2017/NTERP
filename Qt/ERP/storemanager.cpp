#include "storemanager.h"
#include "ui_storemanager.h"
#include "datacenter.h"
#include <QMessageBox>
#include "boost/thread.hpp"
#include "dialoggoodsprint.h"
#include "dialognewgoods.h"
#include  "dialogoutgoods.h"
StoreManager::StoreManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StoreManager)
{
    ui->setupUi(this);

    ui->tabWidget->addTab(&m_goods_Table,"商品库存");
    ui->tabWidget->addTab(&m_record_Table,"出库记录");
    ui->tabWidget->tabBar()->setMovable(true);
    ui->tabWidget->tabBar()->setFont(QFont("Times", 14, QFont::Bold));


    ui->pushButton_new->setStyleSheet("QPushButton{border-image: url(:/icon/new-red.png);}"
                                      "QPushButton:hover{border-image: url(:/icon/new.png);}"
                                      "QPushButton:pressed{border-image: url(:/icon/new.png);}"
                                      "QPushButton:checked{border-image: url(:/icon/new.png);}");

    ui->pushButton_export->setStyleSheet("QPushButton{border-image: url(:/icon/export.png);}"
                                         "QPushButton:hover{border-image: url(:/icon/export-a.png);}"
                                         "QPushButton:pressed{border-image: url(:/icon/export-a.png);}"
                                         "QPushButton:checked{border-image: url(:/icon/export-a.png);}");

    ui->pushButton_reflash->setStyleSheet("QPushButton{border-image: url(:/icon/reflash.png);}"
                                          "QPushButton:hover{border-image: url(:/icon/reflash-a.png);}"
                                          "QPushButton:pressed{border-image: url(:/icon/reflash-a.png);}"
                                          "QPushButton:checked{border-image: url(:/icon/reflash-a.png);}");
    connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(clearSelectSection()));


    connect(ui->radioButton_ave,SIGNAL(clicked(bool)),this,SLOT(changeCol()));
    connect(ui->radioButton_content,SIGNAL(clicked(bool)),this,SLOT(changeCol()));
    connect(ui->radioButton_manu,SIGNAL(clicked(bool)),this,SLOT(changeCol()));

    connect(dataCenter::instance(),SIGNAL(sig_globalGoods(bool)),this,SLOT(getGlobalGoodsCb(bool)));
    connect(dataCenter::instance(),SIGNAL(sig_delGoods(QString,bool)),this,SLOT(delGoodsCb(QString,bool)));

    connect(&m_goods_Table,SIGNAL(GoodsClick(QString)),this,SLOT(GoodsClick(QString)));

    connect(&m_goods_Table,SIGNAL(modGoods()),this,SLOT(on_pushButton_mod_clicked()));

    connect(&m_goods_Table,SIGNAL(delGoods()),this,SLOT(on_pushButton_del_clicked()));


    changeCol();
    setBtnEnable(false,false,false,false);
    initData();
}

StoreManager::~StoreManager()
{
    delete ui;
}

void StoreManager::initData()
{
    m_goods_Table.initGoods(dataCenter::instance()->pub_goods());
    m_goods_Table.checkSelect();
    m_record_Table.checkSelect();
}

void StoreManager::clearSelectSection()
{
    m_goods_Table.clearSelection();
    m_record_Table.clearSelection();
    setBtnEnable(false,false,false,false);
}



void StoreManager::on_pushButton_new_clicked()
{
    DialogNewGoods newGoods;
    newGoods.setModule(true);
    newGoods.initData();
    if(newGoods.exec()==123){
        Goods goods = newGoods.getCurGoods();
        m_goods_Table.appendGoods(goods);
    }
}

void StoreManager::on_pushButton_mod_clicked()
{
    if (cur_Goods.ID==""){
        return ;
    }

    DialogNewGoods newGoods;
    newGoods.setModule(false);
    newGoods.initData();
    newGoods.initGoods(cur_Goods);
    if(newGoods.exec()==123){
        Goods goods = newGoods.getCurGoods();
        m_goods_Table.modGoods(goods);
    }
}



void StoreManager::on_pushButton_in_store_clicked()
{
    inout.setModule(true);
    inout.initGoods(cur_Goods);
    if(inout.exec()==123){
        Goods goods=  inout.getCurgoods();
        cur_Goods = goods;
        m_goods_Table.modGoods(goods);
    }
}

void StoreManager::on_pushButton_out_store_clicked()
{
    //    inout.setModule(false);
    //    inout.initGoods(cur_Goods);
    //    if(inout.exec()==123){
    //        Goods goods=  inout.getCurgoods();
    //        m_goods_Table.modGoods(goods);
    //    }
    DialogOutGoods out;
    if(out.exec()==123){
        m_record_Table.appendRecord(out.getCur_record());
    }
}

void StoreManager::on_pushButton_del_clicked()
{
    if(cur_Goods.ID==""){
        return;
    }
    QMessageBox msgBox;
    msgBox.setWindowTitle("提示");
    msgBox.setText("您将删除商品:"+cur_Goods.Name);
    msgBox.setInformativeText("是否继续操作?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Ok:
    {
        boost::thread t(boost::bind(&dataCenter::net_delGoods,dataCenter::instance(),GoodsService::toJsonObject(cur_Goods)));
        t.detach();
        dataCenter::instance()->pub_showLoadding("正在网络请求...",5000,Qt::black);
        break;
    }
    case QMessageBox::Cancel:
        break;
    default:
        break;
    }
}


void StoreManager::on_pushButton_export_clicked()
{
    DialogGoodsPrint print;
    print.exec();
}

void StoreManager::on_pushButton_reflash_clicked()
{
    cur_Goods.ID = "";
    boost::thread t(boost::bind(&dataCenter::net_getglobalGoods,dataCenter::instance()));
    t.detach();
    dataCenter::instance()->pub_showLoadding("正在网络请求...",5000,Qt::black);
}

void StoreManager::GoodsClick(QString id)
{
    bool ok =false;
    cur_Goods = dataCenter::instance()->pub_getGoods(id,ok);
    if(ok&&cur_Goods.ID!=""){
        setBtnEnable(true,true,true,true);
    }
}



void StoreManager::delGoodsCb(QString ID, bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        m_goods_Table.removeGoods(ID);
        dataCenter::instance()->pub_showMessage("删除成功",3000);
    }else{
        dataCenter::instance()->pub_showMessage("删除失败",4000);
    }
}

void StoreManager::getGlobalGoodsCb(bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        initData();
        dataCenter::instance()->pub_showMessage("刷新成功",3000);
    }else{
        dataCenter::instance()->pub_showMessage("刷新成功",4000);
    }
}

void StoreManager::changeCol()
{
    if(ui->radioButton_ave->isChecked()){
        m_goods_Table.setHeaderColModel(QHeaderView::Stretch);
    }
    if(ui->radioButton_content->isChecked()){
        m_goods_Table.setHeaderColModel(QHeaderView::ResizeToContents);
    }
    if(ui->radioButton_manu->isChecked()){
        m_goods_Table.setHeaderColModel(QHeaderView::Interactive);
    }
    clearSelectSection();
}


void StoreManager::setBtnEnable(bool mod,bool in, bool out, bool del)
{
    mod = in = out = del = true;
    ui->pushButton_mod->setEnabled(mod);
    ui->pushButton_in_store->setEnabled(in);
    ui->pushButton_out_store->setEnabled(out);
    ui->pushButton_del->setEnabled(del);
    if(mod){
        ui->pushButton_mod->setStyleSheet("QPushButton{border-image: url(:/icon/modify-red.png);}"
                                          "QPushButton:hover{border-image: url(:/icon/modify.png);}"
                                          "QPushButton:pressed{border-image: url(:/icon/modify.png);}"
                                          "QPushButton:checked{border-image: url(:/icon/modify.png);}");
    }else{
        ui->pushButton_mod->setStyleSheet("QPushButton{border-image: url(:/icon/modify.png);}");
    }

    if(in){
        ui->pushButton_in_store->setStyleSheet("QPushButton{border-image: url(:/icon/instroe-red.png);}"
                                               "QPushButton:hover{border-image: url(:/icon/instroe.png);}"
                                               "QPushButton:pressed{border-image: url(:/icon/instroe.png);}"
                                               "QPushButton:checked{border-image: url(:/icon/instroe.png);}");
    }else{
        ui->pushButton_in_store->setStyleSheet("QPushButton{border-image: url(:/icon/modify.png);}");
    }

    if(out){
        ui->pushButton_out_store->setStyleSheet("QPushButton{border-image: url(:/icon/out-red.png);}"
                                                "QPushButton:hover{border-image: url(:/icon/out.png);}"
                                                "QPushButton:pressed{border-image: url(:/icon/out.png);}"
                                                "QPushButton:checked{border-image: url(:/icon/out.png);}");
    }else{
        ui->pushButton_out_store->setStyleSheet("QPushButton{border-image: url(:/icon/out.png);}");
    }

    if(del){
        ui->pushButton_del->setStyleSheet("QPushButton{border-image: url(:/icon/delete-red.png);}"
                                          "QPushButton:hover{border-image: url(:/icon/delete.png);}"
                                          "QPushButton:pressed{border-image: url(:/icon/delete.png);}"
                                          "QPushButton:checked{border-image: url(:/icon/delete.png);}");
    }else{
        ui->pushButton_del->setStyleSheet("QPushButton{border-image: url(:/icon/delete.png);}");
    }
}

