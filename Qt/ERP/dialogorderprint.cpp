#include "dialogorderprint.h"
#include "ui_dialogorderprint.h"
#include "datacenter.h"
#include "global.h"
#include "orderservice.h"
#include <QMessageBox>
#include <QFileDialog>
#include "boost/thread.hpp"
#include <QScrollBar>
#include <QToolTip>
#include <QDateTime>

DialogOrderPrint::DialogOrderPrint(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogOrderPrint)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
    //    ui->tableWidget->setSelectionBehavior ( QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    //    ui->tableWidget->setSelectionMode ( QAbstractItemView::SingleSelection); //设置选择模式，选择单行
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); //设置表头背景色
    QFont font = ui->tableWidget->horizontalHeader()->font();
    font.setBold(true);
    ui->tableWidget->horizontalHeader()->setFont(font);

    //设置表头内容
    QStringList header;
    header<<tr("生产批号")<<tr("物料描述")<<tr("创建时间");
    ui->tableWidget->setHorizontalHeaderLabels(header);

    ui->comboBox_order_factory->addItem("全部分厂","00");
    ui->comboBox_order_factory->addItem("涂覆分厂","01");
    ui->comboBox_order_factory->addItem("滚镀分厂","02");
    ui->comboBox_order_factory->addItem("挂镀分厂","03");

    ui->comboBox_order_status->addItem("全部",Status_All);
    ui->comboBox_order_status->addItem("新订单",Status_New);
    ui->comboBox_order_status->addItem("已成品",Status_Produce);
    ui->comboBox_order_status->addItem("已出库",Status_Success);
    ui->comboBox_order_status->addItem("已取消",Status_Cancle);

    //    cur_Status="";
    //    cur_factory="";

    connect(ui->checkBox_check_all,SIGNAL(clicked(bool)),this,SLOT(selectAll(bool)));
    //    connect(ui->comboBox_order_status,SIGNAL(currentIndexChanged(int)),this,SLOT(orderStatusChange(int)));
    //    connect(ui->comboBox_order_factory,SIGNAL(currentIndexChanged(int)),this,SLOT(factoryChange(int)));
    connect(ui->tableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(cellChecked(int,int)));

    connect(this,SIGNAL(sig_exportCb(bool)),this,SLOT(exportCb(bool)));

    connect(ui->tableWidget->verticalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(onSliderChanged(int)));

    m_checkboxs.clear();
    m_orders.clear();

    ui->dateEdit_start_time->setDate(QDate::currentDate().addMonths(-1));
    ui->dateEdit_end_time->setDate(QDate::currentDate());
    ui->checkBox_check_all->setEnabled(m_checkboxs.size()>0);
}

DialogOrderPrint::~DialogOrderPrint()
{
    delete ui;
}
//void DialogOrderPrint::initData(QString status,QString factory)
//{
//    if(status== Status_All){
//        ui->comboBox_order_status->setCurrentIndex(0);
//    }else if(status==Status_New){
//        ui->comboBox_order_status->setCurrentIndex(1);
//    }else if(status==Status_Produce){
//        ui->comboBox_order_status->setCurrentIndex(2);
//    }else if(status==Status_Success){
//        ui->comboBox_order_status->setCurrentIndex(3);
//    }else if(status==Status_Cancle){
//        ui->comboBox_order_status->setCurrentIndex(4);
//    }else{
//        return;
//    }
//    if(factory=="全部分厂"){
//        ui->comboBox_order_factory->setCurrentIndex(0);
//    }else if(factory=="涂覆分厂"){
//        ui->comboBox_order_factory->setCurrentIndex(1);
//    }else if(factory=="滚镀分厂"){
//        ui->comboBox_order_factory->setCurrentIndex(2);
//    }else if(factory=="挂镀分厂"){
//        ui->comboBox_order_factory->setCurrentIndex(3);
//    }else{
//        return;
//    }

//    updateData(status, factory);
//}


