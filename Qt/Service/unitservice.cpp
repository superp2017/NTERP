#include "unitservice.h"
#include "http.h"
UnitService::UnitService()
{

}

QString UnitService::newUnit(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
#if 1
    ok= true;
    return fromJsonObject(para);
#endif

#if 0
    QString unit;
    std::string url = Net_NewUnit;
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
        qDebug()<<"newPlan ret is not 0";
    ok = false;
    return unit;
#endif
}

QJsonObject UnitService::toJsonObject(QString unit)
{
    QJsonObject obj;
    obj.insert("Unit",unit);
    return obj;
}

QString UnitService::fromJsonObject(QJsonObject obj)
{
    QString un;
    if(obj.contains("Unit")){
        QJsonValue value = obj.value("Unit");
        if(value.isString())
           un = value.toString();
    }
    return un;
}

