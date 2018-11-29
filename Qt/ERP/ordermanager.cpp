#include "ordermanager.h"
#include "ui_ordermanager.h"
#include "datacenter.h"
#include "orderservice.h"
#include <QDebug>
#include <QMessageBox>
#include "boost/thread.hpp"
#include "dialogmodprice.h"
#include "dialogorderprint.h"
#include<QDateTime>
#include "dialogorderproduceorout.h"

#include "dialogprintouttable.h"

OrderManager::OrderManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OrderManager)
{
    ui->setupUi(this);

    m_tab_new     = new OrderTable(Status_New);
    m_tab_success = new OrderTable(Status_Success);
    m_tab_produce = new OrderTable(Status_Produce);
    m_tab_all     = new OrderTable(Status_All);

    ui->tabWidget->addTab(m_tab_new,"新订单");
    ui->tabWidget->addTab(m_tab_produce,"已成品");
    ui->tabWidget->addTab(m_tab_success,"已出库");
    ui->tabWidget->addTab(m_tab_all,"全部");
    ui->tabWidget->tabBar()->setMovable(true);
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
    connect(dataCenter::instance(),SIGNAL(sig_produceOrder(Order,bool)),this,SLOT(produceOrderCb(Order,bool)));
    connect(dataCenter::instance(),SIGNAL(sig_finishOrder(Order,bool)),this,SLOT(finishOrderCb(Order,bool)));
    connect(dataCenter::instance(),SIGNAL(sig_delOrder(Order,bool)),this,SLOT(delOrderCb(Order,bool)));

    connect(dataCenter::instance(),SIGNAL(sig_globalOrders(bool)),this,SLOT(GlobalOrdersCb(bool)));


    connect(m_tab_new,SIGNAL(newOrder()),this,SLOT(on_pushButton_new_clicked()));
    connect(m_tab_new,SIGNAL(modOrder()),this,SLOT(on_pushButton_mod_clicked()));
    connect(m_tab_new,SIGNAL(cancleOrder()),this,SLOT(on_pushButton_cancle_clicked()));
    connect(m_tab_new,SIGNAL(modPrice()),this,SLOT(on_pushButton_change_price_clicked()));
    connect(m_tab_new,SIGNAL(produceOrder()),this,SLOT(on_pushButton_produce_clicked()));


    connect(m_tab_produce,SIGNAL(newOrder()),this,SLOT(on_pushButton_new_clicked()));
    connect(m_tab_produce,SIGNAL(produceOrder()),this,SLOT(on_pushButton_produce_clicked()));
    connect(m_tab_produce,SIGNAL(outOrder()),this,SLOT(on_pushButton_success_clicked()));

    connect(m_tab_produce,SIGNAL(modPrice()),this,SLOT(on_pushButton_change_price_clicked()));

    connect(m_tab_success,SIGNAL(newOrder()),this,SLOT(on_pushButton_new_clicked()));
    connect(m_tab_success,SIGNAL(produceOrder()),this,SLOT(on_pushButton_produce_clicked()));
    connect(m_tab_success,SIGNAL(outOrder()),this,SLOT(on_pushButton_success_clicked()));


        connect(m_tab_success,SIGNAL(modPrice()),this,SLOT(on_pushButton_change_price_clicked()));

    connect(m_tab_all,SIGNAL(newOrder()),this,SLOT(on_pushButton_new_clicked()));
    connect(m_tab_all,SIGNAL(modOrder()),this,SLOT(on_pushButton_mod_clicked()));
    connect(m_tab_all,SIGNAL(cancleOrder()),this,SLOT(on_pushButton_cancle_clicked()));
    connect(m_tab_all,SIGNAL(modPrice()),this,SLOT(on_pushButton_change_price_clicked()));
    connect(m_tab_all,SIGNAL(produceOrder()),this,SLOT(on_pushButton_produce_clicked()));
    connect(m_tab_all,SIGNAL(outOrder()),this,SLOT(on_pushButton_success_clicked()));
    connect(m_tab_all,SIGNAL(delOrder()),this,SLOT(on_pushButton_del_clicked()));




    ui->pushButton_new->setStyleSheet("QPushButton{border-image: url(:/icon/new-red.png);}"
                                      "QPushButton:hover{border-image: url(:/icon/new.png);}"
                                      "QPushButton:pressed{border-image: url(:/icon/new.png);}"
                                      "QPushButton:checked{border-image: url(:/icon/new.png);}");
    ui->pushButton_out_print->setStyleSheet("QPushButton{border-image: url(:/icon/outprint.png);}"
                                            "QPushButton:hover{border-image: url(:/icon/outprint-a.png);}"
                                            "QPushButton:pressed{border-image: url(:/icon/outprint-a.png);}"
                                            "QPushButton:checked{border-image: url(:/icon/outprint-a.png);}");

    ui->pushButton_print->setStyleSheet("QPushButton{border-image: url(:/icon/export.png);}"
                                        "QPushButton:hover{border-image: url(:/icon/export-a.png);}"
                                        "QPushButton:pressed{border-image: url(:/icon/export-a.png);}"
                                        "QPushButton:checked{border-image: url(:/icon/export-a.png);}");
    ui->pushButton_reflash->setStyleSheet("QPushButton{border-image: url(:/icon/reflash.png);}"
                                          "QPushButton:hover{border-image: url(:/icon/reflash-a.png);}"
                                          "QPushButton:pressed{border-image: url(:/icon/reflash-a.png);}"
                                          "QPushButton:checked{border-image: url(:/icon/reflash-a.png);}");

    checkAuthor(dataCenter::instance()->pub_CurUser().Author);

    setBtnEnable(false,false,false,false,false,false);
    changeCol();
    updataData();
    connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(tabChange()));

}

