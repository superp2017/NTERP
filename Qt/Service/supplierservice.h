#ifndef SUPPLIERSERVICE_H
#define SUPPLIERSERVICE_H
#include <QJsonObject>
#include <QString>
#include "service_global.h"

class SERVICESHARED_EXPORT SupplierService
{
public:
    SupplierService();
    static Supplier newSupplier(const QJsonObject para, bool &ok, QString hostname, QString hostport);
    static Supplier modSupplier(const QJsonObject para, bool &ok, QString hostname, QString hostport);
    static Supplier delSupplier(const QJsonObject para, bool &ok, QString hostname, QString hostport);
    static QVector<Supplier> getAllSupplierls(bool &ok, QString hostname, QString hostport);

    static QJsonObject toJsonObject(Supplier super);
    static Supplier fromJsonObject(QJsonObject obj);
    static bool exportSupplier(QVector<Supplier>list, QString filepath, bool isOpen=false);

};

#endif // SUPPLIERSERVICE_H