void DialogOrderPrint::on_pushButton_query_clicked()
{
    ui->checkBox_check_all->setChecked(false);
    QString status  = ui->comboBox_order_status->currentData().toString();
    QString factory     = ui->comboBox_order_factory->currentText();
    bool isTime     = ui->groupBox_time->isChecked();
    int startTime = 0;
    int endTime = 0;
    if(isTime){
        startTime   = ui->dateEdit_start_time->dateTime().toSecsSinceEpoch();
        endTime     = ui->dateEdit_end_time->dateTime().toSecsSinceEpoch();
        if(endTime<startTime){
            QToolTip::showText(ui->dateEdit_end_time->mapToGlobal(QPoint(100, 0)), "结束时间不能小于开始时间!");
            return;
        }
        if(startTime>QDateTime::currentSecsSinceEpoch()){
            QToolTip::showText(ui->dateEdit_start_time->mapToGlobal(QPoint(100, 0)), "开始时间不能大于当前时间!");
            return;
        }
    }
    removeAllRow();
    m_checkboxs.clear();
    m_orders = dataCenter::instance()->pub_StatusOrders(status);
    for(Order o:m_orders){
        if(o.Factory==factory||factory=="全部分厂"){
            if(isTime&&(o.CreatStamp<startTime||o.CreatStamp>endTime)) continue;
            ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
            int row=ui->tableWidget->rowCount()-1;
            setRowData(o,row);
        }
    }
    ui->checkBox_check_all->setEnabled(m_checkboxs.size()>0);
}


//void DialogOrderPrint::updateData(QString status, QString factory)
//{
//    cur_Status = status;
//    cur_factory = factory;
//    m_orders = dataCenter::instance()->pub_StatusOrders(status);
//    removeAllRow();
//    m_checkboxs.clear();
//    for(Order o:m_orders){
//        if(o.Factory==factory||factory=="全部分厂"){
//            ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
//            int row=ui->tableWidget->rowCount()-1;
//            setRowData(o,row);
//        }
//    }
//}

QVector<Order> DialogOrderPrint::getSelectOrders()
{
    QVector<Order> ls;
    //    if(m_orders.size()!=m_checkboxs.size()){
    //        dataCenter::instance()->pub_showMessage("操作失败!",3000);
    //        return ls;
    //    }

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
        dataCenter::instance()->pub_showLoadding("正在导出...",30000);
    }else{
        dataCenter::instance()->pub_showMessage("保存路径为空!",3000);
    }
}


void DialogOrderPrint::checkBox()
{
    bool check = true;
    //  bool check_one = false;
    for(QCheckBox* ch:m_checkboxs){
        check    &= ch->isChecked();
        //     check_one|= ch->isChecked();
    }
    if(check){
        ui->checkBox_check_all->setCheckState(Qt::Checked);
    }else{
        //        if(check_one)
        //            ui->checkBox_check_all->setCheckState(Qt::PartiallyChecked);
        //        else{
        ui->checkBox_check_all->setCheckState(Qt::Unchecked);
        //      }
    }
}

void DialogOrderPrint::onSliderChanged(int v)
{
    if(ui->tableWidget->verticalScrollBar()->isVisible()&&v>=ui->tableWidget->verticalScrollBar()->maximum()){
        dataCenter::instance()->pub_getAllOrders(2);
    }
}




void DialogOrderPrint::exportCb(bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        done(123);
        dataCenter::instance()->pub_showMessage("导出成功!",3000);
    }else{
        dataCenter::instance()->pub_showMessage("导出失败!",3000);
    }
}

void DialogOrderPrint::doExport(QVector<Order> ls,QString filepath)
{
    bool ok = OrderService::exportOrders(ui->comboBox_order_status->currentData().toString(),ls,filepath,\
                                         dataCenter::instance()->pub_CurUser().Author,\
                                         dataCenter::instance()->CurSettings().isExportOpen);
    emit sig_exportCb(ok);
}



void DialogOrderPrint::on_pushButton_cancle_clicked()
{
    done(-1);
}

//void DialogOrderPrint::orderStatusChange(int index)
//{
//    index =0;
//    QString status = ui->comboBox_order_status->currentData().toString();
//    if(status==cur_Status) return;
//    initData(status,ui->comboBox_order_factory->currentText());
//}

//void DialogOrderPrint::factoryChange(int index)
//{
//    index =0;
//    QString fac =ui->comboBox_order_factory->currentText();
//    if(cur_factory==fac) return;
//    initData(ui->comboBox_order_status->currentData().toString(),fac);
//}

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
    ui->tableWidget->setItem(row,1,new QTableWidgetItem(order.MaterielDes));
    ui->tableWidget->setItem(row,2,new QTableWidgetItem(order.CreatTime));
}




