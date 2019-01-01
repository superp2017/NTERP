#include "dialogversionupdate.h"
#include "ui_dialogversionupdate.h"
#include <QSettings>

#include <QDesktopServices>
#include <QUrl>
#include <QDebug>
DialogVersionUpdate::DialogVersionUpdate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogVersionUpdate)
{
    ui->setupUi(this);
}

DialogVersionUpdate::~DialogVersionUpdate()
{
    delete ui;
}



void DialogVersionUpdate::setVersion(VersionInfo &info)
{
    ui->lineEdit_date->setText(info.Date);
    ui->lineEdit_version->setText(QString("V%1 (%2)").arg(info.Version).arg(info.VersionNum));
    ui->textEdit_des->setText(QString("更新内容：%1\n下载地址：\n%2")
                              .arg(info.Des)
                              .arg(info.Url));
    m_net_version = info;
    m_url = info.Url;
}

void DialogVersionUpdate::on_pushButton_2_clicked()
{
    done(-1);
}

void DialogVersionUpdate::on_pushButton_clicked()
{
    QDesktopServices::openUrl(QUrl(m_url));
    done(123);
}


