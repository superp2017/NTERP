#include "dialogorderproduceorout.h"
#include "ui_dialogorderproduceorout.h"
#include "datacenter.h"
#include "boost/thread.hpp"
DialogOrderProduceOrOut::DialogOrderProduceOrOut(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogOrderProduceOrOut)
{
    ui->setupUi(this);
}

DialogOrderProduceOrOut::~DialogOrderProduceOrOut()
{
    delete ui;
}

void DialogOrderProduceOrOut::initData(bool isSuccess,QString OrderID, double un,QString unit)
{
    m_isSuccess = isSuccess;
    ui->lineEdit_orderID->setText(OrderID);
    curOrderID = OrderID;
    ui->lineEdit_un->setText(QString("%1").arg(un/100));
    ui->doubleSpinBox->setRange(1.0,un/100);
    ui->doubleSpinBox->setValue(un/100);
    if(isSuccess){
        this->setWindowTitle("订单出库");
        QString str1=QString("未出库的数量")+QString("(")+unit+QString(")");
        QString str2=QString("准备出库的数量")+QString("(")+unit+QString(")");
        ui->label_un->setText(str1);
        ui->label_will->setText(str2);
        ui->pushButton_ok->setText("出库");
    }else{
        this->setWindowTitle("订单成品");
        QString str1=QString("未成品的数量")+QString("(")+unit+QString(")");
        QString str2=QString("准备成品的数量")+QString("(")+unit+QString(")");
        ui->label_un->setText(str1);
        ui->label_will->setText(str2);
        ui->pushButton_ok->setText("出库");
    }
}

void DialogOrderProduceOrOut::on_pushButton_ok_clicked()
{
    int num = ui->doubleSpinBox->value()*100;
    QJsonObject obj;
    obj.insert("OrderID",curOrderID);
    obj.insert("Num",num);
    if(m_isSuccess){
        boost::thread t(boost::bind(&dataCenter::net_finishOrder,dataCenter::instance(),obj));
        t.detach();
        dataCenter::instance()->pub_showLoadding("正在网络请求...",5000,Qt::black);
    }else{
        boost::thread t(boost::bind(&dataCenter::net_produceOrder,dataCenter::instance(),obj));
        t.detach();
        dataCenter::instance()->pub_showLoadding("正在网络请求...",5000,Qt::black);

    }
    done(123);
}

void DialogOrderProduceOrOut::on_pushButton_exit_clicked()
{
    done(-1);
}
