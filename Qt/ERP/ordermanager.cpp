#include "ordermanager.h"
#include "ui_ordermanager.h"
#include "datacenter.h"
#include "orderservice.h"
#include <QDebug>
#include <QMessageBox>
#include "boost/thread.hpp"
#include "dialogmodprice.h"
#include "dialogorderprint.h"

OrderManager::OrderManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OrderManager)
{
    ui->setupUi(this);
    neworer = NULL;

    tab_mode = QHeaderView::Stretch;

    m_tab_new     = new OrderTable();
    m_tab_success = new OrderTable();
    m_tab_all     = new OrderTable();
    m_tab_produce = new OrderTable();
    //QIcon(":/icon/all.ico")
    ui->tabWidget->addTab(m_tab_new,"新订单");
    ui->tabWidget->addTab(m_tab_produce,"生产中");
    ui->tabWidget->addTab(m_tab_success,"已完成");
    ui->tabWidget->addTab(m_tab_all,"全部");
    ui->tabWidget->tabBar()->setMovable(true);
    //  ui->tabWidget->setFont(QFont("Times", 16, QFont::Bold));
    ui->tabWidget->tabBar()->setFont(QFont("Times", 14, QFont::Bold));
    connect(ui->radioButton_ave,SIGNAL(clicked(bool)),this,SLOT(changeCol()));
    connect(ui->radioButton_content,SIGNAL(clicked(bool)),this,SLOT(changeCol()));
    connect(ui->radioButton_manu,SIGNAL(clicked(bool)),this,SLOT(changeCol()));

    connect(m_tab_new,SIGNAL(orderClick(QString)),this,SLOT(orderClick(QString)));
    connect(m_tab_all,SIGNAL(orderClick(QString)),this,SLOT(orderClick(QString)));
    connect(m_tab_produce,SIGNAL(orderClick(QString)),this,SLOT(orderClick(QString)));
    connect(m_tab_success,SIGNAL(orderClick(QString)),this,SLOT(orderClick(QString)));


    connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(clearAllSelect()));

    connect(dataCenter::instance(),SIGNAL(sig_cancleOrder(Order,bool)),this,SLOT(cancleOrderCb(Order,bool)));
    connect(dataCenter::instance(),SIGNAL(sig_finishOrder(Order,bool)),this,SLOT(finishOrderCb(Order,bool)));
    connect(dataCenter::instance(),SIGNAL(sig_produceOrder(Order,bool)),this,SLOT(produceOrderCb(Order,bool)));
    connect(dataCenter::instance(),SIGNAL(sig_globalOrders(bool)),this,SLOT(GlobalOrdersCb(bool)));


    connect(m_tab_new,SIGNAL(newOrder()),this,SLOT(on_pushButton_new_clicked()));
    connect(m_tab_new,SIGNAL(modOrder()),this,SLOT(on_pushButton_mod_clicked()));
    connect(m_tab_new,SIGNAL(cancleOrder()),this,SLOT(on_pushButton_cancle_clicked()));
    connect(m_tab_new,SIGNAL(modPrice()),this,SLOT(on_pushButton_change_price_clicked()));
    connect(m_tab_new,SIGNAL(produceOrder()),this,SLOT(on_pushButton_produce_clicked()));


    connect(m_tab_produce,SIGNAL(outOrder()),this,SLOT(on_pushButton_success_clicked()));

    connect(m_tab_all,SIGNAL(newOrder()),this,SLOT(on_pushButton_new_clicked()));
    connect(m_tab_all,SIGNAL(modOrder()),this,SLOT(on_pushButton_mod_clicked()));
    connect(m_tab_all,SIGNAL(cancleOrder()),this,SLOT(on_pushButton_cancle_clicked()));
    connect(m_tab_all,SIGNAL(modPrice()),this,SLOT(on_pushButton_change_price_clicked()));
    connect(m_tab_all,SIGNAL(produceOrder()),this,SLOT(on_pushButton_produce_clicked()));
    connect(m_tab_all,SIGNAL(outOrder()),this,SLOT(on_pushButton_success_clicked()));

    ui->pushButton_new->setStyleSheet("QPushButton{border-image: url(:/icon/new-red.png);}"
                                      "QPushButton:hover{border-image: url(:/icon/new.png);}"
                                      "QPushButton:pressed{border-image: url(:/icon/new.png);}"
                                      "QPushButton:checked{border-image: url(:/icon/new.png);}");

    ui->pushButton_print->setStyleSheet("QPushButton{border-image: url(:/icon/export.png);}"
                                        "QPushButton:hover{border-image: url(:/icon/export-a.png);}"
                                        "QPushButton:pressed{border-image: url(:/icon/export-a.png);}"
                                        "QPushButton:checked{border-image: url(:/icon/export-a.png);}");
    ui->pushButton_reflash->setStyleSheet("QPushButton{border-image: url(:/icon/reflash.png);}"
                                          "QPushButton:hover{border-image: url(:/icon/reflash-a.png);}"
                                          "QPushButton:pressed{border-image: url(:/icon/reflash-a.png);}"
                                          "QPushButton:checked{border-image: url(:/icon/reflash-a.png);}");


    setBtnEnable(false,false,false,false,false);
    ui->radioButton_ave->setChecked(true);
    updataData();
}

