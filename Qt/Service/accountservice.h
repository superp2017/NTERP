#ifndef ACCOUNTSERVICE_H
#define ACCOUNTSERVICE_H
#include "service_global.h"


class SERVICESHARED_EXPORT accountService
{
public:
    accountService();
     static User login(const QJsonObject para, bool &ok, QString hostname, QString hostport);
};

#endif // ACCOUNTSERVICE_H
