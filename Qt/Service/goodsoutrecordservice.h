#ifndef GOODSOUTRECORDSERVICE_H
#define GOODSOUTRECORDSERVICE_H

#include "service_global.h"
#pragma execution_character_set("utf-8")

class SERVICESHARED_EXPORT GoodsOutRecordService
{
public:
    GoodsOutRecordService();
    static GoodsOutRecord newGoodsRecord(const QJsonObject para, bool &ok, QString hostname, QString hostport);
    static QVector<GoodsOutRecord>GetAllRecords(bool &ok, QString hostname, QString hostport);
    static QJsonObject toJsonObject(GoodsOutRecord record);
    static GoodsOutRecord fromJsonObject(QJsonObject obj);
    static bool exportGoods(QVector<GoodsOutRecord>list, QString filepath, bool isOpen=false);

};

#endif // GOODSOUTRECORDSERVICE_H
