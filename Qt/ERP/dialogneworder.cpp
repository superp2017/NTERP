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
    m_isNewMode(true)
{
    ui->setupUi(this);

    initCombox(dataCenter::instance()->pub_Batchs(),
               dataCenter::instance()->pub_Materiels());

    connect(dataCenter::instance(),SIGNAL(sig_newOrder(Order,bool)),this,SLOT(newOrderCb(Order,bool)));
    connect(dataCenter::instance(),SIGNAL(sig_modOrder(Order,bool)),this,SLOT(modOrderCb(Order,bool)));


    connect(ui->comboBox_mater_number,SIGNAL(currentIndexChanged(int)),this,SLOT(materielIDChange(int)));

//    QRegExp regx("[a-zA-Z0-9-~!@#$%^&*\(\)_+=;:,.<>]+$");
//    QValidator *validator = new QRegExpValidator(regx, this );
//    ui->lineEdit_custombatch->setValidator(validator);

/////神州专用//////////////////
//    ui->lineEdit_fatory->setHidden(true);
//    ui->lineEdit_productline->setHidden(true);
//    ui->label_fac_name->setHidden(true);
//    ui->label_line_name->setHidden(true);
    /////神州专用//////////////////
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

    ui->comboBox_mater_number->clear();
    QStringList materlist;
    QSet<QString> mlist;
    for(Materiel ma:mater){
        m_company_mater[ma.ComponentSolid].push_back(ma.CustomName);
        if(!mlist.contains(ma.ComponentSolid))
            materlist<<ma.ComponentSolid;
        mlist.insert(ma.ComponentSolid);
    }
    ui->comboBox_mater_number->addItems(materlist);
    ui->comboBox_mater_number->setEditable(true);
    QCompleter *completermater = new QCompleter(materlist, this);
    ui->comboBox_mater_number->setCompleter(completermater);
    ui->comboBox_mater_number->setCurrentIndex(-1);
    ui->comboBox_mater_number->blockSignals(false);

    QCompleter *completerBatch = new QCompleter(batch.toList(), this);
    ui->lineEdit_custombatch->setCompleter(completerBatch);

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
    ui->comboBox_mater_number->setCurrentText(order.ComponentSolid);
    materielIDChange(0);

    ui->doubleSpinBox_num->setValue(order.OrderNum);
    ui->doubleSpinBox_num_confirm->setValue(order.OrderNum);
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
        ui->doubleSpinBox_num->setValue(1);
        ui->doubleSpinBox_num_confirm->setValue(1);
    }else{
        this->setWindowTitle("订单修改");
        ui->pushButton_ok->setText("修改");
        ui->comboBox_orderType->setEnabled(false);
        ui->comboBox_mater_number->setEnabled(false);
    }
}

void DialogNewOrder::clearCurMater()
{
    curMater.MaterDes="";
    curMater.MaterID="";
    curMater.Factory= "";
    curMater.ProductionLine="";
    curMater.Unit="";
    curMater.CustomName="";
    curMater.CID="";
    curMater.Money=0;
}

void DialogNewOrder::setCurMater()
{
    ui->lineEdit_fatory->setText(curMater.Factory);
    ui->lineEdit_MaterielDes->setText(curMater.MaterDes);
    ui->lineEdit_productline->setText(curMater.ProductionLine);
    ui->lineEdit_unit->setText(curMater.Unit);
}


void DialogNewOrder::on_pushButton_ok_clicked()
{
    Order  order ;
    if(m_isNewMode){
        order.UID           = dataCenter::instance()->pub_CurUser().UID ;
        order.UserName      = dataCenter::instance()->pub_CurUser().Name;
        order.ProduceNum    = 0;
        order.SuccessNum    = 0;
        order.PrintNum      = 0;
    }else{
        order = curorder;
        order.ProduceNum        = curorder.ProduceNum;
        order.SuccessNum        = curorder.SuccessNum;
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
    order.OrderNum          = ui->doubleSpinBox_num->value();

    if(!checkOrder(order,!m_isNewMode)){
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


bool DialogNewOrder::checkOrder(Order order,bool ismod)
{
    if(order.MaterielID==""||order.MaterielDes==""){
        QToolTip::showText(ui->comboBox_mater_number->mapToGlobal(QPoint(100, 0)), "物料编号不能为空!");
        return false;
    }

    if(order.OrderNum<=0){
        QToolTip::showText(ui->doubleSpinBox_num->mapToGlobal(QPoint(100, 0)), "订单数量填写不正确!");
        return false;
    }
    if(ismod){
        if(order.OrderNum<order.ProduceNum){
            QToolTip::showText(ui->doubleSpinBox_num->mapToGlobal(QPoint(100, 0)), "修改后的订单数量不能少于已经成品的数量!");
            return false;
        }if(order.OrderNum<order.SuccessNum){
            QToolTip::showText(ui->doubleSpinBox_num->mapToGlobal(QPoint(100, 0)), "修改后的订单数量不能少于已经出库的数量!");
            return false;
        }
//        if(order.OrderNum<order.ProduceNum+order.SuccessNum){
//            QToolTip::showText(ui->doubleSpinBox_num->mapToGlobal(QPoint(100, 0)), "修改后的订单数量不能少于已经成品和已经出库的数量之和!");
//            return false;
//        }
    }
    double confirm_num = ui->doubleSpinBox_num_confirm->value();
    if (order.OrderNum!=confirm_num){
        QToolTip::showText(ui->doubleSpinBox_num_confirm->mapToGlobal(QPoint(100, 0)), "两次输入的订单数量不一样!");
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

void DialogNewOrder::companyNameChange(int index)
{
    clearCurMater();
    QString CName = ui->comboBox_company_name->currentText();
    QString MaterID=ui->comboBox_mater_number->currentText();
    QVector<Materiel> ls = dataCenter::instance()->pub_getMaterielFromSolidID(MaterID);
    for(Materiel ma:ls){
        if(ma.CustomName==CName){
            curMater = ma;
            break;
        }
    }
    setCurMater();
}

void DialogNewOrder::materielIDChange(int index)
{
    if(m_company_mater.count(ui->comboBox_mater_number->currentText())==0){
            clearCurMater();
            setCurMater();

            ui->comboBox_company_name->blockSignals(true);
            ui->comboBox_company_name->clear();
            ui->comboBox_company_name->blockSignals(false);
            ui->comboBox_company_name->setEnabled(false);
            return;
        }
        QVector<QString> ls = m_company_mater[ui->comboBox_mater_number->currentText()];
        ui->comboBox_company_name->setEnabled(ls.size()>0);
        ui->comboBox_company_name->blockSignals(true);
        ui->comboBox_company_name->clear();
        for(QString m:ls){
            ui->comboBox_company_name->addItem(m);
        }
        ui->comboBox_company_name->blockSignals(false);
        companyNameChange(0);
}








