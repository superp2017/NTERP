#ifndef PLATINGSERVICE_H
#define PLATINGSERVICE_H
#include <QJsonObject>
#include <QString>
#include "service_global.h"

class SERVICESHARED_EXPORT PlatingService
{
public:
    PlatingService();
    static QString newPlating(const QJsonObject para, bool &ok, QString hostname, QString hostport);
    static QString delPlating(const QJsonObject para, bool &ok, QString hostname, QString hostport);

    static QVector<QString> getAllPlating(bool &ok, QString hostname, QString hostport);

    static QJsonObject toJsonObject(QString unit);
    static QString fromJsonObject(QJsonObject obj);
};

#endif // PLATINGSERVICE_H
