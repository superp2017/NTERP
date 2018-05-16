#include "dialogneworder.h"
#include "ui_dialogneworder.h"
#include "datacenter.h"
#include <QCompleter>
#include <QToolTip>
#include "orderservice.h"
#include "dialognewcustom.h"
#include "service_global.h"
#include "dialognewmateriel.h"
#include "dialognewunit.h"
#include "boost/thread.hpp"

DialogNewOrder::DialogNewOrder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewOrder),
    m_isNewMode(true)
{
    ui->setupUi(this);
    initCombox(dataCenter::instance()->Customers(),\
               dataCenter::instance()->Batchs(),\
               dataCenter::instance()->Materiels(),\
               dataCenter::instance()->Units());

    connect(dataCenter::instance(),SIGNAL(sig_newPlan(Order,bool)),this,SLOT(newOrderCb(Order,bool)));
    connect(dataCenter::instance(),SIGNAL(sig_modPlan(Order,bool)),this,SLOT(modOrderCb(Order,bool)));

    connect(ui->comboBox_customerName,SIGNAL(currentIndexChanged(QString)),this,SLOT(customChange(QString)));
    connect(ui->comboBox_MaterielID,SIGNAL(currentIndexChanged(QString)),this,SLOT(materielChange(QString)));
    connect(ui->comboBox_unit,SIGNAL(currentIndexChanged(QString)),this,SLOT(unitChange(QString)));
    changeModel();
}

DialogNewOrder::~DialogNewOrder()
{
    delete ui;
}


void DialogNewOrder::initCombox(QVector<Customer> custom, QVector<QString> batch, \
                                QVector<Materiel > materID, QVector<QString> unit)

{
    ui->comboBox_orderType->clear();
    ui->comboBox_orderType->addItem("普通订单","0");
    ui->comboBox_orderType->addItem("批量订单","1");
    ui->comboBox_orderType->addItem("试样订单","2");

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

    ui->comboBox_MaterielID->clear();
    ui->comboBox_MaterielID->setEditable(true);
    QStringList materlist;
    for(Materiel c:materID){
        materlist<<c.MaterID;
        ui->comboBox_MaterielID->addItem(c.MaterID,c.MaterDes);
    }

    QCompleter *completerMater = new QCompleter(materlist, this);
    ui->comboBox_MaterielID->setCompleter(completerMater);

    QCompleter *completerUnit = new QCompleter(unit.toList(), this);
    ui->comboBox_unit->clear();
    ui->comboBox_unit->addItems(unit.toList());
    ui->comboBox_unit->setEditable(true);
    ui->comboBox_unit->setCompleter(completerUnit);


    ui->comboBox_MaterielID->addItem(ItemNewMater);
    ui->comboBox_customerName->addItem(ItemNewCustom);
    ui->comboBox_unit->addItem(ItemNewUnit);
    ui->comboBox_customerName->setCurrentIndex(-1);
    ui->comboBox_MaterielID->setCurrentIndex(-1);
}

void DialogNewOrder::initOrder(Order order)
{
    if(order.OrderType=="0"){
        ui->comboBox_orderType->setCurrentIndex(0);
    }
    if(order.OrderType=="1"){
        ui->comboBox_orderType->setCurrentIndex(1);
    }
    if(order.OrderType=="2"){
        ui->comboBox_orderType->setCurrentIndex(2);
    }
    ui->comboBox_MaterielID->setCurrentText(order.MaterielID);
    ui->lineEdit_MaterielDes->setText(order.MaterielDes);
    ui->comboBox_unit->setCurrentText(order.Unit);
    ui->spinBox_num->setValue(order.OrderNum);
    ui->comboBox_customerName->setCurrentText(order.CustomName);
    ui->comboBox_CustomBatch->setCurrentText(order.CustomBatch);
    ui->textEdit_custom_note->setText(order.CustomNote);
    curorder = order;
}


void DialogNewOrder::clearUI()
{
    ui->comboBox_orderType->setCurrentIndex(-1);
    ui->comboBox_MaterielID->setCurrentText("");
    ui->lineEdit_MaterielDes->setText("");
    ui->comboBox_unit->setCurrentText("");
    ui->spinBox_num->setValue(0);
    ui->comboBox_customerName->setCurrentText("");
    ui->comboBox_CustomBatch->setCurrentText("");
    ui->textEdit_custom_note->setText("");
}