OrderManager::~OrderManager()
{
    delete ui;
}


void OrderManager::tabChange()
{
    if(ui->tabWidget->currentWidget()== m_tab_new){
        ui->pushButton_new->setEnabled(true);
        ui->pushButton_new->setVisible(true);
    }else{
        ui->pushButton_new->setEnabled(false);
        ui->pushButton_new->setVisible(false);
    }
    ui->pushButton_change_price->setVisible(false);
}


void OrderManager::updataData()
{
    m_tab_new->initOrder(dataCenter::instance()->pub_StatusOrders(Status_New));
    m_tab_produce->initOrder(dataCenter::instance()->pub_StatusOrders(Status_Produce));
    m_tab_success->initOrder(dataCenter::instance()->pub_StatusOrders(Status_Success));
    m_tab_all->initOrder(dataCenter::instance()->pub_StatusOrders(Status_All));
    clearAllSelect();
}

void OrderManager::checkAuthor(int author)
{
    switch (author) {
    case 0:
        ui->pushButton_del->setEnabled(false);
        ui->pushButton_change_price->setEnabled(false);
        break;
    case 1:
        ui->pushButton_new->setEnabled(false);
        ui->pushButton_change_price->setEnabled(false);
        ui->pushButton_cancle->setEnabled(false);
        ui->pushButton_mod->setEnabled(false);
        ui->pushButton_out_print->setEnabled(false);
        ui->pushButton_success->setEnabled(false);
        ui->pushButton_del->setEnabled(false);
        ui->pushButton_produce->setEnabled(false);
        ui->widget_control->hide();
        break;
    case 2:
        ui->pushButton_new->setEnabled(false);
        ui->pushButton_cancle->setEnabled(false);
        ui->pushButton_mod->setEnabled(false);
        ui->pushButton_out_print->setEnabled(false);
        ui->pushButton_success->setEnabled(false);
        ui->pushButton_del->setEnabled(false);
        ui->pushButton_produce->setEnabled(false);

        break;
    default:
        break;
    }
}


