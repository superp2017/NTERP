#include "ordermanager.h"
#include "ui_ordermanager.h"

OrderManager::OrderManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OrderManager)
{
    ui->setupUi(this);
    tab_mode = QHeaderView::Stretch;

    m_tab_new = new OrderTable();
    m_tab_success = new OrderTable();
    m_tab_all = new OrderTable();

    m_tab_new->setEditTriggers(QTableWidget::NoEditTriggers);
    m_tab_new->setSelectionBehavior ( QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    m_tab_new->setSelectionMode ( QAbstractItemView::SingleSelection); //设置选择模式，选择单行
    m_tab_new->horizontalHeader()->setSectionResizeMode(tab_mode);

    m_tab_success->setEditTriggers(QTableWidget::NoEditTriggers);
    m_tab_success->setSelectionBehavior ( QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    m_tab_success->setSelectionMode ( QAbstractItemView::SingleSelection); //设置选择模式，选择单行
    m_tab_success->horizontalHeader()->setSectionResizeMode(tab_mode);

    m_tab_all->setEditTriggers(QTableWidget::NoEditTriggers);
    m_tab_all->setSelectionBehavior ( QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    m_tab_all->setSelectionMode ( QAbstractItemView::SingleSelection); //设置选择模式，选择单行
    m_tab_all->horizontalHeader()->setSectionResizeMode(tab_mode);


    ui->tabWidget->addTab(m_tab_new,"新订单");
    ui->tabWidget->addTab(m_tab_success,"已完成");
    ui->tabWidget->addTab(m_tab_all,"全部");

    connect(ui->radioButton_ave,SIGNAL(clicked(bool)),this,SLOT(changeCol()));
    connect(ui->radioButton_content,SIGNAL(clicked(bool)),this,SLOT(changeCol()));
    connect(ui->radioButton_manu,SIGNAL(clicked(bool)),this,SLOT(changeCol()));
}

OrderManager::~OrderManager()
{
    delete ui;
}

void OrderManager::on_pushButton_new_clicked()
{

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
    m_tab_new->horizontalHeader()->setSectionResizeMode(tab_mode);
    m_tab_success->horizontalHeader()->setSectionResizeMode(tab_mode);
    m_tab_all->horizontalHeader()->setSectionResizeMode(tab_mode);
}

