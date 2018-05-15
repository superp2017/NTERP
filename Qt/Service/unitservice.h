#ifndef UNITSERVICE_H
#define UNITSERVICE_H
#include <QJsonObject>
#include <QString>
#include "service_global.h"
class UnitService
{
public:
    UnitService();
    static QString newUnit(const QString para, bool &ok, QString hostname, QString hostport);
    static QJsonObject toJsonObject(QString unit);
    static QString fromJsonObject(QJsonObject obj);
};

#endif // UNITSERVICE_H
