#include "dialogneworder.h"
#include "ui_dialogneworder.h"
#include "datacenter.h"
#include <QCompleter>
#include <QToolTip>
#include "orderservice.h"
#include "dialognewcustom.h"
#include "service_global.h"
#include "dialognewunit.h"
#include "boost/thread.hpp"
#include <QRegExpValidator>


DialogNewOrder::DialogNewOrder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewOrder),
    m_isNewMode(true),
    IsHisMode(false)
{
    ui->setupUi(this);



    initCombox(dataCenter::instance()->pub_Batchs(),
               dataCenter::instance()->pub_Materiels());

    connect(dataCenter::instance(),SIGNAL(sig_newOrder(Order,bool)),this,SLOT(newOrderCb(Order,bool)));
    connect(dataCenter::instance(),SIGNAL(sig_modOrder(Order,bool)),this,SLOT(modOrderCb(Order,bool)));


    connect(ui->comboBox_mater_number,SIGNAL(currentIndexChanged(int)),this,SLOT(materielIDChange(int)));

    QRegExp regx("[a-zA-Z0-9]+$");
    QValidator *validator = new QRegExpValidator(regx, this );
    ui->lineEdit_custombatch->setValidator(validator);
}

DialogNewOrder::~DialogNewOrder()
{
    delete ui;
}

void DialogNewOrder::initCombox(QSet<QString> batch,QVector<Materiel>mater)

{

    ui->comboBox_orderType->addItem("普通订单","1");
    ui->comboBox_orderType->addItem("试样订单","2");
    ui->comboBox_orderType->addItem("返工订单","3");

    ui->comboBox_mater_number->blockSignals(true);
    QCompleter *completerBatch = new QCompleter(batch.toList(), this);
    ui->lineEdit_custombatch->setCompleter(completerBatch);

    ui->comboBox_mater_number->clear();
    QStringList materlist;
    for(Materiel ma:mater){
        materlist<<ma.MaterID;
    }
    ui->comboBox_mater_number->addItems(materlist);
    ui->comboBox_mater_number->setEditable(true);
    QCompleter *completermater = new QCompleter(materlist, this);
    ui->comboBox_mater_number->setCompleter(completermater);
    ui->comboBox_mater_number->setCurrentIndex(-1);
    ui->comboBox_mater_number->blockSignals(false);
}

void DialogNewOrder::initOrder(Order order)
{
    if(order.OrderType=="1"){
        ui->comboBox_orderType->setCurrentIndex(0);
    }
    if(order.OrderType=="2"){
        ui->comboBox_orderType->setCurrentIndex(1);
    }
    if(order.OrderType=="3"){
        ui->comboBox_orderType->setCurrentIndex(2);
    }
    ui->comboBox_mater_number->setCurrentText(order.MaterielID);
    ui->lineEdit_fatory->setText(order.Factory);
    ui->lineEdit_productline->setText(order.ProductionLine);
    ui->lineEdit_MaterielDes->setText(order.MaterielDes);
    ui->lineEdit_unit->setText(order.Unit);
    ui->spinBox_num->setValue(order.OrderNum);
    ui->lineEdit_cumerName->setText(order.CustomName);
    ui->lineEdit_custombatch->setText(order.CustomBatch);
    ui->textEdit_custom_note->setText(order.CustomNote);

    curorder = order;
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
        ui->comboBox_orderType->setEnabled(true);
    }else{
        this->setWindowTitle("订单修改");
        ui->pushButton_ok->setText("修改");
        ui->comboBox_orderType->setEnabled(false);
    }
}


void DialogNewOrder::on_pushButton_ok_clicked()
{
    Order  order ;
    if(m_isNewMode){
        order.UID           = dataCenter::instance()->pub_CurUser().UID ;
        order.UserName      = dataCenter::instance()->pub_CurUser().Name;
    }else{
        order = curorder;
    }

    order.CustomID          = curMater.CID;
    order.CustomName        = curMater.CustomName;
    order.Factory           = curMater.Factory;
    order.FactoryNumber     = curMater.FactoryNumber;
    order.ProductionLine    = curMater.ProductionLine;
    order.MaterielDes       = curMater.MaterDes;
    order.MaterielID        = curMater.MaterID;
    order.Plating           = curMater.Plating;
    order.Friction          = curMater.Friction;
    order.Thickness         = curMater.Thickness;
    order.Salt              = curMater.Salt;
    order.ComponentSolid    = curMater.ComponentSolid;
    order.ComponentFormat   = curMater.ComponentFormat;
    order.Money             = curMater.Money;
    order.Unit              = curMater.Unit;

    order.OrderType         = ui->comboBox_orderType->currentData().toString();
    order.CustomBatch       = ui->lineEdit_custombatch->text();
    order.CustomNote        = ui->textEdit_custom_note->toPlainText();
    order.OrderNum          = ui->spinBox_num->value()*100;

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
    if(order.OrderNum<=0){
        QToolTip::showText(ui->spinBox_num->mapToGlobal(QPoint(100, 0)), "订单数量填写不正确!");
        return false;
    }

    if(order.OrderNum<order.ProduceNum||order.OrderNum<order.SuccessNum){
        QToolTip::showText(ui->spinBox_num->mapToGlobal(QPoint(100, 0)), "订单数量不能少于已经成品或者已经出库的数量!");
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


void DialogNewOrder::materielIDChange(int index)
{
    if(index==-1)
        return;
    QString id = ui->comboBox_mater_number->currentText().trimmed();
    if(id.isEmpty())
        return;
    bool ok = false;
    Materiel ma = dataCenter::instance()->pub_getMateriel(id,ok);
    if(!ok)
        return;
    ui->lineEdit_fatory->setText(ma.Factory);
    ui->lineEdit_MaterielDes->setText(ma.MaterDes);
    ui->lineEdit_productline->setText(ma.ProductionLine);
    ui->lineEdit_unit->setText(ma.Unit);
    ui->lineEdit_cumerName->setText(ma.CustomName);
    curMater = ma;

}







