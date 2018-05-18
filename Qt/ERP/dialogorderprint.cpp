#include "dialogorderprint.h"
#include "ui_dialogorderprint.h"
#include "datacenter.h"
#include "global.h"

DialogOrderPrint::DialogOrderPrint(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogOrderPrint)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(2);
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
    header<<tr("选择")<<tr("生产订单");
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->setSortingEnabled(true);//允许列排序

    ui->comboBox_order_status->addItem("新建",Status_New);
    ui->comboBox_order_status->addItem("全部",Status_All);
    ui->comboBox_order_status->addItem("完成",Status_Success);
    ui->comboBox_order_status->addItem("取消",Status_Cancle);

    connect(ui->checkBox_check_all,SIGNAL(clicked(bool)),this,SLOT(selectAll(bool)));
    connect(ui->comboBox_order_status,SIGNAL(currentIndexChanged(int)),this,SLOT(orderStatusChange(int)));
    connect(ui->tableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(cellChecked(int,int)));

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



void DialogOrderPrint::on_pushButton_expot_clicked()
{
    done(456);
}

void DialogOrderPrint::on_pushButton_print_clicked()
{
    done(123);
}

void DialogOrderPrint::on_pushButton_cancle_clicked()
{
    done(-1);
}

void DialogOrderPrint::orderStatusChange(int index)
{
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
    QCheckBox *check1 = new QCheckBox();
    ui->tableWidget->setCellWidget(row,0,check1);
    m_checkboxs.push_back(check1);
    QTableWidgetItem *item1 = ui->tableWidget->item(row,1);
    if(item1==NULL){
        item1 = new QTableWidgetItem();
        ui->tableWidget->setItem(row,1,item1);
    }
    item1->setText(order.OrderID);
    item1->setTextAlignment(Qt::AlignCenter);
    QTableWidgetItem *item0 = ui->tableWidget->item(row,0);
    if(item0!=NULL){
        item0->setTextAlignment(Qt::AlignCenter);
    }
}

