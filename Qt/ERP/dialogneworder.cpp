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
#include <QRegExpValidator>


DialogNewOrder::DialogNewOrder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewOrder),
    m_isNewMode(true)
{
    ui->setupUi(this);

    initData();

    connect(dataCenter::instance(),SIGNAL(sig_newOrder(Order,bool)),this,SLOT(newOrderCb(Order,bool)));
    connect(dataCenter::instance(),SIGNAL(sig_modOrder(Order,bool)),this,SLOT(modOrderCb(Order,bool)));

    connect(ui->comboBox_customerName,SIGNAL(currentIndexChanged(int)),this,SLOT(customChange(int)));
    connect(ui->comboBox_unit,SIGNAL(currentIndexChanged(int)),this,SLOT(unitChange(int)));

    QRegExp regx("[a-zA-Z0-9]+$");
    QValidator *validator = new QRegExpValidator(regx, this );
    ui->lineEdit_custombatch->setValidator(validator);

    changeModel();
}

DialogNewOrder::~DialogNewOrder()
{
    delete ui;
}
void DialogNewOrder::initData()
{
    initCombox(dataCenter::instance()->pub_Customers(),\
               dataCenter::instance()->pub_Batchs(),\
               dataCenter::instance()->pub_Units());
}

void DialogNewOrder::initCombox(QVector<Customer> custom, QSet<QString> batch, QVector<QString> unit)

{
    ui->comboBox_customerName->blockSignals(true);
    ui->comboBox_unit->blockSignals(true);
    ui->comboBox_orderType->clear();
    ui->comboBox_orderType->addItem("普通订单","0");
    ui->comboBox_orderType->addItem("试样订单","1");
    ui->comboBox_orderType->addItem("返工订单","2");

    ui->comboBox_factory->clear();
    ui->comboBox_factory->addItem("涂覆分厂","01");
    ui->comboBox_factory->addItem("滚镀分厂","02");
    ui->comboBox_factory->addItem("挂镀分厂","03");

    ui->comboBox_customerName->clear();
    ui->comboBox_customerName->blockSignals(true);
    QStringList customlist;
    for(Customer c:custom){
        customlist<<c.Name;
        ui->comboBox_customerName->addItem(c.Name,c.CID);
    }

    ui->comboBox_customerName->addItem(ItemNewCustom);
    ui->comboBox_customerName->setCurrentIndex(-1);

    QCompleter *completerBatch = new QCompleter(batch.toList(), this);
    ui->lineEdit_custombatch->setCompleter(completerBatch);

    ui->comboBox_unit->clear();
    QCompleter *completerUnit = new QCompleter(unit.toList(), this);

    ui->comboBox_unit->addItems(unit.toList());
    ui->comboBox_unit->setEditable(true);
    ui->comboBox_unit->setCompleter(completerUnit);

    ui->comboBox_unit->addItem(ItemNewUnit);
    ui->comboBox_unit->setCurrentIndex(-1);

    ui->comboBox_customerName->blockSignals(false);
    ui->comboBox_unit->blockSignals(false);
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
    ui->lineEdit_MaterielDes->setText(order.MaterielDes);
    ui->comboBox_unit->setCurrentText(order.Unit);
    ui->spinBox_num->setValue(order.OrderNum);
    ui->comboBox_customerName->setCurrentText(order.CustomName);
    ui->lineEdit_custombatch->setText(order.CustomBatch);
    ui->textEdit_custom_note->setText(order.CustomNote);
    curorder = order;
}


