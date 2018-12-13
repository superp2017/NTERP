#include "version.h"
#include <QSettings>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>

version::version(QObject *parent) : QObject(parent)
{
    loadVersion();
}

void version::setNetVersion(VersionInfo &info, bool ok)
{
    m_net_ver = info;
    m_net_ok = ok;
}

void version::loadVersion()
{
    QSettings m_settings("./VERSION",QSettings::IniFormat);
    m_settings.beginGroup("VERSION");
    m_cur_ver.Version = m_settings.value("Version").toString();
    m_cur_ver.VersionNum = m_settings.value("VersionNum").toInt();
    m_cur_ver.Date = m_settings.value("Date").toString();
    m_cur_ver.Des = m_settings.value("Des").toString();
    m_settings.endGroup();
}

void version::saveVersion()
{
    QSettings m_settings("./VERSION",QSettings::IniFormat);
    m_settings.beginGroup("VERSION");
    m_settings.setValue("Version",m_cur_ver.Version);
    m_settings.setValue("VersionNum",m_cur_ver.VersionNum);
    m_settings.setValue("Date",m_cur_ver.Date);
    m_settings.setValue("Des",m_cur_ver.Des);
    m_settings.endGroup();
}



void version::checkVersion(QWidget *w)
{
    if(m_net_ok){
        if(m_net_ver.VersionNum>m_cur_ver.VersionNum){
            QString url =QString("http://47.100.166.215/download/");
            QMessageBox msg;
            if(w!=NULL)
                msg.setParent(w);
            msg.setWindowTitle("新版本提示");
            msg.setText(QString("版本号：%1\n版本描述：%2\n更新日期:%3\n下载地址：%4")
                        .arg(m_net_ver.Version)
                        .arg(m_net_ver.Des)
                        .arg(m_net_ver.Date)
                        .arg(url));
            msg.setInformativeText("是否跳转网页下载？");
            msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            msg.setDefaultButton(QMessageBox::Ok);
            int ret = msg.exec();
            switch (ret) {
            case QMessageBox::Ok:
                QDesktopServices::openUrl(QUrl(url));
                break;
            case QMessageBox::Cancel:
                // Cancel was clicked
                break;
            default:
                // should never be reached
                break;
            }
        }
    }
}
