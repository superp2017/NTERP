#include "version.h"
#include <QSettings>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>
#include "dialogversionupdate.h"

version::version(QObject *parent) : QObject(parent)
{
    loadVersion();
    m_net_ok = false;
    m_net_ver.VersionNum = -1;
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
    m_settings.beginGroup("DOWNLOAD");
    m_cur_ver.Url = m_settings.value("Url").toString();
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

    m_settings.beginGroup("DOWNLOAD");
    m_settings.setValue("Url",m_cur_ver.Url);
    m_settings.endGroup();
}



int version::checkVersion(QWidget *w)
{
    if(m_net_ok){
        if(m_net_ver.VersionNum>m_cur_ver.VersionNum){
            DialogVersionUpdate v;
            v.setVersion( m_net_ver);
            int ret = v.exec();
            return ret;
        }
    }
    return 123;
}