void DialogNewOrder::setModel(bool isNew)
{
    m_isNewMode = isNew;
    changeModel();
}
void DialogNewOrder::changeModel()
{
    if(m_isNewMode){
        this->setWindowTitle("新增订单");
        ui->pushButton_ok->setText("创建");
        ui->comboBox_customerName->setEnabled(true);
    }else{
        this->setWindowTitle("订单修改");
        ui->pushButton_ok->setText("修改");
        ui->comboBox_customerName->setEnabled(false);
    }
}


void DialogNewOrder::on_pushButton_ok_clicked()
{
    Order  order;
    if(m_isNewMode){
        order.UID = dataCenter::instance()->CurUser().UID ;
        order.UserName = dataCenter::instance()->CurUser().Name;
        order.CustomID = ui->comboBox_customerName->currentData().toString();
        order.CustomName = ui->comboBox_customerName->currentText();
    }else{
        order = curorder;
    }
    order.OrderType = ui->comboBox_orderType->currentData().toString();
    order.MaterielID = ui->comboBox_MaterielID->currentText();
    order.MaterielDes = ui->lineEdit_MaterielDes->text();
    order.Unit = ui->comboBox_unit->currentText();
    order.CustomBatch = ui->comboBox_CustomBatch->currentText();
    order.CustomNote = ui->textEdit_custom_note->toPlainText();
    order.OrderNum =ui->spinBox_num->value();

    if(!checkOrder(order)){
        return;
    }
    QJsonObject para = OrderService::toJsonObject(order);
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

void DialogNewOrder::modOrderCb(Order order,bool ok)
{
    dataCenter::instance()->hideLoadding();
    if(ok){
        dataCenter::instance()->showMessage("修改成功!",4000);
        curorder = order;
        done(123);
    }else{
        dataCenter::instance()->showMessage("修改失败!",4000);
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


    if(!dataCenter::instance()->checkCustomerExist(order.CustomName)){
        QToolTip::showText(ui->comboBox_customerName->mapToGlobal(QPoint(100, 0)), "该客户不存在!");
        return false;
    }

    return true;
}


Order DialogNewOrder::getCurorder() const
{
    return curorder;
}




void DialogNewOrder::on_pushButton_cancel_clicked()
{
    done(-1);
}

void DialogNewOrder::customChange(QString name)
{
    if(name==ItemNewCustom){
        DialogNewCustom cus;
        if(cus.exec()==123){
            ui->comboBox_customerName->blockSignals(true);
            Customer custom = cus.getCurCustom();
            ui->comboBox_customerName->insertItem(ui->comboBox_customerName->count()-1,custom.Name);
            ui->comboBox_customerName->setCurrentIndex(ui->comboBox_customerName->count()-2);
            ui->comboBox_customerName->blockSignals(false);
        }else{
            ui->comboBox_customerName->setCurrentIndex(-1);
        }
    }

}

void DialogNewOrder::materielChange(QString mater)
{

    if(mater==ItemNewMater){
        DialogNewMateriel mater;
        if(mater.exec()==123){
            ui->comboBox_MaterielID->blockSignals(true);
            Materiel ma = mater.getMater();
            ui->comboBox_MaterielID->insertItem(ui->comboBox_MaterielID->count()-1,ma.MaterID,ma.MaterDes);
            ui->comboBox_MaterielID->setCurrentIndex(ui->comboBox_MaterielID->count()-2);
            ui->lineEdit_MaterielDes->setText(ma.MaterDes);
            ui->comboBox_MaterielID->blockSignals(false);
        }else{
            ui->comboBox_MaterielID->setCurrentIndex(-1);
        }
    }else{
        ui->lineEdit_MaterielDes->setText(ui->comboBox_MaterielID->currentData().toString());
    }

}

void DialogNewOrder::unitChange(QString un)
{
    if(un==ItemNewUnit){
        DialogNewUnit unit;
        if(unit.exec()==123){
            ui->comboBox_unit->blockSignals(true);
            QString u = unit.getUnit();
            ui->comboBox_unit->insertItem(ui->comboBox_unit->count()-1,u);
            ui->comboBox_unit->setCurrentIndex(ui->comboBox_unit->count()-2);
            ui->comboBox_unit->blockSignals(false);
        }else{
            ui->comboBox_unit->setCurrentIndex(-1);
        }
    }
}
