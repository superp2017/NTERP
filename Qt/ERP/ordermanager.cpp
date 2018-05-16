#include "ordermanager.h"
#include "ui_ordermanager.h"
#include "datacenter.h"
#include "orderservice.h"
#include <QDebug>

OrderManager::OrderManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OrderManager)
{
    ui->setupUi(this);
   neworer = NULL;

    tab_mode = QHeaderView::Stretch;

    m_tab_new = new OrderTable();
    m_tab_success = new OrderTable();
    m_tab_all = new OrderTable();
    //QIcon(":/icon/all.ico")
    ui->tabWidget->addTab(m_tab_new,"新订单");
    ui->tabWidget->addTab(m_tab_success,"已完成");
    ui->tabWidget->addTab(m_tab_all,"全部");
    ui->tabWidget->tabBar()->setMovable(true);
    ui->tabWidget->setFont(QFont("Times", 16, QFont::Bold));
    connect(ui->radioButton_ave,SIGNAL(clicked(bool)),this,SLOT(changeCol()));
    connect(ui->radioButton_content,SIGNAL(clicked(bool)),this,SLOT(changeCol()));
    connect(ui->radioButton_manu,SIGNAL(clicked(bool)),this,SLOT(changeCol()));

    connect(m_tab_new,SIGNAL(orderClick(QString)),this,SLOT(orderClick(QString)));
    connect(m_tab_all,SIGNAL(orderClick(QString)),this,SLOT(orderClick(QString)));

    connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(clearAllSelect()));

    ui->radioButton_ave->setChecked(true);

    ui->pushButton_mod->setVisible(false);
    ui->pushButton_cancle->setVisible(false);
    ui->pushButton_success->setVisible(false);

}

OrderManager::~OrderManager()
{
    delete ui;
}


void OrderManager::orderClick(QString orderID)
{
    bool exist =false;
    cur_order = dataCenter::instance()->getOrder(orderID,exist);
    if(!exist){
        return;
    }

    if(ui->tabWidget->currentWidget()==m_tab_all){
        if(cur_order.Current.Status == Status_New){
            ui->pushButton_mod->setVisible(true);
            ui->pushButton_cancle->setVisible(true);
            ui->pushButton_success->setVisible(true);
        }
        if(cur_order.Current.Status == Status_Success){
            ui->pushButton_mod->setVisible(false);
            ui->pushButton_cancle->setVisible(false);
            ui->pushButton_success->setVisible(false);
        }
        if(cur_order.Current.Status == Status_Cancle){
            ui->pushButton_mod->setVisible(false);
            ui->pushButton_cancle->setVisible(false);
            ui->pushButton_success->setVisible(false);
        }
    }

    if(ui->tabWidget->currentWidget()==m_tab_new){
        ui->pushButton_mod->setVisible(true);
        ui->pushButton_cancle->setVisible(true);
        ui->pushButton_success->setVisible(true);
    }
    if(ui->tabWidget->currentWidget()==m_tab_success){
        ui->pushButton_mod->setVisible(false);
        ui->pushButton_cancle->setVisible(false);
        ui->pushButton_success->setVisible(false);
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
    m_tab_success->setHeaderColModel(tab_mode);
    m_tab_all->setHeaderColModel(tab_mode);
}


void OrderManager::on_pushButton_new_clicked()
{
    if(neworer==NULL){
        neworer = new DialogNewOrder();
    }
    if(neworer->exec()==123){
        Order order = neworer->getCurorder();
        m_tab_new->appendOrder(order);
        m_tab_all->appendOrder(order);
    }
}


void OrderManager::on_pushButton_mod_clicked()
{
    if(cur_order.OrderID==""){
        return;
    }
    if(neworer==NULL){
        neworer = new DialogNewOrder();
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

}

void OrderManager::on_pushButton_success_clicked()
{

}

void OrderManager::clearAllSelect()
{
    m_tab_all->clearSelection();
    m_tab_new->clearSelection();
    m_tab_success->clearSelection();
    ui->pushButton_mod->setVisible(false);
    ui->pushButton_cancle->setVisible(false);
    ui->pushButton_success->setVisible(false);
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
