#include "ordermanager.h"
#include "ui_ordermanager.h"
#include "datacenter.h"

OrderManager::OrderManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OrderManager)
{
    ui->setupUi(this);
    tab_mode = QHeaderView::Stretch;

    m_tab_new = new OrderTable();
    m_tab_success = new OrderTable();
    m_tab_all = new OrderTable();

    ui->tabWidget->addTab(m_tab_new,QIcon(":/icon/all.ico"),"新订单");
    ui->tabWidget->addTab(m_tab_success,"已完成");
    ui->tabWidget->addTab(m_tab_all,"全部");
    ui->tabWidget->tabBar()->setMovable(true);
    ui->tabWidget->setFont(QFont("Times", 16, QFont::Bold));
    connect(ui->radioButton_ave,SIGNAL(clicked(bool)),this,SLOT(changeCol()));
    connect(ui->radioButton_content,SIGNAL(clicked(bool)),this,SLOT(changeCol()));
    connect(ui->radioButton_manu,SIGNAL(clicked(bool)),this,SLOT(changeCol()));
    ui->radioButton_ave->setChecked(true);
    id = 1;
}

OrderManager::~OrderManager()
{
    delete ui;
}

void OrderManager::on_pushButton_new_clicked()
{
    Order *order = new Order();
    order->OrderID =QString("%1").arg(id);
    order->MaterielID = QString("%1").arg(id+100);
    order->MaterielDes=QString("这个订单的id为%1").arg(id);
    order->CustomBatch = QString("%1").arg(id+10000);
    order->CustomNote = "无";
    order->ProduceID = QString("%1").arg(id+5000);
    order->Money = id+8000;
    order->OrderNum  = id;
    order->Unit = "元";
    m_tab_new->appendOrder(order);
    dataCenter::instance()->appendOrder(order);
    id++;
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

