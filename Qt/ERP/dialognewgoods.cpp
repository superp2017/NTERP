#include "dialognewgoods.h"
#include "ui_dialognewgoods.h"
#include "datacenter.h"
#include "boost/thread.hpp"
#include <QToolTip>
#include <QCompleter>


DialogNewGoods::DialogNewGoods(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewGoods)
{
    ui->setupUi(this);
    m_module = true;
    initData();
    connect(dataCenter::instance(),SIGNAL(sig_newGoods(Goods,bool)),this,SLOT(newGoodsCb(Goods,bool)));
    connect(dataCenter::instance(),SIGNAL(sig_modGoods(Goods,bool)),this,SLOT(modGoodsCb(Goods,bool)));
}

DialogNewGoods::~DialogNewGoods()
{
    delete ui;
}

void DialogNewGoods::setModule(bool isNew)
{
    m_module = isNew;
    if(m_module){
        this->setWindowTitle("商品创建");
        ui->pushButton_ok->setText("创建");
    }else{
        this->setWindowTitle("商品修改");
        ui->pushButton_ok->setText("修改");
    }
}

void DialogNewGoods::initUI()
{
    ui->comboBox_type->setCurrentIndex(-1);
    ui->lineEdit_color->clear();
    ui->lineEdit_format->clear();
    ui->doubleSpinBox_price->setValue(0);
    ui->lineEdit_name->clear();
    ui->comboBox_supplier->setCurrentIndex(-1);
}

void DialogNewGoods::initData()
{
    initCombox(dataCenter::instance()->pub_Suppliers(),\
               dataCenter::instance()->pub_goodsType(),
               dataCenter::instance()->pub_Units());
}



void DialogNewGoods::initGoods(Goods goods)
{
    ui->lineEdit_name->setText(goods.Name);
    ui->comboBox_type->setCurrentText(goods.Type);
    ui->lineEdit_color->setText(goods.Color);
    ui->lineEdit_format->setText(goods.Format);
    ui->doubleSpinBox_price->setValue(goods.Price/100.0);
    ui->comboBox_supplier->setCurrentText(goods.SupplierName);
    ui->textEdit_note->setText(goods.Note);
    curGoods = goods;
}

void DialogNewGoods::on_pushButton_ok_clicked()
{
    Goods goods = curGoods;
    goods.Name          = ui->lineEdit_name->text().trimmed();\
    if(goods.Name==""){
        QToolTip::showText(ui->lineEdit_name->mapToGlobal(QPoint(100, 0)), "商品名称不能为空!");
        return;
    }
    goods.Type          = ui->comboBox_type->currentText();
    if(goods.Type==""){
        QToolTip::showText(ui->comboBox_type->mapToGlobal(QPoint(100, 0)), "商品分类不能为空!");
        return ;
    }
    goods.Format        = ui->lineEdit_format->text();
    goods.Color         = ui->lineEdit_color->text();
    goods.Price         = (int)(ui->doubleSpinBox_price->value()*100);
    goods.Note          = ui->textEdit_note->toPlainText();
    goods.SupplierName  = ui->comboBox_supplier->currentText();
    goods.Num           = ui->spinBox_num->value();
    if(goods.SupplierName!=""){
        if(!dataCenter::instance()->pub_checkSuppliser(goods.SupplierName)){
            QToolTip::showText(ui->comboBox_supplier->mapToGlobal(QPoint(100, 0)), "供应商不存在!");
            return ;
        }
        goods.SID       = ui->comboBox_supplier->currentData().toString();
    }
    QJsonObject para = GoodsService::toJsonObject(goods);
    if(m_module){
        boost::thread(boost::bind(&dataCenter::net_newGoods,dataCenter::instance(),para)).detach();
    }else{
        boost::thread(boost::bind(&dataCenter::net_modGoods,dataCenter::instance(),para)).detach();
    }
    dataCenter::instance()->pub_showLoadding("正在网络请求...",5000,Qt::black);
}

void DialogNewGoods::on_pushButton_exit_clicked()
{
    this->done(-1);
}


void DialogNewGoods::initCombox(QVector<Supplier> sup, QVector<QString> type, QVector<QString> units)
{
    ui->comboBox_supplier->clear();
    QStringList suplist;
    for(Supplier s:sup){
        suplist<<s.Name;
        ui->comboBox_supplier->addItem(s.Name,s.SID);
    }
    QCompleter *supCompleter = new QCompleter(suplist, this);
    ui->comboBox_supplier->setEditable(true);
    ui->comboBox_supplier->setCompleter(supCompleter);

    ui->comboBox_type->clear();
    ui->comboBox_type->addItems(type.toList());
    ui->comboBox_type->setEditable(true);
    QCompleter *typeCompleter = new QCompleter(type.toList(), this);
    ui->comboBox_type->setCompleter(typeCompleter);

    ui->comboBox_unit->clear();
    ui->comboBox_unit->addItems(units.toList());
    ui->comboBox_unit->setEditable(true);
    QCompleter *unitCompleter = new QCompleter(units.toList(), this);
    ui->comboBox_unit->setCompleter(unitCompleter);
}

Goods DialogNewGoods::getCurGoods() const
{
    return curGoods;
}

void DialogNewGoods::newGoodsCb(Goods goods,bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        dataCenter::instance()->pub_showMessage("修改成功!",3000);
        curGoods = goods;
        this->done(123);
    }else{
        dataCenter::instance()->pub_showMessage("修改失败!",3000);
    }
}

void DialogNewGoods::modGoodsCb(Goods goods,bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        dataCenter::instance()->pub_showMessage("创建成功!",3000);
        curGoods = goods;
        this->done(123);
    }else{
        dataCenter::instance()->pub_showMessage("创建失败!",3000);
    }
}

