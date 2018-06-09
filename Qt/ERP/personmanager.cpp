#include "personmanager.h"
#include "ui_personmanager.h"
#include "datacenter.h"
#include <QMessageBox>
#include "dialoguserprint.h"
#include "boost/thread.hpp"
#include <QDebug>

PersonManager::PersonManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PersonManager)
{
    ui->setupUi(this);
    tab_mode = QHeaderView::Stretch;
    newuser =NULL;

    connect(ui->radioButton_ave,SIGNAL(clicked(bool)),this,SLOT(changeCol()));
    connect(ui->radioButton_connent,SIGNAL(clicked(bool)),this,SLOT(changeCol()));
    connect(ui->radioButton_manue,SIGNAL(clicked(bool)),this,SLOT(changeCol()));

    connect(ui->tableWidget,SIGNAL(userClick(QString)),this,SLOT(userClick(QString)));

    connect(dataCenter::instance(),SIGNAL(sig_outEmployee(User,bool)),this,SLOT(outUserCb(User,bool)));
    connect(dataCenter::instance(),SIGNAL(sig_delEmployee(QString,bool)),this,SLOT(delUserCb(QString,bool)));
    connect(dataCenter::instance(),SIGNAL(sig_globalEmployees(bool)),this,SLOT(getGlobalUserCb(bool)));

    connect(ui->tableWidget,SIGNAL(newUser()),this,SLOT(on_pushButton_newUser_clicked()));
    connect(ui->tableWidget,SIGNAL(modUser()),this,SLOT(on_pushButton_mod_clicked()));
    connect(ui->tableWidget,SIGNAL(outUser()),this,SLOT(on_pushButton_out_clicked()));
    connect(ui->tableWidget,SIGNAL(delUser()),this,SLOT(on_pushButton_del_clicked()));


    ui->pushButton_newUser->setStyleSheet("QPushButton{border-image: url(:/icon/new-red.png);}"
                                          "QPushButton:hover{border-image: url(:/icon/new.png);}"
                                          "QPushButton:pressed{border-image: url(:/icon/new.png);}"
                                          "QPushButton:checked{border-image: url(:/icon/new.png);}");


    ui->pushButton_export->setStyleSheet("QPushButton{border-image: url(:/icon/export.png);}"
                                         "QPushButton:hover{border-image: url(:/icon/export-a.png);}"
                                         "QPushButton:pressed{border-image: url(:/icon/export-a.png);}"
                                         "QPushButton:checked{border-image: url(:/icon/export-a.png);}");

    ui->pushButton_reflash->setStyleSheet("QPushButton{border-image: url(:/icon/reflash.png);}"
                                          "QPushButton:hover{border-image: url(:/icon/reflash-a.png);}"
                                          "QPushButton:pressed{border-image: url(:/icon/reflash-a.png);}"
                                          "QPushButton:checked{border-image: url(:/icon/reflash-a.png);}");

    setBtnEnable(false,false,false);

    ui->radioButton_ave->setChecked(true);
    changeCol();
    updateData();
}

PersonManager::~PersonManager()
{
    delete ui;
}



void PersonManager::updateData()
{
    ui->tableWidget->initUser(dataCenter::instance()->pub_employees());
    ui->tableWidget->checkSelect();
}

void PersonManager::clearAllSelect()
{
    ui->tableWidget->clearSelection();
    setBtnEnable(false,false,false);
    curUser.UID=="";
    ui->tableWidget->checkSelect();
}



void PersonManager::new_employee()
{
    if(newuser==NULL){
        newuser = new DialogNewUser();
    }
    newuser->setModel(true);
    newuser->clearUI();
    if(newuser->exec()==123){
        User user = newuser->CurUser();
        ui->tableWidget->appendUser(user);
    }
}



void PersonManager::on_pushButton_newUser_clicked()
{
    new_employee();
}

void PersonManager::on_pushButton_mod_clicked()
{
    if(curUser.UID==""){
        return;
    }
    if(newuser==NULL){
        newuser = new DialogNewUser();
    }
    newuser->setModel(false);
    newuser->clearUI();
    newuser->initUI(curUser);
    if(newuser->exec()==123){
        User user = newuser->CurUser();
        ui->tableWidget->modUser(user);
    }
}

void PersonManager::userClick(QString UID)
{
    bool ok =false;
    curUser = dataCenter::instance()->pub_getUser(UID,ok);
    if(ok){
        if(curUser.Status=="-1"){
            setBtnEnable(false,false,false);
        }
        if(curUser.Status=="0"){
            setBtnEnable(true,true,true);
        }
        if(curUser.Status=="1"){
            setBtnEnable(false,false,true);
        }
    }
}