void OrderManager::orderClick(QString orderID)
{
    bool exist =false;
    cur_order = dataCenter::instance()->pub_getOrder(orderID,exist);
    if(!exist||cur_order.OrderID==""){
        return;
    }

    //    if(cur_order.Current.Status == Status_Cancle){
    //        setBtnEnable(false,false,false,false,false,true);
    //    }else{
    bool produce = cur_order.Current.Status==Status_New||\
            cur_order.Current.Status==Status_PartProduce||\
            cur_order.Current.Status==Status_Part_Part;
    bool out = cur_order.Current.Status==Status_Produce||\
            cur_order.Current.Status==Status_PartSuccess||\
            cur_order.Current.Status==Status_PartProduce||\
            cur_order.Current.Status==Status_Part_Part;
    bool mod = cur_order.Current.Status==Status_New;
    bool cancel = cur_order.Current.Status== Status_Cancle;
    setBtnEnable(mod,mod,produce,out,true,cancel);
    //    }


    //    if(ui->tabWidget->currentWidget()==m_tab_all){
    //        if(cur_order.Current.Status == Status_Cancle){
    //            setBtnEnable(false,false,false,false,false,true);
    //        }else{
    //            bool produce = cur_order.Current.Status==Status_New||\
    //                    cur_order.Current.Status==Status_PartProduce||\
    //                    cur_order.Current.Status==Status_Part_Part;
    //            bool out = cur_order.Current.Status==Status_Produce||\
    //                    cur_order.Current.Status==Status_PartSuccess||\
    //                    cur_order.Current.Status==Status_PartProduce||\
    //                    cur_order.Current.Status==Status_Part_Part;
    //            bool mod = cur_order.Current.Status==Status_New;
    //            bool cancel = cur_order.Current.Status== Status_Cancle;
    //            setBtnEnable(mod,mod,produce,out,mod,cancel);
    //        }
    //    }

    //    if(ui->tabWidget->currentWidget()== m_tab_new){
    //        setBtnEnable(true,true,true,false,true,false);
    //    }

    //    if(ui->tabWidget->currentWidget()==m_tab_success){
    //        setBtnEnable(false,false,false,false,false,false);
    //    }
    //    if(ui->tabWidget->currentWidget()==m_tab_produce){
    //        setBtnEnable(false,false,false,true,false,false);
    //    }
}

void OrderManager::changeCol()
{
    QHeaderView::ResizeMode  tab_mode;
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

    clearAllSelect();
}



