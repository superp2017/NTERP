#ifndef HTTP_H
#define HTTP_H
#include <QJsonObject>
#include <QString>
#include "service_global.h"
class  Http
{
public:
    Http();
    static Ret fetch(std::string url, QJsonObject para, bool &success, \
                          QString host,  QString port);
};

#endif // HTTP_H
