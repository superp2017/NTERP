#ifndef NACCOUNTSETTINGS_H
#define NACCOUNTSETTINGS_H

#include <QString>
#pragma execution_character_set("utf-8")


class nSysConfig
{
public:
    nSysConfig();
    QString HOST_NAME(){
      return   m_hostName;
    }
    QString  HOST_PORT(){
        return m_hostPort;
    }
private:
    QString    m_hostName;
    QString    m_hostPort;

};

#endif // NACCOUNTSETTINGS_H
