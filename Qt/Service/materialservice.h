#ifndef MATERIALSERVICE_H
#define MATERIALSERVICE_H
#include "service_global.h"
#include <QJsonObject>
#include <QVector>

#pragma execution_character_set("utf-8")


class SERVICESHARED_EXPORT MaterialService
{
public:
    MaterialService();

    static Materiel newMaterial(const QJsonObject para,bool&ok,QString hostname, QString hostport);
    static Materiel modMaterial(const QJsonObject para,bool&ok,QString hostname, QString hostport);
    static Materiel delMaterial(const QJsonObject para, bool &ok, QString hostname, QString hostport);
    static Materiel queryMaterial(const QJsonObject para,bool&ok,QString hostname, QString hostport);
    static QVector<QString>getCustomerMaterID(const QJsonObject para,bool&ok,QString hostname, QString hostport);
    static QVector<Materiel> getAllMateriels(const QJsonObject para, bool &ok, QString hostname, QString hostport);

    static QJsonObject toJsonObject(Materiel ma);
    static Materiel fromJsonObject(QJsonObject obj);
    static bool exportMateriel(QVector<Materiel> list, QString filepath, bool isOpen=false);
};

#endif // MATERIALSERVICE_H