void DialogNewOrder::clearUI()
{
    ui->comboBox_orderType->setCurrentIndex(0);
    ui->lineEdit_MaterielDes->setText("");
    ui->comboBox_unit->setCurrentIndex(-1);
    ui->spinBox_num->setValue(0);
    ui->comboBox_customerName->setCurrentIndex(-1);
    ui->lineEdit_custombatch->setText("");
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
        order.UID = dataCenter::instance()->pub_CurUser().UID ;
        order.UserName = dataCenter::instance()->pub_CurUser().Name;
        order.CustomID = ui->comboBox_customerName->currentData().toString();
        order.CustomName = ui->comboBox_customerName->currentText();
    }else{
        order = curorder;
    }
    order.Factory = ui->comboBox_factory->currentText();
    order.FactoryNumber = ui->comboBox_factory->currentData().toString();
    order.OrderType = ui->comboBox_orderType->currentData().toString();
    order.MaterielDes = ui->lineEdit_MaterielDes->toPlainText();
    order.Unit = ui->comboBox_unit->currentText();
    order.CustomBatch = ui->lineEdit_custombatch->text();
    order.CustomNote = ui->textEdit_custom_note->toPlainText();
    order.OrderNum =ui->spinBox_num->value();

    if(!checkOrder(order)){
        return;
    }
    QJsonObject para = OrderService::toJsonObject(order);
    if(m_isNewMode){
        boost::thread t(boost::bind(&dataCenter::net_newOrder,dataCenter::instance(),para));
        t.detach();
    }else{
        boost::thread t(boost::bind(&dataCenter::net_modOrder,dataCenter::instance(),para));
        t.detach();
    }
    dataCenter::instance()->pub_showLoadding("正在网络请求...",5000,Qt::black);
}

void DialogNewOrder::newOrderCb(Order order,bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        dataCenter::instance()->pub_showMessage("下单成功!",4000);
        curorder = order;
        done(123);
    }else{
        dataCenter::instance()->pub_showMessage("下单失败!",4000);
        curorder = order;
        done(0);
    }
}

void DialogNewOrder::modOrderCb(Order order,bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        dataCenter::instance()->pub_showMessage("修改成功!",4000);
        curorder = order;
        done(123);
    }else{
        dataCenter::instance()->pub_showMessage("修改失败!",4000);
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

    if(!dataCenter::instance()->pub_checkCustomerExist(order.CustomName)){
        QToolTip::showText(ui->comboBox_customerName->mapToGlobal(QPoint(100, 0)), "该客户不存在!");
        return false;
    }

    if(order.MaterielDes==""){
        QToolTip::showText(ui->lineEdit_MaterielDes->mapToGlobal(QPoint(100, 0)), "物料描述不能为空!");
        return false;
    }

    if(order.Unit==""){
        QToolTip::showText(ui->comboBox_unit->mapToGlobal(QPoint(100, 0)), "计量单位不能为空!");
        return false;
    }

    if(!dataCenter::instance()->pub_checkUnitExist(order.Unit)){
        QToolTip::showText(ui->comboBox_unit->mapToGlobal(QPoint(100, 0)), "计量单位不存在!");
        return false;
    }

    if(order.OrderNum<=0){
        QToolTip::showText(ui->spinBox_num->mapToGlobal(QPoint(100, 0)), "订单数量填写不正确!");
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

void DialogNewOrder::on_pushButton_edit_des_clicked()
{
    DialogNewMateriel mater;
    if(mater.exec()==123){
        Materiel ma = mater.getMater();
        ui->lineEdit_MaterielDes->setText(ma.MaterDes);
    }
}

void DialogNewOrder::customChange(int index)
{
    if (index==-1) return;
    if(ui->comboBox_customerName->currentText()==ItemNewCustom){
        DialogNewCustom cus;
        cus.setMode(true);
        if(cus.exec()==123){
            ui->comboBox_customerName->blockSignals(true);
            Customer custom = cus.getCurCustom();
            ui->comboBox_customerName->insertItem(ui->comboBox_customerName->count()-1,custom.Name,custom.CID);
            ui->comboBox_customerName->setCurrentIndex(ui->comboBox_customerName->count()-2);
            ui->comboBox_customerName->blockSignals(false);
        }else{
            ui->comboBox_customerName->setCurrentIndex(-1);
        }
    }

}



void DialogNewOrder::unitChange(int index)
{
    if (index==-1) return;
    if(ui->comboBox_unit->currentText()==ItemNewUnit){
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


