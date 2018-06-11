#include "dialognewdepartment.h"
#include "ui_dialognewdepartment.h"
#include <QToolTip>
#include "datacenter.h"
#include "userservice.h"
#include "boost/thread.hpp"

DialogNewDepartment::DialogNewDepartment(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewDepartment)
{
    ui->setupUi(this);
    connect(dataCenter::instance(),SIGNAL(sig_newDepartment(QString,bool)),this,SLOT(newDepartMentCb(QString,bool)));
}

DialogNewDepartment::~DialogNewDepartment()
{
    delete ui;
}

void DialogNewDepartment::on_pushButton_cancle_clicked()
{
    done(-1);
}

void DialogNewDepartment::on_pushButton__ok_clicked()
{
    cur_department = ui->lineEdit->text().trimmed();
    if(cur_department.isEmpty()){
        QToolTip::showText(ui->lineEdit->mapToGlobal(QPoint(100, 0)), "部门不能为空!");
        return ;
    }
    if(dataCenter::instance()->pub_checkUnitExist(cur_department)){
        QToolTip::showText(ui->lineEdit->mapToGlobal(QPoint(100, 0)), "此部门已经存在!");
        return ;
    }
    QJsonObject obj;
    obj.insert("Department",cur_department);
    boost::thread t(boost::bind(&dataCenter::net_delDepartment,dataCenter::instance(),obj));
    t.detach();

    dataCenter::instance()->pub_showLoadding("正在网络请求...",5000,Qt::black);
}

void DialogNewDepartment::newDepartMentCb(QString de,bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        dataCenter::instance()->pub_showMessage("添加成功!",4000);
        cur_department = de;
        done(123);
    }else{
        dataCenter::instance()->pub_showMessage("添加失败!",4000);
        cur_department = de;
        done(0);
    }
}

QString DialogNewDepartment::getCur_department() const
{
    return cur_department;
}
