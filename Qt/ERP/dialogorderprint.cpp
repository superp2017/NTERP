#include "dialogorderprint.h"
#include "ui_dialogorderprint.h"
#include "datacenter.h"
#include "global.h"
#include "orderservice.h"
#include <QMessageBox>
#include <QFileDialog>
#include "boost/thread.hpp"

DialogOrderPrint::DialogOrderPrint(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogOrderPrint)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
    //    ui->tableWidget->setSelectionBehavior ( QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    //    ui->tableWidget->setSelectionMode ( QAbstractItemView::SingleSelection); //设置选择模式，选择单行
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); //设置表头背景色
    QFont font = ui->tableWidget->horizontalHeader()->font();
    font.setBold(true);
    ui->tableWidget->horizontalHeader()->setFont(font);

    //设置表头内容
    QStringList header;
    header<<tr("生产订单");
    ui->tableWidget->setHorizontalHeaderLabels(header);
//    ui->tableWidget->setSortingEnabled(true);//允许列排序

    ui->comboBox_order_status->addItem("新建订单",Status_New);
    ui->comboBox_order_status->addItem("全部订单",Status_All);
    ui->comboBox_order_status->addItem("完成订单",Status_Success);
    ui->comboBox_order_status->addItem("取消订单",Status_Cancle);

    connect(ui->checkBox_check_all,SIGNAL(clicked(bool)),this,SLOT(selectAll(bool)));
    connect(ui->comboBox_order_status,SIGNAL(currentIndexChanged(int)),this,SLOT(orderStatusChange(int)));
    connect(ui->tableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(cellChecked(int,int)));

    connect(this,SIGNAL(sig_exportCb(bool)),this,SLOT(exportCb(bool)));

    cur_Status="";
    m_checkboxs.clear();
    m_orders.clear();
}

DialogOrderPrint::~DialogOrderPrint()
{
    delete ui;
}

void DialogOrderPrint::initData(QString status)
{
    if(status==Status_New){
        ui->comboBox_order_status->setCurrentIndex(0);
    }else if(status==Status_All){
        ui->comboBox_order_status->setCurrentIndex(1);
    }else if(status==Status_Success){
        ui->comboBox_order_status->setCurrentIndex(2);
    }else if(status==Status_Cancle){
        ui->comboBox_order_status->setCurrentIndex(3);
    }else{
        return;
    }

    updateData(status);
}

void DialogOrderPrint::updateData(QString status)
{
    cur_Status = status;
    m_orders = dataCenter::instance()->StatusOrders(status);
    removeAllRow();
    m_checkboxs.clear();
    for(Order o:m_orders){
        ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
        int row=ui->tableWidget->rowCount()-1;
        setRowData(o,row);
    }
}

QVector<Order> DialogOrderPrint::getSelectOrders()
{
    QVector<Order> ls;
    if(m_orders.size()!=m_checkboxs.size()){
        dataCenter::instance()->showMessage("操作失败!",3000);
        return ls;
    }

    for(int i =0;i<m_checkboxs.size();++i){
        if(m_checkboxs.at(i)->isChecked()){
            ls.push_back(m_orders.at(i));
        }
    }
    return ls;
}

void DialogOrderPrint::on_pushButton_export_clicked()
{
    QVector<Order> ls= getSelectOrders();
    if(ls.size()==0){
        QMessageBox::information(this,"提示","请至少选择一个订单...");
        return;
    }

    QString filepath= QFileDialog::getSaveFileName(NULL,"Save orders",".","Microsoft Office 2007 (*.xlsx)");//获取保存路径
    if(!filepath.isEmpty()){
        boost::thread t(boost::bind(&DialogOrderPrint::doExport,this,ls,filepath));
        t.detach();
        dataCenter::instance()->showLoadding("正在导出...",10000);
    }else{
        dataCenter::instance()->showMessage("保存路径为空!",3000);
    }
}


void DialogOrderPrint::checkBox()
{
    bool check = true;
    bool check_one = false;
    for(QCheckBox* ch:m_checkboxs){
        check    &= ch->isChecked();
        check_one|= ch->isChecked();
    }
    if(check){
        ui->checkBox_check_all->setCheckState(Qt::Checked);
    }else{
        if(check_one)
            ui->checkBox_check_all->setCheckState(Qt::PartiallyChecked);
        else{
            ui->checkBox_check_all->setCheckState(Qt::Unchecked);
        }
    }
}


void DialogOrderPrint::exportCb(bool ok)
{
    dataCenter::instance()->hideLoadding();
    if(ok){
        done(123);
        dataCenter::instance()->showMessage("导出成功!",3000);
    }else{
        dataCenter::instance()->showMessage("导出失败!",3000);
    }
}

void DialogOrderPrint::doExport(QVector<Order> ls,QString filepath)
{
    bool ok = OrderService::exportOrders(ls,filepath,true);
    emit sig_exportCb(ok);
}


void DialogOrderPrint::on_pushButton_print_clicked()
{
    QVector<Order> ls= getSelectOrders();
    if(ls.size()==0){
        QMessageBox::information(this,"提示","请至少选择一个订单...");
        return;
    }
    if(OrderService::printOrders(ls)){
        dataCenter::instance()->showMessage("打印成功!",3000);
    }else{
        dataCenter::instance()->showMessage("打印失败!",3000);
    }
    done(123);
}

void DialogOrderPrint::on_pushButton_cancle_clicked()
{
    done(-1);
}

void DialogOrderPrint::orderStatusChange(int index)
{
    index =0;
    QString status = ui->comboBox_order_status->currentData().toString();
    if(status==cur_Status) return;
    initData(status);
}

void DialogOrderPrint::selectAll(bool checked)
{
    for(QCheckBox *box:m_checkboxs){
        if(box!=NULL){
            box->setChecked(checked);
        }
    }

}

void DialogOrderPrint::cellChecked(int row, int col)
{
    col=0;
    if(row<m_checkboxs.size()&&row>=0){
        m_checkboxs.at(row)->setChecked(!m_checkboxs.at(row)->isChecked());
    }
}



void DialogOrderPrint::removeAllRow()
{
    int count = ui->tableWidget->rowCount();
    for(int i=0;i<count;++i){
        ui->tableWidget->removeRow(0);
    }
}

void DialogOrderPrint::setRowData(Order order, int row)
{
    QCheckBox *check1 = new QCheckBox(order.OrderID);
    ui->tableWidget->setCellWidget(row,0,check1);
    connect(check1,SIGNAL(clicked(bool)),this,SLOT(checkBox()));
    m_checkboxs.push_back(check1);
}



