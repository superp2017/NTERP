#include "formfinance.h"
#include "ui_formfinance.h"
#include "datacenter.h"
#include <QToolTip>
#include <QTableWidgetItem>

FormFinance::FormFinance(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormFinance)
{
    ui->setupUi(this);
    ui->pushButton_export->setStyleSheet("QPushButton{border-image: url(:/icon/export.png);}"
                                         "QPushButton:hover{border-image: url(:/icon/export-a.png);}"
                                         "QPushButton:pressed{border-image: url(:/icon/export-a.png);}"
                                         "QPushButton:checked{border-image: url(:/icon/export-a.png);}");

    ui->pushButton_search->setStyleSheet("QPushButton{border-image: url(:/icon/search.png);}"
                                         "QPushButton:hover{border-image: url(:/icon/search-a.png);}"
                                         "QPushButton:pressed{border-image: url(:/icon/search-a.png);}"
                                         "QPushButton:checked{border-image: url(:/icon/search-a.png);}");


    ui->dateEdit_start->setDate(QDate::currentDate().addDays(-1));
    ui->dateEdit_end->setDate(QDate::currentDate());


    ui->tableWidget->setColumnCount(13);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //设置表头内容
    QStringList header;
    header<<tr("生产批号")<<tr("订单类型")<<tr("分厂名称")<<tr("客户名称")<<tr("物料描述")\
         <<tr("订单总量")<<tr("单位")<<tr("客户批次")<<tr("客户备注")<<tr("未税单价(元)")\
        <<tr("未税总价(元)")<<tr("状态")<<tr("创建时间");

    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->setSortingEnabled(true);//允许列排序

    initUI();

    initOrder(dataCenter::instance()->pub_StatusOrders(Status_All));
}

FormFinance::~FormFinance()
{
    delete ui;
}

void FormFinance::initUI()
{

    for(Customer cus: dataCenter::instance()->pub_Customers()){
        ui->comboBox_company->addItem(cus.Name);
    }
    ui->comboBox_order_type->addItem("普通订单","1");
    ui->comboBox_order_type->addItem("试样订单","2");
    ui->comboBox_order_type->addItem("返工订单","3");

    ui->comboBox_order_status->addItem("全部订单",Status_All);
    ui->comboBox_order_status->addItem("新建订单",Status_New);
    ui->comboBox_order_status->addItem("已成品",Status_Produce);
    ui->comboBox_order_status->addItem("已出库",Status_Success);
    ui->comboBox_order_status->addItem("已取消",Status_Cancle);

    ui->comboBox_factort->addItem("全部分厂","00");
    ui->comboBox_factort->addItem("涂覆分厂","01");
    ui->comboBox_factort->addItem("滚镀分厂","02");
    ui->comboBox_factort->addItem("挂镀分厂","03");

}


void FormFinance::initOrder(QVector<Order> list)
{
    ui->tableWidget->removeAllRow();
    for(Order o:list){
        ui->tableWidget->setRowCount(list.size());
        for(int i=0;i<list.size();++i){
            setRowData(list.at(i),i);
        }
    }
}


void FormFinance::on_pushButton_search_clicked()
{
    qint64 start,end;
    if(ui->groupBox_time->isChecked()){
        start = ui->dateEdit_start->dateTime().toMSecsSinceEpoch();
        end = ui->dateEdit_end->dateTime().toMSecsSinceEpoch();
        if(end<start){
            QToolTip::showText(ui->dateEdit_end->mapToGlobal(QPoint(100, 0)), "结束时间不能小于开始时间!");
            return;
        }
    }
    if(ui->comboBox_company->currentText()==""){
        QToolTip::showText(ui->comboBox_company->mapToGlobal(QPoint(100, 0)), "请至少选择一个公司!");
        return;
    }

    do_search(ui->comboBox_company->currentText(),\
              ui->comboBox_order_status->currentData().toString(),\
              ui->comboBox_order_type->currentData().toString(),\
              ui->comboBox_factort->currentData().toString(),\
              start,end,\
              ui->groupBox_company->isChecked(),\
              ui->groupBox_order_type->isChecked(),\
              ui->groupBox_factory->isChecked(),\
              ui->groupBox_time->isChecked());

}

void FormFinance::on_pushButton_export_clicked()
{

}

void FormFinance::do_search(QString cusName, QString status, QString type, QString fac, \
                            qint64 start, qint64 end, bool Iscus, bool Isrtype,\
                            bool IsStatus, bool isFac, bool time)
{

}


