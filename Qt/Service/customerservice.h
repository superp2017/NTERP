#ifndef CUSTOMERSERVICE_H
#define CUSTOMERSERVICE_H
#include "service_global.h"


class SERVICESHARED_EXPORT CustomerService
{
public:
    CustomerService();
    static Customer newCustomer(const QJsonObject para, bool &ok, QString hostname, QString hostport);
    static Customer modCustomer(const QJsonObject para, bool &ok, QString hostname, QString hostport);
    static QVector<Customer> getAllCustomer(bool &ok, QString hostname, QString hostport);

    static QJsonObject toJsonObject(Customer customer);
    static Customer fromJsonObject(QJsonObject obj);
    static bool exportCustomer(QVector<Customer>list, QString filepath, bool isOpen=false);
};

#endif // CUSTOMERSERVICE_H
