#include "dialogoutgoods.h"
#include "ui_dialogoutgoods.h"
#include "datacenter.h"
#include <QCompleter>
#include <QToolTip>
#include "boost/thread.hpp"
#include <QJsonObject>
#include "goodsoutrecordservice.h"

DialogOutGoods::DialogOutGoods(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogOutGoods)
{
    ui->setupUi(this);
    initCombox();
    connect(ui->comboBox_goods_id,SIGNAL(currentIndexChanged(QString)),this,SLOT(GoodIDChange()));
    ui->textEdit_des->setEnabled(false);
    ui->lineEdit_type->setEnabled(false);
    ui->comboBox_unit->setEditable(false);
    connect(dataCenter::instance(),SIGNAL(sig_newGoodsRecord(GoodsOutRecord,bool)),this,SLOT(NewOutCb(GoodsOutRecord,bool)));
}

DialogOutGoods::~DialogOutGoods()
{
    delete ui;
}

void DialogOutGoods::initCombox()
{
    ui->comboBox_goods_id->blockSignals(true);
    QStringList id_list;
    for(Goods g:dataCenter::instance()->pub_goods()){
        id_list<<g.ID;
    }
    QCompleter *completer_id= new QCompleter(id_list, this);
    ui->comboBox_goods_id->clear();
    ui->comboBox_goods_id->setEditable(true);
    ui->comboBox_goods_id->addItems(id_list);
    ui->comboBox_goods_id->setCompleter(completer_id);


    ui->comboBox_factory->addItem("涂覆分厂","01");
    ui->comboBox_factory->addItem("滚镀分厂","02");
    ui->comboBox_factory->addItem("挂镀分厂","03");

    QStringList depart_list;
    for(QString de:dataCenter::instance()->pub_getDepartments()){
        depart_list<<de;
    }
    depart_list<<"涂覆线"<<"滚镀锌线"<<"磷化线"<<"镀铜镀锡线"<<"挂镀锌线"<<"挂镀锌镍线";
    QCompleter *completer_de= new QCompleter(depart_list, this);
    ui->comboBox_department->clear();
    ui->comboBox_department->setEditable(true);
    ui->comboBox_department->addItems(depart_list);
    ui->comboBox_department->setCompleter(completer_de);

    QStringList user_list;
    ui->comboBox_userName->clear();
    for(User u:dataCenter::instance()->pub_employees()){
        ui->comboBox_userName->addItem(u.Name,u.UID);
        user_list<<u.Name;
    }
    ui->comboBox_userName->setEditable(true);
    QCompleter *completer_user= new QCompleter(user_list, this);
    ui->comboBox_userName->setCompleter(completer_user);


    QStringList unit_list;
    for(QString g:dataCenter::instance()->pub_Units()){
        unit_list<<g;
    }
    QCompleter *completer_unit= new QCompleter(unit_list, this);
    ui->comboBox_unit->clear();
    ui->comboBox_unit->setEditable(true);
    ui->comboBox_unit->addItems(unit_list);
    ui->comboBox_unit->setCompleter(completer_unit);

    QStringList store_list;
    store_list<<"总仓库"<<"涂覆仓库"<<"滚镀仓库"<<"挂镀仓库";
    QCompleter *completer_store= new QCompleter(store_list, this);
    ui->comboBox_storeName->clear();
    ui->comboBox_storeName->setEditable(true);
    ui->comboBox_storeName->addItems(store_list);
    ui->comboBox_storeName->setCompleter(completer_store);


    ui->comboBox_goods_id->blockSignals(false);
}

void DialogOutGoods::on_pushButton_ok_clicked()
{
    cur_record.GoodsID      = ui->comboBox_goods_id->currentText();
    cur_record.GoodsName    = ui->textEdit_des->toPlainText();
    cur_record.Type         = ui->lineEdit_type->text();
    cur_record.Factory      = ui->comboBox_factory->currentText();
    cur_record.Department   = ui->comboBox_department->currentText();
    cur_record.Note         = ui->textEdit_note->toPlainText();
    cur_record.Nums         = ui->doubleSpinBox_num->value();
    cur_record.StrorageName = ui->comboBox_storeName->currentText();
    cur_record.Unit         = ui->comboBox_unit->currentText();
    cur_record.UserName     = ui->comboBox_userName->currentText();
    if(!cur_record.UserName.isEmpty())
        cur_record.UserID   = ui->comboBox_userName->currentData().toString();

    if(cur_record.GoodsID==""){
        QToolTip::showText(ui->comboBox_goods_id->mapToGlobal(QPoint(100, 0)), "物品编号不能为空!");
        return;
    }
    if(cur_record.GoodsName ==""){
        QToolTip::showText(ui->textEdit_des->mapToGlobal(QPoint(100, 0)), "物品描述不能为空!");
        return;
    }
    if(cur_record.Nums <=0){
        QToolTip::showText(ui->doubleSpinBox_num->mapToGlobal(QPoint(100, 0)), "出库数量不正确s!");
        return;
    }
    if(cur_record.Unit==""){
        QToolTip::showText(ui->comboBox_unit->mapToGlobal(QPoint(100, 0)), "计量单位不能为空!");
        return;
    }
    QJsonObject para        = GoodsOutRecordService::toJsonObject(cur_record);
    boost::thread(boost::bind(&dataCenter::net_newGoodsOut,dataCenter::instance(),para));
    dataCenter::instance()->pub_showLoadding("正在网络请求...",5000,Qt::black);
}

void DialogOutGoods::on_pushButton_cancel_clicked()
{
    done(-1);
}

void DialogOutGoods::GoodIDChange()
{
    QString goodsID =  ui->comboBox_goods_id->currentText();
    if(!goodsID.isEmpty()){
        bool ok = false;
        Goods g = dataCenter::instance()->pub_getGoods(goodsID,ok);
        if(ok){
            ui->textEdit_des->setText(g.Name);
            ui->lineEdit_type->setText(g.Type);
        }else{
            ui->textEdit_des->setText("");
            ui->lineEdit_type->setText("");
        }
    }else{
        ui->textEdit_des->setText("");
        ui->lineEdit_type->setText("");
    }
}

void DialogOutGoods::NewOutCb(GoodsOutRecord g, bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        dataCenter::instance()->pub_showMessage("创建成功!",4000);
        cur_record = g;
        done(123);
    }else{
        dataCenter::instance()->pub_showMessage("创建失败!",4000);
        cur_record = g;
        done(0);
    }
}

GoodsOutRecord DialogOutGoods::getCur_record() const
{
    return cur_record;
}

void DialogOutGoods::initGood(Goods g, bool isNew)
{
    ui->comboBox_goods_id->setEnabled(!isNew);
    ui->comboBox_goods_id->setCurrentText(g.ID);
    ui->textEdit_des->setText(g.Name);
    ui->lineEdit_type->setText(g.Type);
    ui->doubleSpinBox_num->setMaximum(g.Num);
}
