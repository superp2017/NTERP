#include "formcenter.h"
#include "ui_formcenter.h"
#include <QMessageBox>
#include <QDockWidget>
#include "datacenter.h"
#include <QDebug>


FormCenter::FormCenter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormCenter)
{
    ui->setupUi(this);
    ui->exit->setStyleSheet("QPushButton{border-image: url(:/icon/Q-out-yellow.png);}"
                            "QPushButton:hover{border-image: url(:/icon/Q-out.png);}"
                            "QPushButton:pressed{border-image: url(:/icon/Q-out.png);}");

    ui->pushButton_mini->setStyleSheet("QPushButton{border-image: url(:/icon/mini-a.png);}"
                                       "QPushButton:hover{border-image: url(:/icon/mini.png);}"
                                       "QPushButton:pressed{border-image: url(:/icon/mini.png);}");

    ui->order_btn->setStyleSheet("QToolButton{border-image: url(:/icon/OrderManagement.jpg);}"
                                 "QToolButton:hover{border-image: url(:/icon/OrderManagementgreen.jpg);}"
                                 "QToolButton:pressed{border-image: url(:/icon/OrderManagementgreen.jpg);}"
                                 "QToolButton:checked{border-image: url(:/icon/OrderManagementgreen.jpg);}");
    ui->person_btn->setStyleSheet("QToolButton{border-image: url(:/icon/PersonnelManagement.jpg);}"
                                  "QToolButton:hover{border-image: url(:/icon/PersonnelManagementgreen.jpg);}"
                                  "QToolButton:pressed{border-image: url(:/icon/PersonnelManagementgreen.jpg);}"
                                  "QToolButton:checked{border-image: url(:/icon/PersonnelManagementgreen.jpg);}");
    ui->store_btn->setStyleSheet("QToolButton{border-image: url(:/icon/WarehouseManagement.jpg);}"
                                 "QToolButton:hover{border-image: url(:/icon/WarehouseManagementgreen.jpg);}"
                                 "QToolButton:pressed{border-image: url(:/icon/WarehouseManagementgreen.jpg);}"
                                 "QToolButton:checked{border-image: url(:/icon/WarehouseManagementgreen.jpg);}");
    ui->set_Btn->setStyleSheet("QToolButton{border-image: url(:/icon/SystemSetup.jpg);}"
                               "QToolButton:hover{border-image: url(:/icon/SystemSetupgreen.jpg);}"
                               "QToolButton:pressed{border-image: url(:/icon/SystemSetupgreen.jpg);}"
                               "QToolButton:checked{border-image: url(:/icon/SystemSetupgreen.jpg);}");
    ui->stackedWidget->addWidget(&m_order);
    ui->stackedWidget->addWidget(&m_store);
    ui->stackedWidget->addWidget(&m_person);
    ui->stackedWidget->addWidget(&m_sys);

    connect(this,SIGNAL(action_new_order()),&m_order,SLOT(new_order()));
    connect(this,SIGNAL(action_new_user()),&m_person,SLOT(new_employee()));

    connect(dataCenter::instance(),SIGNAL(sig_showStatusMessage(QString,int)),this,SLOT(showMessage(QString,int)));

    ui->order_btn->setChecked(true);


    m_timer = new QTimer(this);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(timeoutslot()));//timeoutslot()为自定义槽
    connect(&m_sys,SIGNAL(newOrder()),&m_order,SLOT(new_order()));
    ui->label__curuser_name->setText("当前用户: "+dataCenter::instance()->pub_CurUser().Name);
}

FormCenter::~FormCenter()
{
    delete ui;
}





void FormCenter::on_order_btn_clicked()
{
    ui->person_btn->setChecked(false);
    ui->store_btn->setChecked(false);
    ui->set_Btn->setChecked(false);
    ui->stackedWidget->setCurrentWidget(&m_order);

}

void FormCenter::on_store_btn_clicked()
{
    ui->order_btn->setChecked(false);
    ui->person_btn->setChecked(false);
    ui->set_Btn->setChecked(false);
    ui->stackedWidget->setCurrentWidget(&m_store);
}

void FormCenter::on_person_btn_clicked()
{
    ui->order_btn->setChecked(false);
    ui->store_btn->setChecked(false);
    ui->set_Btn->setChecked(false);
    ui->stackedWidget->setCurrentWidget(&m_person);
}

void FormCenter::on_set_Btn_clicked()
{
    ui->order_btn->setChecked(false);
    ui->store_btn->setChecked(false);
    ui->person_btn->setChecked(false);
    ui->stackedWidget->setCurrentWidget(&m_sys);
}


void FormCenter::timeoutslot()
{
    ui->label_msg->setText("");
}

void FormCenter::showMessage(QString msg, int delay)
{
    ui->label_msg->setText(msg);
    if(delay>0){
        m_timer->start(delay);
    }
}

void FormCenter::on_exit_clicked()
{
    QMessageBox msg;
    msg.setWindowTitle("提示");
    msg.setText("系统即将退出");
    msg.setInformativeText("是否继续？");
    msg.setStandardButtons(QMessageBox::Ok| QMessageBox::Cancel);
    msg.setDefaultButton(QMessageBox::Ok);
    int ret = msg.exec();
    switch (ret) {
    case QMessageBox::Ok:
        emit exitApp();
        break;
    case QMessageBox::Cancel:
        break;
    }
}

void FormCenter::clearChecked()
{
    ui->set_Btn->setChecked(false);
    ui->order_btn->setChecked(false);
    ui->store_btn->setChecked(false);
    ui->person_btn->setChecked(false);
}

void FormCenter::on_pushButton_mini_clicked()
{
    emit show_mini();
}


