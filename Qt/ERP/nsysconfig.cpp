#include "nsysconfig.h"
#include <QSettings>
nSysConfig::nSysConfig()
{
   QSettings m_settings("./config.ini",QSettings::IniFormat);
    m_settings.beginGroup("Account");
    m_hostName = m_settings.value("HOST_NAME").toString();
    m_hostPort  = m_settings.value("HOST_PORT").toString();
    m_settings.endGroup();
}
