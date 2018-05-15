#ifndef CUSTOMERSERVICE_H
#define CUSTOMERSERVICE_H
#include "service_global.h"


class SERVICESHARED_EXPORT CustomerService
{
public:
    CustomerService();
    static Customer newCustomer(const QJsonObject para, bool &ok, QString hostname, QString hostport);
    static QJsonObject toJsonObject(Customer customer);
    static Customer fromJsonObject(QJsonObject obj);
};

#endif // CUSTOMERSERVICE_H
