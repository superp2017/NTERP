#ifndef ORDERSERVICE_H
#define ORDERSERVICE_H
#include "service_global.h"
#include <QVariantMap>

class SERVICESHARED_EXPORT OrderService
{
public:
    OrderService();
    void newOrder(QVariantMap para);
};

#endif // ORDERSERVICE_H