void FormFinance::setRowData(Order para, int row)
{
    QTableWidgetItem *item0 = ui->tableWidget->item(row,0);
    QTableWidgetItem *item1 = ui->tableWidget->item(row,1);
    QTableWidgetItem *item2 = ui->tableWidget->item(row,2);
    QTableWidgetItem *item3 = ui->tableWidget->item(row,3);
    QTableWidgetItem *item4 = ui->tableWidget->item(row,4);
    QTableWidgetItem *item5 = ui->tableWidget->item(row,5);
    QTableWidgetItem *item6 = ui->tableWidget->item(row,6);
    QTableWidgetItem *item7 = ui->tableWidget->item(row,7);
    QTableWidgetItem *item8 = ui->tableWidget->item(row,8);
    QTableWidgetItem *item9 = ui->tableWidget->item(row,9);
    QTableWidgetItem *item10 = ui->tableWidget->item(row,10);
    QTableWidgetItem *item11 = ui->tableWidget->item(row,11);
    QTableWidgetItem *item12 = ui->tableWidget->item(row,12);
    if(item0==NULL){
        item0 = new QTableWidgetItem();
        ui->tableWidget->setItem(row,0,item0);
    }
    if(item1==NULL){
        item1 = new QTableWidgetItem();
        ui->tableWidget->setItem(row,1,item1);
    }
    if(item2==NULL){
        item2 = new QTableWidgetItem();
        ui->tableWidget->setItem(row,2,item2);
    }
    if(item3==NULL){
        item3 = new QTableWidgetItem();
        ui->tableWidget->setItem(row,3,item3);
    }
    if(item4==NULL){
        item4 = new QTableWidgetItem();
        ui->tableWidget->setItem(row,4,item4);
    }
    if(item5==NULL){
        item5 = new QTableWidgetItem();
        ui->tableWidget->setItem(row,5,item5);
    }
    if(item6==NULL){
        item6 = new QTableWidgetItem();
        ui->tableWidget->setItem(row,6,item6);
    }
    if(item7==NULL){
        item7 = new QTableWidgetItem();
        ui->tableWidget->setItem(row,7,item7);
    }
    if(item8==NULL){
        item8 = new QTableWidgetItem();
        ui->tableWidget->setItem(row,8,item8);
    }
    if(item9==NULL){
        item9 = new QTableWidgetItem();
        ui->tableWidget->setItem(row,9,item9);
    }
    if(item10==NULL){
        item10 = new QTableWidgetItem();
        ui->tableWidget->setItem(row,10,item10);
    }
    if(item11==NULL){
        item11 = new QTableWidgetItem();
        ui->tableWidget->setItem(row,11,item11);
    }
    if(item11==NULL){
        item11 = new QTableWidgetItem();
        ui->tableWidget->setItem(row,11,item11);
    }
    if(item12==NULL){
        item12 = new QTableWidgetItem();
        ui->tableWidget->setItem(row,12,item12);
    }


    item0->setText(para.OrderID);
    QString type;
    if(para.OrderType=="1"){
        type="普通订单";
    }
    if(para.OrderType=="2"){
        type="试样订单";
    }
    if(para.OrderType=="3"){
        type="返工订单";
    }
    item1->setText(type);
    item2->setText(para.Factory);
    item3->setText(para.CustomName);
    item4->setText(para.MaterielDes);
    item5->setText(QString("%1").arg(para.OrderNum));
    item6->setText(para.Unit);
    item7->setText(para.CustomBatch);
    item8->setText(para.CustomNote);
    item9->setText(QString("%1").arg(para.Money));
    item10->setText(QString("%1").arg(para.TotleMoney));
    QString status;

    if(para.Current.Status==Status_Cancle){
        status="已取消";
    }
    if(para.Current.Status==Status_PartSuccess){
        status="部分出库";
    }
    if(para.Current.Status==Status_PartProduce){
        status="部分成品";
    }
    if(para.Current.Status==Status_New){
        status="新建";
    }
    if(para.Current.Status==Status_Produce){
        status="全部成品";
    }
    if(para.Current.Status==Status_Success){
        status="全部出库";
    }
    if(para.Current.Status==Status_Part_Part){
        status="部分成品部分出库";
    }


    item11->setText(QString("%1").arg(status));
    item12->setText( QDateTime::fromString(para.CreatTime,"yyyy-MM-dd HH:mm:ss").toString("yyyy-MM-dd"));

    item0->setTextAlignment(Qt::AlignCenter);
    item1->setTextAlignment(Qt::AlignCenter);
    item2->setTextAlignment(Qt::AlignCenter);
    item3->setTextAlignment(Qt::AlignCenter);
    item4->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    item5->setTextAlignment(Qt::AlignCenter);
    item6->setTextAlignment(Qt::AlignCenter);
    item7->setTextAlignment(Qt::AlignCenter);
    item8->setTextAlignment(Qt::AlignCenter);
    item9->setTextAlignment(Qt::AlignCenter);
    item10->setTextAlignment(Qt::AlignCenter);
    item11->setTextAlignment(Qt::AlignCenter);
    item12->setTextAlignment(Qt::AlignCenter);
}



