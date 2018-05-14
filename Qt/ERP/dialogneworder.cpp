#include "dialogneworder.h"
#include "ui_dialogneworder.h"
#include "datacenter.h"
#include <QCompleter>
#include <QToolTip>
#include <boost/thread/thread.hpp>
#include "orderservice.h"

DialogNewOrder::DialogNewOrder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewOrder)
{
    ui->setupUi(this);
    initCombox(dataCenter::instance()->Customers(),\
               dataCenter::instance()->Batchs(),\
               dataCenter::instance()->Materiels(),\
               dataCenter::instance()->Units());
    connect(dataCenter::instance(),SIGNAL(sig_newPlan(Order,bool)),this,SLOT(newOrderCb(Order,bool)));
}

DialogNewOrder::~DialogNewOrder()
{
    delete ui;
}



void DialogNewOrder::on_pushButton_ok_clicked()
{
    Order  order;
    order.OrderType = ui->comboBox_orderType->currentData().toString();
    order.UID = dataCenter::instance()->CurUser().UID ;
    order.UserName = dataCenter::instance()->CurUser().Name;
    order.MaterielID = ui->comboBox_MaterielID->currentText();
    order.MaterielDes = ui->lineEdit_MaterielDes->text();
    order.Unit = ui->comboBox_unit->currentText();
    order.CustomID = ui->comboBox_customerName->currentData().toString();
    order.CustomName = ui->comboBox_customerName->currentText();
    order.CustomBatch = ui->comboBox_CustomBatch->currentText();
    order.CustomNote = ui->textEdit_custom_note->toPlainText();
    order.OrderNum =ui->spinBox_num->value();

    if(!checkOrder(order)){
        return;
    }
    para = OrderService::toJsonObject(order);
    boost::thread t(boost::bind(&dataCenter::newOrder,dataCenter::instance(),para));
    t.detach();
    dataCenter::instance()->showLoadding("正在网络请求...",5000,Qt::black);
}

void DialogNewOrder::newOrderCb(Order order,bool ok)
{
    dataCenter::instance()->hideLoadding();
    if(ok){
        dataCenter::instance()->showMessage("下单成功!",4000);
        curorder = order;
        done(123);
    }else{
        dataCenter::instance()->showMessage("下单失败!",4000);
        curorder = order;
        done(0);
    }
}


bool DialogNewOrder::checkOrder(Order order)
{

    if(order.CustomName==""){
        QToolTip::showText(ui->comboBox_customerName->mapToGlobal(QPoint(100, 0)), "客户不能为空!");
        return false;
    }
    if(order.MaterielID==""){
        QToolTip::showText(ui->comboBox_MaterielID->mapToGlobal(QPoint(100, 0)), "物料编号不能为空!");
        return false;
    }

    if(order.MaterielDes==""){
        QToolTip::showText(ui->lineEdit_MaterielDes->mapToGlobal(QPoint(100, 0)), "物料描述不能为空!");
        return false;
    }

    if(order.Unit==""){
        QToolTip::showText(ui->comboBox_unit->mapToGlobal(QPoint(100, 0)), "单位不能为空!");
        return false;
    }

    if(order.OrderNum<=0){
        QToolTip::showText(ui->spinBox_num->mapToGlobal(QPoint(100, 0)), "订单数量填写不正确!");
        return false;
    }

    if(dataCenter::instance()->checkMaterielID(order.MaterielID)){
        QToolTip::showText(ui->comboBox_MaterielID->mapToGlobal(QPoint(100, 0)), "物料编号已经存在!");
        return false;
    }

    return true;
}

Order DialogNewOrder::getCurorder() const
{
    return curorder;
}

QJsonObject DialogNewOrder::getPara() const
{
    return para;
}



void DialogNewOrder::initCombox(QVector<Customer> custom, QVector<QString> batch, \
                                QVector<Materiel > materID, QVector<QString> unit)

{
    ui->comboBox_orderType->clear();
    ui->comboBox_orderType->addItem("普通订单","0");
    ui->comboBox_orderType->addItem("批量订单","1");
    ui->comboBox_orderType->addItem("试样订单","1");

    ui->comboBox_customerName->clear();
    QStringList customlist;
    for(Customer c:custom){
        customlist<<c.Name;
        ui->comboBox_customerName->addItem(c.Name,c.CID);
    }
    QCompleter *completerCustomer = new QCompleter(customlist, this);

    ui->comboBox_customerName->setEditable(true);
    ui->comboBox_customerName->setCompleter(completerCustomer);


    QCompleter *completerBatch = new QCompleter(batch.toList(), this);
    ui->comboBox_CustomBatch->clear();
    ui->comboBox_CustomBatch->addItems(batch.toList());
    ui->comboBox_CustomBatch->setEditable(true);
    ui->comboBox_CustomBatch->setCompleter(completerBatch);

    QStringList materlist;
    for(Materiel c:materID){
        materlist<<c.MaterDes;
    }

    QCompleter *completerMater = new QCompleter(materlist, this);
    ui->comboBox_MaterielID->clear();
    ui->comboBox_MaterielID->addItems(materlist);
    ui->comboBox_MaterielID->setEditable(true);
    ui->comboBox_MaterielID->setCompleter(completerMater);

    QCompleter *completerUnit = new QCompleter(unit.toList(), this);
    ui->comboBox_MaterielID->clear();
    ui->comboBox_MaterielID->addItems(unit.toList());
    ui->comboBox_MaterielID->setEditable(true);
    ui->comboBox_MaterielID->setCompleter(completerUnit);


    ui->comboBox_MaterielID->addItem(ItemNewMater);
    ui->comboBox_customerName->addItem(ItemNewCustom);
    ui->comboBox_unit->addItem(ItemNewUnit);
    ui->comboBox_customerName->setCurrentIndex(-1);
    ui->comboBox_MaterielID->setCurrentIndex(-1);
}





void DialogNewOrder::on_pushButton_cancel_clicked()
{
    done(-1);
}
