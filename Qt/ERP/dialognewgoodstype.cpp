#include "dialognewgoodstype.h"
#include "ui_dialognewgoodstype.h"
#include "datacenter.h"
#include <QToolTip>
#include "boost/thread.hpp"


DialogNewGoodsType::DialogNewGoodsType(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewGoodsType)
{
    ui->setupUi(this);
    connect(dataCenter::instance(),SIGNAL(sig_newGoodsType(QString,bool)),this,SLOT(newTypeCb(QString,bool)));
}

DialogNewGoodsType::~DialogNewGoodsType()
{
    delete ui;
}

QString DialogNewGoodsType::getType()const
{
    return curType;
}

void DialogNewGoodsType::on_pushButton_cancle_clicked()
{
    done(-1);
}

void DialogNewGoodsType::on_pushButton__ok_clicked()
{
    curType = ui->lineEdit->text().trimmed();
    if(curType.isEmpty()){
        QToolTip::showText(ui->lineEdit->mapToGlobal(QPoint(100, 0)), "分类不能为空!");
        return ;
    }
    if(dataCenter::instance()->pub_checkTypeExist(curType)){
        QToolTip::showText(ui->lineEdit->mapToGlobal(QPoint(100, 0)), "此分类已经存在!");
        return ;
    }
    QJsonObject obj;
    obj.insert("Type",curType);
    boost::thread(boost::bind(&dataCenter::net_newGoodsType,dataCenter::instance(),obj)).detach();
    dataCenter::instance()->pub_showLoadding("正在网络请求...",5000,Qt::black);
}

void DialogNewGoodsType::newTypeCb(QString type, bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    curType = type;
    if(ok){
        dataCenter::instance()->pub_showMessage("添加成功!",3000);
        done(123);
    }else{
        dataCenter::instance()->pub_showMessage("添加失败!",3000);
        done(0);
    }
}
