#ifndef ORDERSERVICE_H
#define ORDERSERVICE_H
#include "service_global.h"
#include <QJsonObject>


class SERVICESHARED_EXPORT OrderService
{
public:
    OrderService();
    static Order newOrder(const QJsonObject para, bool &ok, QString hostname, QString hostport);
    static QJsonObject toJsonObject(Order order);
    static Order fromJsonObject(QJsonObject obj);
};

#endif // ORDERSERVICE_H
