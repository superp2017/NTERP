#include "dialogneworder.h"
#include "ui_dialogneworder.h"
#include "datacenter.h"
#include <QCompleter>
#include <QToolTip>


DialogNewOrder::DialogNewOrder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewOrder)
{
    ui->setupUi(this);
    curorder = NULL;
    initCombox(dataCenter::instance()->Customers(),\
               dataCenter::instance()->Batchs(),\
               dataCenter::instance()->Materiels(),\
               dataCenter::instance()->Units());
}

DialogNewOrder::~DialogNewOrder()
{
    delete ui;
}

Order *DialogNewOrder::getOrder()
{
    return curorder;
}

void DialogNewOrder::test()
{

}




//QString           UID;            //用户id
//QString           UserName;       //用户姓名
//QString           OrderID;        //订单id
//QString           MaterielID;     //材料id
//QString           MaterielDes;    //材料描述
//QString           Unit;           //单位
//QString           CustomID;       //客户ID
//QString           CustomName;     //客户姓名
//QString           CustomBatch;    //客户批次
//QString           CustomNote;     //客户备注
//QString           ProduceID;      //生产编号
//QString           SuccessTime;    //出货时间
//QString           CreatTime;      //创建时间
//OderFlow          Current;        //当前状态
//QVector<OderFlow> Flow;           //订单流程
//int               OrderNum;       //订单数量
//int               Money;          //价格



void DialogNewOrder::on_pushButton_ok_clicked()
{
    curorder = new Order();
    curorder->OrderType = ui->comboBox_orderType->currentData().toString();
    curorder->UID = dataCenter::instance()->CurUser()->UID ;
    curorder->UserName = dataCenter::instance()->CurUser()->Name;
    curorder->MaterielID = ui->comboBox_MaterielID->currentText();
    curorder->MaterielDes = ui->lineEdit_MaterielDes->text();
    curorder->Unit = ui->comboBox_unit->currentText();
    curorder->CustomID = ui->comboBox_customerName->currentData().toString();
    curorder->CustomName = ui->comboBox_customerName->currentText();
    curorder->CustomBatch = ui->comboBox_CustomBatch->currentText();
    curorder->CustomNote = ui->textEdit_custom_note->toPlainText();
    curorder->OrderNum =ui->spinBox_num->value();

    if(!checkOrder()){
        return;
    }

    dataCenter::instance()->showLoadding("正在网络请求...",4000,Qt::black);
}

void DialogNewOrder::newOrderCb(bool ok)
{
    dataCenter::instance()->hideLoadding();
    if(ok){
        dataCenter::instance()->showMessage("下单成功!",2000);
        done(123);
    }else{
        dataCenter::instance()->showMessage("下单失败!",2000);
        done(0);
    }
}


bool DialogNewOrder::checkOrder()
{
    if(curorder==NULL){
        return false;
    }
    if(curorder->CustomName==""){
        QToolTip::showText(ui->comboBox_customerName->mapToGlobal(QPoint(100, 0)), "客户不能为空!");
        return false;
    }
    if(curorder->MaterielID==""){
        QToolTip::showText(ui->comboBox_MaterielID->mapToGlobal(QPoint(100, 0)), "物料编号不能为空!");
        return false;
    }

    if(curorder->MaterielDes==""){
        QToolTip::showText(ui->lineEdit_MaterielDes->mapToGlobal(QPoint(100, 0)), "物料描述不能为空!");
        return false;
    }

    if(curorder->Unit==""){
        QToolTip::showText(ui->comboBox_unit->mapToGlobal(QPoint(100, 0)), "单位不能为空!");
        return false;
    }

    if(curorder->OrderNum<=0){
        QToolTip::showText(ui->spinBox_num->mapToGlobal(QPoint(100, 0)), "订单数量填写不正确!");
        return false;
    }

    if(dataCenter::instance()->checkMaterielID(curorder->MaterielID)){
        QToolTip::showText(ui->comboBox_MaterielID->mapToGlobal(QPoint(100, 0)), "物料编号已经存在!");
        return false;
    }

    return true;
}





void DialogNewOrder::initCombox(QVector<Customer*> custom, QVector<QString> batch, \
                                QVector<Materiel *> materID, QVector<QString> unit)

{
    ui->comboBox_orderType->clear();
    ui->comboBox_orderType->addItem("普通订单","0");
    ui->comboBox_orderType->addItem("批量订单","1");
    ui->comboBox_orderType->addItem("试样订单","1");

    ui->comboBox_customerName->clear();
    QStringList customlist;
    for(Customer* c:custom){
        customlist<<c->Name;
        ui->comboBox_customerName->addItem(c->Name,c->CID);
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
    for(Materiel* c:materID){
        materlist<<c->MaterDes;
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
