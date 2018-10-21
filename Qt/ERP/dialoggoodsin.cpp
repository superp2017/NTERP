#include "dialoggoodsin.h"
#include "ui_dialoggoodsin.h"
#include "datacenter.h"
#include "boost/thread.hpp"
#include <QCompleter>
#include <QToolTip>

DialogGoodsIn::DialogGoodsIn(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogGoodsIn)
{
    ui->setupUi(this);
    connect(dataCenter::instance(),SIGNAL(sig_inGoods(Goods,bool)),this,SLOT(intGoodsCb(Goods,bool)));
    initCombox();
    connect(ui->comboBox_goodID,SIGNAL(currentIndexChanged(QString)),this,SLOT(GoodsChange()));
}

void DialogGoodsIn::initCombox()
{
    ui->comboBox_goodID->blockSignals(true);

    QStringList ids;
    for(Goods g:dataCenter::instance()->pub_goods()){
        ids <<g.ID;
    }
    QCompleter *complete_ID = new QCompleter(ids, this);
    ui->comboBox_goodID->clear();
    ui->comboBox_goodID->addItems(ids);
    ui->comboBox_goodID->setEditable(true);
    ui->comboBox_goodID->setCompleter(complete_ID);

    ui->comboBox_goodID->blockSignals(false);
}

void DialogGoodsIn::clearUI()
{

}

DialogGoodsIn::~DialogGoodsIn()
{
    delete ui;
}

void DialogGoodsIn::initGoods(Goods goods,bool isNew)
{
    ui->comboBox_goodID->setEnabled(!isNew);
    curgoods = goods;
    ui->comboBox_goodID->setCurrentText(goods.ID);
    ui->lineEdit_name->setText(goods.Name);
    ui->lineEdit_type->setText(goods.Type);
    ui->lineEdit_format->setText(goods.Format);
    ui->doubleSpinBox_Num->setValue(0);
    ui->lineEdit_supplier->setText(goods.SupplierName);
    ui->lineEdit_unit->setText(goods.Unit);
}

void DialogGoodsIn::on_pushButton_ok_clicked()
{
    QString goodsID = ui->comboBox_goodID->currentText();
    if(goodsID.isEmpty()){
        QToolTip::showText(ui->comboBox_goodID->mapToGlobal(QPoint(100, 0)), "商品编号不能为空!");
        return;
    }
    if(curgoods.ID.isEmpty()){
        QToolTip::showText(ui->comboBox_goodID->mapToGlobal(QPoint(100, 0)), "未找到对应的商品!");
        return;
    }
    double num = ui->doubleSpinBox_Num->value();
    QJsonObject obj;
    obj.insert("ID",goodsID);
    obj.insert("Num",num);
    boost::thread(boost::bind(&dataCenter::net_addOutGoodsNum,dataCenter::instance(),obj)).detach();
    dataCenter::instance()->pub_showLoadding("正在通讯...",10000);

}

void DialogGoodsIn::on_pushButton_exit_clicked()
{
    done(-1);
}

void DialogGoodsIn::intGoodsCb(Goods g, bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        curgoods = g;
        dataCenter::instance()->pub_showMessage("商品入库成功\n",3000);
        done(123);
    }else{
        dataCenter::instance()->pub_showMessage("商品入库失败\n",3000);
        done(-1);
    }
}

void DialogGoodsIn::GoodsChange()
{
    curgoods.ID="";
    QString goodsID = ui->comboBox_goodID->currentText();
    if(!goodsID.isEmpty()){
        bool ok = false;
        Goods g = dataCenter::instance()->pub_getGoods(goodsID,ok);
        if(ok)
            initGoods(g);
        else{
            ui->lineEdit_name->setText("");
            ui->lineEdit_type->setText("");
            ui->lineEdit_format->setText("");
            ui->doubleSpinBox_Num->setValue(0);
            ui->lineEdit_supplier->setText("");
            ui->lineEdit_unit->setText("");
        }
    } else{
        ui->lineEdit_name->setText("");
        ui->lineEdit_type->setText("");
        ui->lineEdit_format->setText("");
        ui->doubleSpinBox_Num->setValue(0);
        ui->lineEdit_supplier->setText("");
        ui->lineEdit_unit->setText("");
    }
}



Goods DialogGoodsIn::getCurgoods() const
{
    return curgoods;
}
