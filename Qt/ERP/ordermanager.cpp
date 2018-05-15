#include "ordermanager.h"
#include "ui_ordermanager.h"
#include "datacenter.h"
#include "dialogneworder.h"
#include "orderservice.h"
#include <QDebug>

OrderManager::OrderManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OrderManager)
{
    ui->setupUi(this);
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

    ui->pushButton_mod->setEnabled(false);
    ui->pushButton_cancle->setEnabled(false);
    ui->pushButton_success->setEnabled(false);

}

OrderManager::~OrderManager()
{
    delete ui;
}


void OrderManager::on_pushButton_new_clicked()
{
    DialogNewOrder neworer;
    if(neworer.exec()==123){
        Order order = neworer.getCurorder();
        m_tab_new->appendOrder(order);
        m_tab_all->appendOrder(order);
    }
}


void OrderManager::orderClick(QString orderID)
{
    bool exist =false;
    Order cur_order = dataCenter::instance()->getOrder(orderID,exist);
    if(!exist){
        return;
    }

    if(ui->tabWidget->currentWidget()==m_tab_all){
        if(cur_order.Current.Status == Status_New){
            ui->pushButton_mod->setEnabled(true);
            ui->pushButton_cancle->setEnabled(true);
            ui->pushButton_success->setEnabled(true);
        }
        if(cur_order.Current.Status == Status_Success){
            ui->pushButton_mod->setEnabled(false);
            ui->pushButton_cancle->setEnabled(false);
            ui->pushButton_success->setEnabled(false);
        }
        if(cur_order.Current.Status == Status_Cancle){
            ui->pushButton_mod->setEnabled(false);
            ui->pushButton_cancle->setEnabled(false);
            ui->pushButton_success->setEnabled(false);
        }
    }

    if(ui->tabWidget->currentWidget()==m_tab_new){
        ui->pushButton_mod->setEnabled(true);
        ui->pushButton_cancle->setEnabled(true);
        ui->pushButton_success->setEnabled(true);
    }
    if(ui->tabWidget->currentWidget()==m_tab_success){
        ui->pushButton_mod->setEnabled(false);
        ui->pushButton_cancle->setEnabled(false);
        ui->pushButton_success->setEnabled(false);
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




void OrderManager::on_pushButton_mod_clicked()
{

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
    ui->pushButton_mod->setEnabled(false);
    ui->pushButton_cancle->setEnabled(false);
    ui->pushButton_success->setEnabled(false);

}
