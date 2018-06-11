#ifndef NACCOUNTSETTINGS_H
#define NACCOUNTSETTINGS_H

#include <QString>
#pragma execution_character_set("utf-8")
#include "global.h"


class nSysConfig
{
public:
    nSysConfig();
    QString HOST_NAME();
    QString  HOST_PORT();
    void setSetting(SysSetting set);
    SysSetting Setting() const;
    void setAccount(QString acc);
    void reSave();

private:
    SysSetting m_set;

};

#endif // NACCOUNTSETTINGS_H