void PersonManager::on_pushButton_out_clicked()
{
    if(curUser.UID==""){
        return;
    }
    QMessageBox msgBox;
    msgBox.setWindowTitle("提示");
    msgBox.setText("您将离职员工:"+curUser.Name);
    msgBox.setInformativeText("是否继续操作?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Ok:
    {
        boost::thread t(boost::bind(&dataCenter::net_outUser,dataCenter::instance(),UserService::toJsonObject(curUser)));
        t.detach();
        dataCenter::instance()->pub_showLoadding("正在网络请求...",5000,Qt::black);
        break;
    }
    case QMessageBox::Cancel:
        break;
    default:
        break;
    }
}

void PersonManager::on_pushButton_del_clicked()
{
    if(curUser.UID==""){
        return;
    }

    QMessageBox msgBox;
    msgBox.setWindowTitle("提示");
    msgBox.setText("您将永久删除员工:"+curUser.Name+"的全部信息,");
    msgBox.setInformativeText("是否继续操作?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Ok:
    {
        boost::thread t(boost::bind(&dataCenter::net_delUser,dataCenter::instance(),UserService::toJsonObject(curUser)));
        t.detach();
        dataCenter::instance()->pub_showLoadding("正在网络请求...",5000,Qt::black);
        break;
    }
    case QMessageBox::Cancel:
        break;
    default:
        break;
    }
}


void PersonManager::outUserCb(User user, bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        dataCenter::instance()->pub_showMessage("离职成功!",4000);
        ui->tableWidget->modUser(user);
    }else{
        dataCenter::instance()->pub_showMessage("离职失败!",4000);
    }
}

void PersonManager::delUserCb(QString user, bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        dataCenter::instance()->pub_showMessage("删除成功!",4000);
        ui->tableWidget->removeUser(user);
        setBtnEnable(false,false,false);
    }else{
        dataCenter::instance()->pub_showMessage("删除失败!",4000);
    }
}

void PersonManager::getGlobalUserCb(bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        clearAllSelect();
        updateData();
        dataCenter::instance()->pub_showMessage("刷新成功!",4000);
    }else{
        dataCenter::instance()->pub_showMessage("刷新失败!",4000);
    }
}


void PersonManager::changeCol()
{
    if(ui->radioButton_ave->isChecked()){
        ui->tableWidget->setHeaderColModel(QHeaderView::Stretch);
    }
    if(ui->radioButton_connent->isChecked()){
        ui->tableWidget->setHeaderColModel(QHeaderView::ResizeToContents);

    }
    if(ui->radioButton_manue->isChecked()){
        ui->tableWidget->setHeaderColModel(QHeaderView::Interactive);
    }
    clearAllSelect();
}



void PersonManager::on_pushButton_export_clicked()
{
    DialogUserPrint  print;
    print.initData(dataCenter::instance()->pub_employees());
    print.exec();
}

void PersonManager::on_pushButton_reflash_clicked()
{
    curUser.UID = "";
    boost::thread t(boost::bind(&dataCenter::net_getGlobalUsers,dataCenter::instance()));
    t.detach();
    dataCenter::instance()->pub_showLoadding("正在网络请求...",5000,Qt::black);
}

void PersonManager::setBtnEnable(bool m, bool o, bool d)
{
    ui->pushButton_mod->setEnabled(m);
    ui->pushButton_out->setEnabled(o);
    ui->pushButton_del->setEnabled(d);

    if(m){
        ui->pushButton_mod->setStyleSheet("QPushButton{border-image: url(:/icon/modify-red.png);}"
                                          "QPushButton:hover{border-image: url(:/icon/modify.png);}"
                                          "QPushButton:pressed{border-image: url(:/icon/modify.png);}"
                                          "QPushButton:checked{border-image: url(:/icon/modify.png);}");
    }else{
        ui->pushButton_mod->setStyleSheet("QPushButton{border-image: url(:/icon/modify.png);}");
    }

    if(o){
        ui->pushButton_out->setStyleSheet("QPushButton{border-image: url(:/icon/quit-red.png);}"
                                          "QPushButton:hover{border-image: url(:/icon/quit.png);}"
                                          "QPushButton:pressed{border-image: url(:/icon/quit.png);}"
                                          "QPushButton:checked{border-image: url(:/icon/quit.png);}");

    }else{
        ui->pushButton_out->setStyleSheet("QPushButton{border-image: url(:/icon/quit.png);}");
    }

    if(d){
        ui->pushButton_del->setStyleSheet("QPushButton{border-image: url(:/icon/delete-red.png);}"
                                          "QPushButton:hover{border-image: url(:/icon/delete.png);}"
                                          "QPushButton:pressed{border-image: url(:/icon/delete.png);}"
                                          "QPushButton:checked{border-image: url(:/icon/delete.png);}");
    }else{
        ui->pushButton_del->setStyleSheet("QPushButton{border-image: url(:/icon/delete.png);}");
    }

}

