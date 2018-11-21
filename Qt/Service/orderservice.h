#ifndef ORDERSERVICE_H
#define ORDERSERVICE_H
#include "service_global.h"
#include <QJsonObject>
#pragma execution_character_set("utf-8")


class SERVICESHARED_EXPORT OrderService
{
public:
    OrderService();
    static Order newOrder(const QJsonObject para, bool &ok, QString hostname, QString hostport);
    static Order modOrder(const QJsonObject para, bool &ok, QString hostname, QString hostport);
    static Order cancleOrder(const QJsonObject para, bool &ok, QString hostname, QString hostport);
    static Order produceOrder(const QJsonObject para, bool &ok, QString hostname, QString hostport);
    static Order finishOrder(const QJsonObject para, bool &ok, QString hostname, QString hostport);
    static Order modOrderPrice(const QJsonObject para, bool &ok, QString hostname, QString hostport);
    static Order delOrder(const QJsonObject para, bool &ok, QString hostname, QString hostport);
    static QVector<Order> getAllOrders(bool &ok,QString hostname, QString hostport);
    static void updatePrintNum(const QJsonObject para, bool &ok, QString hostname, QString hostport);
    static QJsonObject toJsonObject(Order order);
    static Order fromJsonObject(QJsonObject obj);

    static bool exportOrders(QString curstatus, QVector<Order> list, QString filepath, bool isOpen=false);

    static bool exportOrdersEx(QVector<Order>list,QString filepath,bool isOpen = false);
private:
};

#endif // ORDERSERVICE_H
