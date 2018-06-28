#include "platingservice.h"
#include "http.h"
#include <QJsonArray>
#include <QJsonValue>
#include <QDebug>
PlatingService::PlatingService()
{

}

QString PlatingService::newPlating(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
    QString unit;
    std::string url = Net_NewPlating;
    bool r   = false;
    Ret ret  = Http::fetch(url,para,r,hostname,hostport);
    if(r&&ret.ret){
        if(ret.data.isString()){
            unit = ret.data.toString();
            ok = true;
            return  unit;
        }
    }
    if(!ret.ret)
        qDebug()<<"newPlating ret is not 0"<<endl;
    ok = false;
    return unit;
}

QString PlatingService::delPlating(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
    QString unit;
    std::string url = Net_DelPlating;
    bool r   = false;
    Ret ret  = Http::fetch(url,para,r,hostname,hostport);
    if(r&&ret.ret){
        if(ret.data.isString()){
            unit = ret.data.toString();
            ok = true;
            return  unit;
        }
    }
    if(!ret.ret)
        qDebug()<<"delPlating ret is not 0"<<endl;
    ok = false;
    return unit;
}

QVector<QString> PlatingService::getAllPlating(bool &ok, QString hostname, QString hostport)
{
    QVector<QString> unit;
    std::string url = Net_GlobalPlating;
    bool r   = false;
    Ret ret  = Http::fetch(url,QJsonObject(),r,hostname,hostport);
    if(r&&ret.ret){
        if(ret.data.isArray()){
            for (QJsonValue v:ret.data.toArray()){
                unit.push_back(v.toString());
            }
            ok = true;
            return  unit;
        }
    }
    if(!ret.ret)
        qDebug()<<"getAllPlating ret is not 0"<<endl;
    ok = false;
    return unit;
}

QJsonObject PlatingService::toJsonObject(QString unit)
{
    QJsonObject obj;
    obj.insert("Plating",unit);
    return obj;
}

QString PlatingService::fromJsonObject(QJsonObject obj)
{
    QString un;
    if(obj.contains("Plating")){
        QJsonValue value = obj.value("Plating");
        if(value.isString())
           un = value.toString();
    }
    return un;
}