void OrderManager::new_order()
{
    AUTHOR_Equal(1);
    AUTHOR_Equal(2);
    DialogNewOrder    neworer;
    neworer.setModel(true);
    if(neworer.exec()==123){
        Order order = neworer.getCurorder();
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
    AUTHOR_Equal(1);
    AUTHOR_Equal(2);
    if(cur_order.OrderID==""){
        return;
    }
    DialogNewOrder    neworer;
    neworer.setModel(false);
    neworer.initOrder(cur_order);
    if(neworer.exec()==123){
        Order order = neworer.getCurorder();
        cur_order = order;
        m_tab_new->modOrder(order);
        m_tab_all->modOrder(order);
    }
}

void OrderManager::on_pushButton_cancle_clicked()
{
    AUTHOR_Equal(1);
    AUTHOR_Equal(2);
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
        QJsonObject obj;
        obj.insert("OrderID",cur_order.OrderID);
        boost::thread t(boost::bind(&dataCenter::net_cancleOrder,dataCenter::instance(),obj));
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


void OrderManager::on_pushButton_produce_clicked()
{
    AUTHOR_Equal(1);
    AUTHOR_Equal(2);
    if(cur_order.OrderID==""){
        return;
    }
    DialogOrderProduceOrOut produce;
    produce.initData(false,cur_order.OrderID,cur_order.OrderNum-cur_order.ProduceNum,cur_order.Unit);
    produce.exec();
}


void OrderManager::on_pushButton_success_clicked()
{
    AUTHOR_Equal(1);
    AUTHOR_Equal(2);
    if(cur_order.OrderID==""){
        return;
    }
    DialogOrderProduceOrOut produce;
    produce.initData(true,cur_order.OrderID,cur_order.ProduceNum-cur_order.SuccessNum,cur_order.Unit);
    produce.exec();
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
    AUTHOR_Limit(2);
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



void OrderManager::on_pushButton_del_clicked()
{
    AUTHOR_Limit(3);
    if(cur_order.OrderID==""){
        return;
    }
    QMessageBox msgBox;
    msgBox.setWindowTitle("警告");
    msgBox.setText("您即将删除订单"+cur_order.OrderID);
    msgBox.setInformativeText("是否继续操作?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Ok:
    {
        // Save was clicked
        QJsonObject obj;
        obj.insert("OrderID",cur_order.OrderID);
        boost::thread t(boost::bind(&dataCenter::net_delOrder,dataCenter::instance(),obj));
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
    if(ui->tabWidget->currentWidget()==m_tab_produce){
        status =Status_Produce;
    }
    if(ui->tabWidget->currentWidget()==m_tab_all){
        status =Status_All;
    }
    print.initData(status,"全部分厂");
    print.exec();
}

void OrderManager::on_pushButton_out_print_clicked()
{
    AUTHOR_Equal(1);
    AUTHOR_Equal(2);
    DialogPrintOutTable print;
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

void OrderManager::produceOrderCb(Order order, bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        dataCenter::instance()->pub_showMessage("生产完成!",4000);
        if(order.ProduceNum==order.OrderNum)
            m_tab_new->removeOrder(order);
        else
            m_tab_new->modOrder(order);
        m_tab_produce->modOrder(order);
        m_tab_all->modOrder(order);
    }else{
        dataCenter::instance()->pub_showMessage("操作失败!",4000);
    }
}

void OrderManager::finishOrderCb(Order order, bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        dataCenter::instance()->pub_showMessage("出库成功!",4000);
        if(order.SuccessNum==order.OrderNum)
            m_tab_produce->removeOrder(order);
        else
            m_tab_produce->modOrder(order);
        m_tab_success->modOrder(order);
        m_tab_all->modOrder(order);
    }else{
        dataCenter::instance()->pub_showMessage("出库失败!",4000);
    }
}

void OrderManager::delOrderCb(Order order, bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        dataCenter::instance()->pub_showMessage("删除成功!",4000);
        m_tab_all->removeOrder(order);
    }else{
        dataCenter::instance()->pub_showMessage("删除失败!",4000);
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
    m_tab_produce->clearSelection();
    checkSelect();
    setBtnEnable(false,false,false,false,false,false);
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

void OrderManager::setBtnEnable(bool mod, bool cancel, bool produce, bool out,  bool change, bool del, bool isnew)
{
    if(ui->pushButton_new->isEnabled()){
        ui->pushButton_new->setVisible(isnew);
    }else{
        ui->pushButton_new->setVisible(false);
    }

    if(!ui->pushButton_out_print->isEnabled())
        ui->pushButton_out_print->setVisible(false);

    if(!ui->pushButton_print->isEnabled()){
        ui->pushButton_print->setVisible(false);
    }

    if(ui->pushButton_mod->isEnabled()){
        ui->pushButton_mod->setVisible(mod);
        if(mod){
            ui->pushButton_mod->setStyleSheet("QPushButton{border-image: url(:/icon/modify-red.png);}"
                                              "QPushButton:hover{border-image: url(:/icon/modify.png);}"
                                              "QPushButton:pressed{border-image: url(:/icon/modify.png);}"
                                              "QPushButton:checked{border-image: url(:/icon/modify.png);}");
        }else{
            ui->pushButton_mod->setStyleSheet("QPushButton{border-image: url(:/icon/modify.png);}");
        }
    }else{
        ui->pushButton_mod->setVisible(false);
    }

    if(ui->pushButton_cancle->isEnabled()){
        ui->pushButton_cancle->setVisible(cancel);
        if(cancel){
            ui->pushButton_cancle->setStyleSheet("QPushButton{border-image: url(:/icon/cancel-red.png);}"
                                                 "QPushButton:hover{border-image: url(:/icon/cancel.png);}"
                                                 "QPushButton:pressed{border-image: url(:/icon/cancel.png);}"
                                                 "QPushButton:checked{border-image: url(:/icon/cancel.png);}");
        }else{
            ui->pushButton_cancle->setStyleSheet("QPushButton{border-image: url(:/icon/cancel.png);}");
        }
    }else{
        ui->pushButton_cancle->setVisible(false);
    }


    if(ui->pushButton_produce->isEnabled()){
        ui->pushButton_produce->setVisible(produce);
        if(produce){
            ui->pushButton_produce->setStyleSheet("QPushButton{border-image: url(:/icon/production-red.png);}"
                                                  "QPushButton:hover{border-image: url(:/icon/production.png);}"
                                                  "QPushButton:pressed{border-image: url(:/icon/production.png);}"
                                                  "QPushButton:checked{border-image: url(:/icon/production.png);}");
        }else{
            ui->pushButton_produce->setStyleSheet("QPushButton{border-image: url(:/icon/production.png);}");
        }
    }else{
        ui->pushButton_produce->setVisible(false);
    }

    if( ui->pushButton_success->isEnabled()){
        ui->pushButton_success->setVisible(out);
        if(out){
            ui->pushButton_success->setStyleSheet("QPushButton{border-image: url(:/icon/out-red.png);}"
                                                  "QPushButton:hover{border-image: url(:/icon/out.png);}"
                                                  "QPushButton:pressed{border-image: url(:/icon/out.png);}"
                                                  "QPushButton:checked{border-image: url(:/icon/out.png);}");
        } else{
            ui->pushButton_success->setStyleSheet("QPushButton{border-image: url(:/icon/out.png);}");
        }
    }else{
        ui->pushButton_success->setVisible(false);
    }
    if(ui->pushButton_change_price->isEnabled()){
        ui->pushButton_change_price->setVisible(change);
        if(change){
            ui->pushButton_change_price->setStyleSheet("QPushButton{border-image: url(:/icon/price-red.png);}"
                                                       "QPushButton:hover{border-image: url(:/icon/price.png);}"
                                                       "QPushButton:pressed{border-image: url(:/icon/price.png);}"
                                                       "QPushButton:checked{border-image: url(:/icon/price.png);}");
        }else{
            ui->pushButton_change_price->setStyleSheet("QPushButton{border-image: url(:/icon/price.png);}");
        }
    }else{
        ui->pushButton_change_price->setVisible(false);
    }
    if(ui->pushButton_del->isEnabled()){
        ui->pushButton_del->setVisible(del);

        if(del){
            ui->pushButton_del->setStyleSheet("QPushButton{border-image: url(:/icon/delete-red.png);}"
                                              "QPushButton:hover{border-image: url(:/icon/delete.png);}"
                                              "QPushButton:pressed{border-image: url(:/icon/delete.png);}"
                                              "QPushButton:checked{border-image: url(:/icon/delete.png);}");
        }else{
            ui->pushButton_del->setStyleSheet("QPushButton{border-image: url(:/icon/delete.png);}");
        }
    }else{
        ui->pushButton_del->setVisible(false);
    }
}

void OrderManager::checkSelect()
{
    m_tab_new->checkSelect();
    m_tab_success->checkSelect();
    m_tab_all->checkSelect();
}



void OrderManager::showAll()
{
    if(ui->tabWidget->currentWidget()==m_tab_all){
        m_tab_all->showAllRow();
    }

    if(ui->tabWidget->currentWidget()== m_tab_new){
        m_tab_new->showAllRow();
    }

    if(ui->tabWidget->currentWidget()==m_tab_success){
        m_tab_success->showAllRow();
    }
}




