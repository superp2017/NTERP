#include "dialognewunit.h"
#include "ui_dialognewunit.h"
#include <QToolTip>
#include "datacenter.h"
#include "unitservice.h"
#include "boost/thread.hpp"


DialogNewUnit::DialogNewUnit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewUnit)

{
    ui->setupUi(this);
    connect(dataCenter::instance(),SIGNAL(sig_newUnit(QString,bool)),this,SLOT(newUnitCb(QString,bool)));
}

DialogNewUnit::~DialogNewUnit()
{
    delete ui;
}

void DialogNewUnit::on_pushButton__ok_clicked()
{
    cur_unit = ui->lineEdit->text().trimmed();
    if(cur_unit.isEmpty()){
        QToolTip::showText(ui->lineEdit->mapToGlobal(QPoint(100, 0)), "单位不能为空!");
        return ;
    }
    if(dataCenter::instance()->pub_checkUnitExist(cur_unit)){
        QToolTip::showText(ui->lineEdit->mapToGlobal(QPoint(100, 0)), "此单位已经存在!");
        return ;
    }

    boost::thread t(boost::bind(&dataCenter::net_newUnit,dataCenter::instance(),UnitService::toJsonObject(cur_unit)));
    t.detach();

    dataCenter::instance()->pub_showLoadding("正在网络请求...",5000,Qt::black);
}

void DialogNewUnit::on_pushButton_cancle_clicked()
{
    done(-1);
}

void DialogNewUnit::newUnitCb(QString unit, bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        dataCenter::instance()->pub_showMessage("添加成功!",4000);
        cur_unit = unit;
        done(123);
    }else{
        dataCenter::instance()->pub_showMessage("添加失败!",4000);
        cur_unit = unit;
        done(0);
    }
}

QString DialogNewUnit::getUnit() const
{
    return cur_unit;
}
