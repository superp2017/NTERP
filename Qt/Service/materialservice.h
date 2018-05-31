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

    static QVector<Materiel> getAllMateriels(bool &ok, QString hostname, QString hostport);

    static QJsonObject toJsonObject(Materiel ma);
    static Materiel fromJsonObject(QJsonObject obj);
    static bool exportMateriel(QVector<Materiel> list, QString filepath, bool isOpen=false);
};

#endif // MATERIALSERVICE_H
