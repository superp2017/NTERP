#ifndef UNITSERVICE_H
#define UNITSERVICE_H
#include <QJsonObject>
#include <QString>
#include "service_global.h"
class SERVICESHARED_EXPORT UnitService
{
public:
    UnitService();
    static QString newUnit(const QJsonObject para, bool &ok, QString hostname, QString hostport);
    static QString delUnit(const QJsonObject para, bool &ok, QString hostname, QString hostport);

    static QVector<QString> getAllUnits(bool &ok, QString hostname, QString hostport);

    static QJsonObject toJsonObject(QString unit);
    static QString fromJsonObject(QJsonObject obj);
};

#endif // UNITSERVICE_H
