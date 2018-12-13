#ifndef NACCOUNTSETTINGS_H
#define NACCOUNTSETTINGS_H

#include <QString>
#include <QSet>
#pragma execution_character_set("utf-8")
#include "global.h"


class nSysConfig
{
public:
    nSysConfig();
    void clear();
    QString HOST_NAME();
    QString  HOST_PORT();
    void setSetting(SysSetting set);
    SysSetting Setting() const;
    void setAccount(QString acc,QString code);
    void reSave();

    QSet<QString> accounts() const;
private:
    SysSetting m_set;
    QSet<QString> m_accounts;

};

#endif // NACCOUNTSETTINGS_H
