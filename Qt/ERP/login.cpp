#include "login.h"
#include "ui_login.h"
#include <QStatusBar>
#include <QLabel>
#include <QMovie>
#include <QVBoxLayout>
#include <QToolTip>
#include "datacenter.h"


#pragma execution_character_set("utf-8")

NLogin::NLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NLogin)
{
    ui->setupUi(this);
    ui->n_useNameLine->setPlaceholderText("用户名");
    ui->n_usePwdLine->setPlaceholderText("密码");
    ui->n_usePwdLine->setEchoMode(QLineEdit::Password);
    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(login()));
    QPalette pal(palette());
    pal.setBrush(QPalette::Window, QBrush(QImage(":/icon/login.png")));
    setPalette(pal);
    ////////////////////////////
    ui->widget_2->setAutoFillBackground(true); // 这句要加上, 否则可能显示不出背景图.
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window,
                     QBrush(QPixmap(":/icon/login-a.png").scaled(ui->widget_2->size(),
                                                                 Qt::IgnoreAspectRatio,
                                                                 Qt::SmoothTransformation)));             // 使用平滑的缩放方式
    ui->widget_2->setPalette(palette);                           // 给widget加上背景图
    ui->pushButton->setStyleSheet("QPushButton{border-image: url(:/icon/loginButton-a.png);}"
                                  "QPushButton:pressed{border-image: url(:/icon/loginButton-two.png);}");
}

NLogin::~NLogin()
{
    delete ui;
}

void NLogin::login()
{
    QString useName = ui->n_useNameLine->text();
    QString usePwd = ui->n_usePwdLine->text();
    if(useName == "admin" && usePwd == "123456"){
//      dataCenter::instance()->setCurUser(U);
      this->done(123);
    }else{
        QToolTip::showText(ui->n_useNameLine->mapToGlobal(QPoint(100, 0)), "用户名或密码不正确");
        QToolTip::showText(ui->n_usePwdLine->mapToGlobal(QPoint(100, 0)), "用户名或密码不正确");
    }

}
