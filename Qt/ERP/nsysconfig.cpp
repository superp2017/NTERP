#include "nsysconfig.h"
#include <QSettings>

nSysConfig::nSysConfig()
{
    QSettings m_settings("./config.ini",QSettings::IniFormat);
    m_settings.beginGroup("Account");
    m_set.Host_name  = m_settings.value("HOST_NAME").toString();
    m_set.Host_port  = m_settings.value("HOST_PORT").toString();
    m_set.Account = m_settings.value("Account").toString();
    m_set.Code = m_settings.value("Code").toString();
    m_settings.endGroup();

    m_settings.beginGroup("Setting");
    QString op = m_settings.value("ExportOpen").toString();
    if(op=="0"){
        m_set.isExportOpen = true;
    }else{
        m_set.isExportOpen = false;
    }
    m_settings.endGroup();
    m_settings.beginGroup("Accounts");
    m_accounts = m_settings.value("Lists").toStringList().toSet();
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
    m_settings.setValue("Code",set.Code);
    m_settings.endGroup();
    m_settings.beginGroup("Accounts");
//    QString list;
//    QList<QString> l = m_accounts.toList();

    QStringList list = QStringList::fromSet(m_accounts);
//    for(int i =0;i<l.size();++i){
//        list+=l.at(i);
//        if(i!=l.size()-1){
//            list+=",";
//        }
//    }
    m_settings.setValue("Lists",list);
    m_settings.endGroup();
}

SysSetting nSysConfig::Setting() const
{
    return m_set;
}

void nSysConfig::setAccount(QString acc, QString code)
{
    m_set.Account = acc;
    m_set.Code = code;
    m_accounts.insert(acc);
}

void nSysConfig::reSave()
{
    setSetting(m_set);
}

QSet<QString> nSysConfig::accounts() const
{
    return m_accounts;
}
