#include "login.h"
#include "ui_login.h"
#include <QStatusBar>
#include <QLabel>
#include <QMovie>
#include <QVBoxLayout>
#include <QToolTip>
#include "datacenter.h"
#include <QMessageBox>
#include "boost/thread.hpp"
#include <QJsonObject>
#include <QDebug>

NLogin::NLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NLogin)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window|\
                   Qt::FramelessWindowHint |\
                   Qt::WindowSystemMenuHint|\
                   Qt::WindowMinimizeButtonHint|\
                   Qt::WindowMaximizeButtonHint);

    ui->n_useNameLine->setPlaceholderText("用户名");
    ui->n_usePwdLine->setPlaceholderText("密码");
    ui->n_usePwdLine->setEchoMode(QLineEdit::Password);

    ui->pushButton->setStyleSheet("QPushButton{border-image: url(:/icon/logon-a.png);}"
                                  "QPushButton:hover{border-image: url(:/icon/login-b.png);}"
                                  "QPushButton:pressed{border-image: url(:/icon/login-b.png);}"
                                  "QPushButton:checked{border-image: url(:/icon/login-b.png);}");
    ui->pushButton_exit->setStyleSheet("QPushButton{border-image: url(:/icon/Q-out-yellow.png);}"
                                  "QPushButton:hover{border-image: url(:/icon/Q-out.png);}"
                                  "QPushButton:pressed{border-image: url(:/icon/Q-out.png);}"
                                  "QPushButton:checked{border-image: url(:/icon/Q-out.png);}");


    ui->pushButton_mini->setStyleSheet("QPushButton{border-image: url(:/icon/mini-a.png);}"
                                       "QPushButton:hover{border-image: url(:/icon/mini.png);}"
                                       "QPushButton:pressed{border-image: url(:/icon/mini.png);}");



    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(login()));
    connect(dataCenter::instance(),SIGNAL(sig_login(bool)),this,SLOT(loginCb(bool)));
    ui->n_useNameLine->setText(dataCenter::instance()->CurSettings().Account);
}

NLogin::~NLogin()
{
    delete ui;
}

void NLogin::do_login(QString acc, QString pwd)
{
    QJsonObject para;
    para.insert("Account",acc);
    para.insert("Code",pwd);
    boost::thread t(boost::bind(&dataCenter::net_login,dataCenter::instance(),para));
    t.detach();
    dataCenter::instance()->pub_showMessage("正在登录...",3000);
}

void NLogin::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);
    QPalette pal(palette());
    pal.setBrush(QPalette::Window, QBrush(QImage(":/icon/login-back.png")));
    setPalette(pal);
    ////////////////////////////
    ui->widget_2->setAutoFillBackground(true); // 这句要加上, 否则可能显示不出背景图.
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window,
                     QBrush(QPixmap(":/icon/login-for.png").scaled(ui->widget_2->size(),
                                                                   Qt::IgnoreAspectRatio,
                                                                   Qt::SmoothTransformation)));             // 使用平滑的缩放方式
    ui->widget_2->setPalette(palette);                           // 给widget加上背景图

}

void NLogin::loginCb(bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(!ok){
        QMessageBox msgBox;
        msgBox.setText("登录失败!");
        msgBox.exec();
        return;
    }
    this->done(123);
}

void NLogin::login()
{
    QString useName = ui->n_useNameLine->text();
    QString usePwd = ui->n_usePwdLine->text();
    if(useName == "") {
        QToolTip::showText(ui->n_useNameLine->mapToGlobal(QPoint(100, 0)), "用户名输入有误");
        return;
    }
    if(usePwd == ""){
        QToolTip::showText(ui->n_usePwdLine->mapToGlobal(QPoint(100, 0)), "密码输入有误");
        return;
    }
    do_login(useName,usePwd);
}

void NLogin::on_pushButton_exit_clicked()
{
    this->done(-1);
}

void NLogin::on_pushButton_mini_clicked()
{
  this->showMinimized();
}