OrderManager::~OrderManager()
{
    delete ui;
}

void OrderManager::updataData()
{
    m_tab_new->initOrder(dataCenter::instance()->pub_StatusOrders(Status_New));
    m_tab_success->initOrder(dataCenter::instance()->pub_StatusOrders(Status_Success));
    m_tab_produce->initOrder(dataCenter::instance()->pub_StatusOrders(Status_Produce));
    m_tab_all->initOrder(dataCenter::instance()->pub_StatusOrders(Status_All));
}


void OrderManager::orderClick(QString orderID)
{
    bool exist =false;
    cur_order = dataCenter::instance()->pub_getOrder(orderID,exist);
    if(!exist){
        return;
    }

    if(ui->tabWidget->currentWidget()==m_tab_all){
        if(cur_order.Current.Status == Status_New){
            setBtnEnable(true,true,false,true,true);
        }
        if(cur_order.Current.Status == Status_Produce){
            setBtnEnable(false,false,true,false,false);
        }
        if(cur_order.Current.Status == Status_Success){
            setBtnEnable(false,false,false,false,false);
        }
        if(cur_order.Current.Status == Status_Cancle){
            setBtnEnable(false,false,false,false,false);
        }
    }

    if(ui->tabWidget->currentWidget()== m_tab_new){
        setBtnEnable(true,true,false,true,true);
    }
    if(ui->tabWidget->currentWidget()== m_tab_produce){
        setBtnEnable(false,false,true,false,false);
    }
    if(ui->tabWidget->currentWidget()==m_tab_success){
        setBtnEnable(false,false,false,false,false);
    }

}

void OrderManager::changeCol()
{
    if(ui->radioButton_ave->isChecked()){
        tab_mode = QHeaderView::Stretch;
    }
    if(ui->radioButton_content->isChecked()){
        tab_mode = QHeaderView::ResizeToContents;
    }
    if(ui->radioButton_manu->isChecked()){
        tab_mode = QHeaderView::Interactive;
    }
    m_tab_new->setHeaderColModel(tab_mode);
    m_tab_produce->setHeaderColModel(tab_mode);
    m_tab_success->setHeaderColModel(tab_mode);
    m_tab_all->setHeaderColModel(tab_mode);
}

void OrderManager::new_order()
{
    if(neworer==NULL){
        neworer = new DialogNewOrder();
    }else{
        neworer->initData();
    }
    neworer->clearUI();
    neworer->setModel(true);
    if(neworer->exec()==123){
        Order order = neworer->getCurorder();
        m_tab_new->appendOrder(order);
        m_tab_all->appendOrder(order);
    }
}

void OrderManager::on_pushButton_new_clicked()
{
    new_order();
}

void OrderManager::on_pushButton_mod_clicked()
{
    if(cur_order.OrderID==""){
        return;
    }
    if(neworer==NULL){
        neworer = new DialogNewOrder();
    }else{
        neworer->initData();
    }
    neworer->setModel(false);
    neworer->clearUI();
    neworer->initOrder(cur_order);
    if(neworer->exec()==123){
        Order order = neworer->getCurorder();
        m_tab_new->modOrder(order);
        m_tab_all->modOrder(order);
    }
}

