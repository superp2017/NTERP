#ifndef VERSION_H
#define VERSION_H

#include <QObject>
#include "service_global.h"
#pragma execution_character_set("utf-8")

class version : public QObject
{
    Q_OBJECT
public:
    explicit version(QObject *parent = 0);
    void setNetVersion(VersionInfo &info,bool ok);

signals:

public slots:
    int checkVersion(QWidget *w=0);
private:
     void loadVersion();
     void saveVersion();

private:
    VersionInfo m_cur_ver;//本地版本
    VersionInfo m_net_ver;//网络版本
    bool m_net_ok;
    QString downurl;

};

#endif // VERSION_H
