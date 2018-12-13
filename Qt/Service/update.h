#ifndef UPDATE_H
#define UPDATE_H

#include <QObject>
#include "service_global.h"
#pragma execution_character_set("utf-8")

#include <QJsonObject>
class SERVICESHARED_EXPORT update : public QObject
{
    Q_OBJECT
public:
    explicit update(QObject *parent = 0);
    static int HeartBeat(const QJsonObject para, bool &ok, QString hostname, QString hostport);
    static VersionInfo GetVersion(const QJsonObject para, bool &ok, QString hostname, QString hostport);
    static void SetVersion(const QJsonObject para, bool &ok, QString hostname, QString hostport);
    static QJsonObject toJsonObject(VersionInfo v);
    static VersionInfo fromJsonObject(QJsonObject obj);
public slots:
};

#endif // UPDATE_H
