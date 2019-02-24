#include "dialogorderdetail.h"
#include "ui_dialogorderdetail.h"


DialogOrderDetail::DialogOrderDetail(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogOrderDetail)
{
    ui->setupUi(this);
    ui->tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior ( QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    ui->tableWidget->setSelectionMode ( QAbstractItemView::SingleSelection); //设置选择模式，选择单行
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setColumnCount(3);
    //设置表头内容
    QStringList header;
    header<<tr("操作者")<<tr("动作")<<tr("操作时间");
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->setSortingEnabled(true);//允许列排序
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); //设置表头背景色
    ui->pushButton_out_table->setVisible(false);

    ////神州专用////
    ui->label_fac_name->hide();
    ui->label_line_name->hide();
    ui->lineEdit_Factory->hide();
    ui->lineEdit_production_line->hide();
      ////神州专用////
}

DialogOrderDetail::~DialogOrderDetail()
{
    delete ui;
}

void DialogOrderDetail::init(Order order)
{
    ui->lineEdit_createName->setText(order.UserName);
    ui->lineEdit_creatTime->setText(order.CreatTime);
    ui->lineEdit_customerName->setText(order.CustomName);
    ui->lineEdit_customer_batch->setText(order.CustomBatch);
    ui->lineEdit_Factory->setText(order.Factory);
    ui->lineEdit_production_line->setText(order.ProductionLine);
    ui->lineEdit_materID->setText(order.MaterielID);
    ui->lineEdit_money->setText(QString("%1").arg(order.Money));
    ui->lineEdit_orderID->setText(order.OrderID);
    ui->lineEdit_orderNum->setText(QString("%1").arg(order.OrderNum)+order.Unit);
    ui->lineEdit_produceNum->setText(QString("%1").arg(order.ProduceNum)+order.Unit);
    ui->lineEdit_successNum->setText(QString("%1").arg(order.SuccessNum)+order.Unit);
    QString orderType;
    if(order.OrderType=="1"){
        orderType = "普通订单";
    }
    if(order.OrderType=="2"){
        orderType = "试样订单";
    }
    if(order.OrderType=="3"){
        orderType = "返工订单";
    }
    ui->lineEdit_orderType->setText(orderType);
    ui->lineEdit_outTime->setText(order.SuccessTime);
    ui->lineEdit_produceID->setText(order.ProduceTime);

    QString status;
//    if(order.Current.Status==Status_New){
//        status="新建";
//        // ui->pushButton_out_table->setVisible(false);
//    }
//    if(order.Current.Status==Status_Produce){
//        status="已生产";
//        // ui->pushButton_out_table->setVisible(false);
//    }
//    if(order.Current.Status==Status_Success){
//        status="已出货";
//        // ui->pushButton_out_table->setVisible(true);
//    }
//    if(order.Current.Status==Status_Cancle){
//        status="已取消";
//    }

    if(order.Current.Status==Status_Cancle){
        status="已取消";
    }
    if(order.Current.Status==Status_PartSuccess){
        status="部分出库";
    }
    if(order.Current.Status==Status_PartProduce){
        status="部分成品";
    }
    if(order.Current.Status==Status_New){
        status="新建";
    }
    if(order.Current.Status==Status_Produce){
        status="全部成品";
    }
    if(order.Current.Status==Status_Success){
        status="全部出库";
    }
    if(order.Current.Status==Status_Part_Part){
        status="部分成品部分出库";
    }


    ui->lineEdit_status->setText(status);
    ui->textEdit_customNote->setText(order.CustomNote);
    ui->textEdit_materDes->setText(order.MaterielDes);

    removeAllRow();

    for(OderFlow flow:order.Flow){
        int row =ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(row+1);
        QTableWidgetItem *item1= new QTableWidgetItem(flow.UserName);
        QTableWidgetItem *item2= new QTableWidgetItem(flow.Action);
        QTableWidgetItem *item3= new QTableWidgetItem(flow.OpreatTime);
        ui->tableWidget->setItem(row,0,item1);
        ui->tableWidget->setItem(row,1,item2);
        ui->tableWidget->setItem(row,2,item3);
        item1->setTextAlignment(Qt::AlignCenter);
        item2->setTextAlignment(Qt::AlignCenter);
        item3->setTextAlignment(Qt::AlignCenter);
    }
}

void DialogOrderDetail::removeAllRow()
{
    int count = ui->tableWidget->rowCount();
    for(int i=0;i<count;++i){
        ui->tableWidget->removeRow(0);
    }
}


void DialogOrderDetail::on_pushButton_ok_clicked()
{
    this->done(123);
}


void DialogOrderDetail::on_pushButton_out_table_clicked()
{

}
