#include "update.h"

update::update(QObject *parent) : QObject(parent)
{

}


CacheInfo update::CacheformJson(QJsonObject obj)
{
    CacheInfo info;
    if(obj.contains("DataType")){
        QJsonValue v = obj.value("DataType");
        if (v.isDouble()){
            info.DataType = v.toInt();
        }
    }
    if(obj.contains("UpDateType")){
        QJsonValue v = obj.value("UpDateType");
        if (v.isDouble()){
            info.UpDateType = v.toInt();
        }
    }
    if(obj.contains("ID")){
        QJsonValue v = obj.value("ID");
        if(v.isString()){
            info.ID = v.toString();
        }
    }

    if(obj.contains(TimeStamp)){
        QJsonValue v = obj.value("TimeStamp");
        if (v.isDouble()){
            info.TimeStamp = v.toInt();
        }
    }

    if(obj.contains("Data")){
        info.Data = obj.value("Data");
    }
    return info;
}

QVector<CacheInfo> update::HeartBeat(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
    QVector<CacheInfo> list;
    std::string url = Net_HeartBeat;
    Materiel c;
    bool r   = false;
    Ret ret  = Http::fetch(url,para,r,hostname,hostport);
    if(r&&ret.ret){
        if(ret.data.isArray()){
            QJsonArray arr = ret.data.toArray();
            for(QJsonValue v:arr){
                CacheInfo r = CacheformJson(v.toObject());
                list.push_back(r);
            }
        }
        ok =true;
        return list;
    }
    if(!ret.ret)
        qDebug()<<"HeartBeat ret is not 0"<<endl;
    ok = false;
    return list;
}