void OrderManager::on_pushButton_cancle_clicked()
{
    if(cur_order.OrderID==""){
        return;
    }

    QMessageBox msgBox;
    msgBox.setWindowTitle("提示");
    msgBox.setText("您即将取消订单"+cur_order.OrderID);
    msgBox.setInformativeText("是否继续操作?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Ok:
    {
        // Save was clicked
        boost::thread t(boost::bind(&dataCenter::net_cancleOrder,dataCenter::instance(),OrderService::toJsonObject(cur_order)));
        t.detach();
        dataCenter::instance()->pub_showLoadding("正在网络请求...",5000,Qt::black);
        break;
    }
    case QMessageBox::Cancel:
        break;
    default:
        // should never be reached
        break;
    }
}

void OrderManager::on_pushButton_success_clicked()
{
    if(cur_order.OrderID==""){
        return;
    }
    QMessageBox msgBox;
    msgBox.setWindowTitle("提示");
    msgBox.setText("您准备完成订单"+cur_order.OrderID);
    msgBox.setInformativeText("是否继续操作?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Ok:
    {
        // Save was clicked
        boost::thread t(boost::bind(&dataCenter::net_finishOrder,dataCenter::instance(),OrderService::toJsonObject(cur_order)));
        t.detach();
        dataCenter::instance()->pub_showLoadding("正在网络请求...",5000,Qt::black);
        break;
    }
    case QMessageBox::Cancel:
        // Cancel was clicked
        break;
    default:
        // should never be reached
        break;
    }
}


void OrderManager::on_pushButton_produce_clicked()
{
    if(cur_order.OrderID==""){
        return;
    }
    QMessageBox msgBox;
    msgBox.setWindowTitle("提示");
    msgBox.setText("订单:"+cur_order.OrderID+"即将进入生产,");
    msgBox.setInformativeText("是否继续操作?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Ok:
    {
        // Save was clicked
        boost::thread t(boost::bind(&dataCenter::net_produceOrder,dataCenter::instance(),OrderService::toJsonObject(cur_order)));
        t.detach();
        dataCenter::instance()->pub_showLoadding("正在网络请求...",5000,Qt::black);
        break;
    }
    case QMessageBox::Cancel:
        // Cancel was clicked
        break;
    default:
        // should never be reached
        break;
    }
}



void OrderManager::on_pushButton_reflash_clicked()
{
    clearCurOrder();
    boost::thread t(boost::bind(&dataCenter::net_getglobalOrders,dataCenter::instance()));
    t.detach();
    dataCenter::instance()->pub_showLoadding("正在网络请求...",5000,Qt::black);
}

void OrderManager::GlobalOrdersCb(bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        dataCenter::instance()->pub_showMessage("刷新订单成功!",4000);
        updataData();
    }else{
        dataCenter::instance()->pub_showMessage("刷新订单失败!",4000);
    }
}

void OrderManager::on_pushButton_change_price_clicked()
{
    if(cur_order.OrderID==""){
        return;
    }
    DialogModPrice mod;
    mod.setWindowTitle("定价");
    mod.initData(cur_order);
    if(mod.exec()==123){
        Order order = mod.getCurOrder();
        m_tab_new->modOrder(order);
        m_tab_all->modOrder(order);
    }
}

void OrderManager::on_pushButton_print_clicked()
{
    DialogOrderPrint  print;
    QString status;
    if(ui->tabWidget->currentWidget()==m_tab_new){
        status = Status_New;
    }
    if(ui->tabWidget->currentWidget()==m_tab_success){
        status =Status_Success;
    }
    if(ui->tabWidget->currentWidget()==m_tab_all){
        status =Status_All;
    }
    print.initData(status);
    print.exec();
}

void OrderManager::cancleOrderCb(Order order, bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        dataCenter::instance()->pub_showMessage("取消成功!",4000);
        m_tab_new->removeOrder(order);
        m_tab_success->appendOrder(order);
        m_tab_all->modOrder(order);
    }else{
        dataCenter::instance()->pub_showMessage("取消失败!",4000);
    }
}

void OrderManager::finishOrderCb(Order order, bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        dataCenter::instance()->pub_showMessage("出库成功!",4000);
        m_tab_produce->removeOrder(order);
        m_tab_success->appendOrder(order);
        m_tab_all->modOrder(order);
    }else{
        dataCenter::instance()->pub_showMessage("出库失败!",4000);
    }
}

