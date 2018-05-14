#include "ordermanager.h"
#include "ui_ordermanager.h"
#include "datacenter.h"
#include "dialogneworder.h"
#include "orderservice.h"
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
    ui->radioButton_ave->setChecked(true);
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

