#ifndef GOODS_H
#define GOODS_H
#include "service_global.h"
#pragma execution_character_set("utf-8")


class SERVICESHARED_EXPORT GoodsService
{
public:
    GoodsService();
    static Goods newGoods(const QJsonObject para, bool &ok, QString hostname, QString hostport);
    static Goods modGoods(const QJsonObject para, bool &ok, QString hostname, QString hostport);
    static Goods inOutGoods(const QJsonObject para, bool &ok, QString hostname, QString hostport);
    static QString delGoods(const QJsonObject para, bool &ok, QString hostname, QString hostport);
    static QVector<Goods> getAllGoods(bool &ok, QString hostname, QString hostport);
    static QVector<Goods> getSupplierGoods(bool &ok, QString hostname, QString hostport);
    static QString newGoodsType(const QJsonObject para, bool &ok, QString hostname, QString hostport);
    static QString delGoodsType(const QJsonObject para, bool &ok, QString hostname, QString hostport);
    static QVector<QString>getAllGoodsType(bool &ok, QString hostname, QString hostport);
    static QJsonObject toJsonObject(Goods goods);
    static Goods fromJsonObject(QJsonObject obj);
    static bool exportGoods(QVector<Goods>list, QString filepath, bool isOpen=false);
};

#endif // GOODS_H