void OrderManager::produceOrderCb(Order order, bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        dataCenter::instance()->pub_showMessage("操作成功!",4000);
        m_tab_new->removeOrder(order);
        m_tab_produce->appendOrder(order);
        m_tab_all->modOrder(order);
    }else{
        dataCenter::instance()->pub_showMessage("操作失败!",4000);
    }
}


void OrderManager::mousePressEvent(QMouseEvent *e)
{
    e->ignore();
}


void OrderManager::clearAllSelect()
{
    m_tab_all->clearSelection();
    m_tab_new->clearSelection();
    m_tab_success->clearSelection();
    setBtnEnable(false,false,false,false,false);
    clearCurOrder();
}



void OrderManager::clearCurOrder()
{
    cur_order.OrderID ="";
    cur_order.UID="";
    cur_order.UserName="";
    cur_order.OrderID="";
    cur_order.OrderType="";
    cur_order.MaterielID="";
    cur_order.MaterielDes="";
    cur_order.Unit="";
    cur_order.CustomID="";
    cur_order.CustomName="";
    cur_order.CustomBatch="";
    cur_order.CustomNote="";
    cur_order.ProduceID="";
    cur_order.SuccessTime="";
    cur_order.CreatTime="";
    cur_order.Current.Status="";
    cur_order.Current.Action="";
    cur_order.Current.UserName="";
    cur_order.Current.OpreatTime="";
    cur_order.Flow.clear();
    cur_order.OrderNum=0;
    cur_order.Money=0;
}

void OrderManager::setBtnEnable(bool mod, bool cancel, bool out, bool produce, bool change)
{
    ui->pushButton_mod->setEnabled(mod);
    ui->pushButton_cancle->setEnabled(cancel);
    ui->pushButton_success->setEnabled(out);
    ui->pushButton_change_price->setEnabled(change);
    ui->pushButton_produce->setEnabled(produce);


    if(mod)
        ui->pushButton_mod->setStyleSheet("QPushButton{border-image: url(:/icon/modify-red.png);}"
                                          "QPushButton:hover{border-image: url(:/icon/modify.png);}"
                                          "QPushButton:pressed{border-image: url(:/icon/modify.png);}"
                                          "QPushButton:checked{border-image: url(:/icon/modify.png);}");
    else
        ui->pushButton_mod->setStyleSheet("QPushButton{border-image: url(:/icon/modify.png);}");

    if(cancel)
        ui->pushButton_cancle->setStyleSheet("QPushButton{border-image: url(:/icon/cancel-red.png);}"
                                             "QPushButton:hover{border-image: url(:/icon/cancel.png);}"
                                             "QPushButton:pressed{border-image: url(:/icon/cancel.png);}"
                                             "QPushButton:checked{border-image: url(:/icon/cancel.png);}");
    else
        ui->pushButton_cancle->setStyleSheet("QPushButton{border-image: url(:/icon/cancel.png);}");

    if(out)
        ui->pushButton_success->setStyleSheet("QPushButton{border-image: url(:/icon/out-red.png);}"
                                              "QPushButton:hover{border-image: url(:/icon/out.png);}"
                                              "QPushButton:pressed{border-image: url(:/icon/out.png);}"
                                              "QPushButton:checked{border-image: url(:/icon/out.png);}");
    else
        ui->pushButton_success->setStyleSheet("QPushButton{border-image: url(:/icon/out.png);}");

    if(change)
        ui->pushButton_change_price->setStyleSheet("QPushButton{border-image: url(:/icon/price-red.png);}"
                                                   "QPushButton:hover{border-image: url(:/icon/price.png);}"
                                                   "QPushButton:pressed{border-image: url(:/icon/price.png);}"
                                                   "QPushButton:checked{border-image: url(:/icon/price.png);}");
    else
        ui->pushButton_change_price->setStyleSheet("QPushButton{border-image: url(:/icon/price.png);}");

    if(produce)
        ui->pushButton_produce->setStyleSheet("QPushButton{border-image: url(:/icon/production-red.png);}"
                                              "QPushButton:hover{border-image: url(:/icon/production.png);}"
                                              "QPushButton:pressed{border-image: url(:/icon/production.png);}"
                                              "QPushButton:checked{border-image: url(:/icon/production.png);}");
    else
        ui->pushButton_produce->setStyleSheet("QPushButton{border-image: url(:/icon/production.png);}");
}



