#ifndef GOODS_H
#define GOODS_H
#include "service_global.h"


class SERVICESHARED_EXPORT GoodsService
{
public:
    GoodsService();
    static Goods newGoods(const QJsonObject para, bool &ok, QString hostname, QString hostport);
    static Goods modGoods(const QJsonObject para, bool &ok, QString hostname, QString hostport);
    static Goods delGoods(const QJsonObject para, bool &ok, QString hostname, QString hostport);
    static QVector<Goods> getAllGoods(bool &ok, QString hostname, QString hostport);
    static QJsonObject toJsonObject(Goods goods);
    static Goods fromJsonObject(QJsonObject obj);
    static bool exportGoods(QVector<Goods>list, QString filepath, bool isOpen=false);
};

#endif // GOODS_H
