#include "dialoginoutgoods.h"
#include "ui_dialoginoutgoods.h"
#include "datacenter.h"
#include "boost/thread.hpp"

DialogInOutGoods::DialogInOutGoods(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogInOutGoods)
{
    ui->setupUi(this);
    isIn = true;
    connect(dataCenter::instance(),SIGNAL(sig_inoutGoods(Goods,bool)),this,SLOT(inoutGoodsCb(Goods,bool)));
}

DialogInOutGoods::~DialogInOutGoods()
{
    delete ui;
}

void DialogInOutGoods::setModule(bool in)
{
    isIn = in;
    if(in){
        ui->label_inout->setText("入库数量");
        ui->pushButton_ok->setText("入库");
        this->setWindowTitle("货品入库");
    }else{
        ui->label_inout->setText("出库数量");
        ui->pushButton_ok->setText("出库");
        this->setWindowTitle("货品出库");
    }
}

void DialogInOutGoods::initGoods(Goods goods)
{
    curgoods = goods;
    if(isIn){
        ui->spinBox->setMaximum(999999);
    }else{
        ui->spinBox->setMaximum(goods.Num);
    }
    ui->lineEdit_name->setText(goods.Name);
    ui->lineEdit_format->setText(goods.Format);
    ui->lineEdit_id->setText(goods.ID);
    ui->lineEdit_num->setText(QString("%1").arg(goods.Num));
    ui->lineEdit_type->setText(goods.Type);
}

void DialogInOutGoods::on_pushButton_ok_clicked()
{
    int num = ui->spinBox->value();
    QJsonObject obj;
    obj.insert("ID",curgoods.ID);
    obj.insert("Num",num);
    obj.insert("IsIn",isIn);
    boost::thread(boost::bind(&dataCenter::net_inOutGoods,dataCenter::instance(),obj)).detach();
    dataCenter::instance()->pub_showLoadding("正在通讯...",10000);
}


void DialogInOutGoods::inoutGoodsCb(Goods goods, bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        curgoods = goods;
        dataCenter::instance()->pub_showMessage("库存变动成功\n",3000);
        done(123);
    }else{
        dataCenter::instance()->pub_showMessage("库存变动失败\n",3000);
        done(-1);
    }
}

Goods DialogInOutGoods::getCurgoods() const
{
    return curgoods;
}


void DialogInOutGoods::on_pushButton_exit_clicked()
{
    done(-1);
}

