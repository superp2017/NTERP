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
    //    static CacheInfo CacheformJson(QJsonObject obj);
    //    static QVector<CacheInfo> HeartBeat(const QJsonObject para, bool &ok, QString hostname, QString hostport);
    //signals:
    static int HeartBeat(const QJsonObject para, bool &ok, QString hostname, QString hostport);
public slots:
};

#endif // UPDATE_H
