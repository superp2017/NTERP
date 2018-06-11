#include "nsysconfig.h"
#include <QSettings>

nSysConfig::nSysConfig()
{
    QSettings m_settings("./config.ini",QSettings::IniFormat);
    m_settings.beginGroup("Account");
    m_set.Host_name  = m_settings.value("HOST_NAME").toString();
    m_set.Host_port  = m_settings.value("HOST_PORT").toString();
    m_set.Account = m_settings.value("Account").toString();
    m_settings.endGroup();

    m_settings.beginGroup("Setting");
    QString op = m_settings.value("ExportOpen").toString();
    if(op=="0"){
        m_set.isExportOpen = true;
    }else{
        m_set.isExportOpen = false;
    }
    m_settings.endGroup();
}

QString nSysConfig::HOST_NAME()
{
    return  m_set.Host_name;
}

QString nSysConfig::HOST_PORT()
{
    return m_set.Host_port;
}

void nSysConfig::setSetting(SysSetting set)
{
    m_set = set;
    QSettings m_settings("./config.ini",QSettings::IniFormat);
    QString op="0";
    if(set.isExportOpen)
        op="0";
    else
        op="1";
    m_settings.beginGroup("Setting");
    m_settings.setValue("ExportOpen",op);
    m_settings.endGroup();

    m_settings.beginGroup("Account");
    m_settings.setValue("HOST_NAME",set.Host_name);
    m_settings.setValue("HOST_PORT",set.Host_port);
    m_settings.setValue("Account",set.Account);
    m_settings.endGroup();
}

SysSetting nSysConfig::Setting() const
{
    return m_set;
}

void nSysConfig::setAccount(QString acc)
{
    m_set.Account = acc;
}

void nSysConfig::reSave()
{
    setSetting(m_set);
}
