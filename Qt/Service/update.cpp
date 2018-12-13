#include "update.h"
#include "http.h"
update::update(QObject *parent) : QObject(parent)
{

}

int update::HeartBeat(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
    int stamp = 0;
    std::string url = Net_HeartBeat;
    bool r   = false;
    Ret ret  = Http::fetch(url,para,r,hostname,hostport);
    if(r&&ret.ret){
        if(ret.data.isDouble()){
            stamp = ret.data.toInt();
        }
        ok =true;
        return stamp;
    }
    if(!ret.ret)
        qDebug()<<"HeartBeat ret is not 0"<<endl;
    ok = false;
    return  stamp;
}

VersionInfo update::GetVersion(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
    VersionInfo v;
    std::string url = Net_Getversion;
    bool r   = false;
    Ret ret  = Http::fetch(url,para,r,hostname,hostport);
    if(r&&ret.ret){
        if(ret.data.isObject()){
            v = fromJsonObject(ret.data.toObject());
        }
        ok =true;
        return v;
    }
    if(!ret.ret)
        qDebug()<<"GetVersion ret is not 0"<<endl;
    ok = false;
    return  v;
}

void update::SetVersion(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
    std::string url = Net_Setversion;
    bool r   = false;
    Ret ret  = Http::fetch(url,para,r,hostname,hostport);
    if(r&&ret.ret){
        ok =true;
        return ;
    }
    if(!ret.ret)
        qDebug()<<"SetVersion ret is not 0"<<endl;
    ok = false;
    return ;
}


QJsonObject update::toJsonObject(VersionInfo v)
{
    QJsonObject obj;
    obj.insert("Version",v.Version);
    obj.insert("VersionNum",v.VersionNum);
    obj.insert("Date",v.Date);
    obj.insert("Des",v.Des);
    return obj;
}

VersionInfo update::fromJsonObject(QJsonObject obj)
{
    VersionInfo v;
    if(obj.contains("Version")){
        QJsonValue va = obj.value("Version");
        if(va.isString())
            v.Version = va.toString();
    }
    if(obj.contains("VersionNum")){
        QJsonValue va = obj.value("VersionNum");
        if(va.isDouble())
            v.VersionNum = va.toInt();
    }
    if(obj.contains("Date")){
        QJsonValue va = obj.value("Date");
        if(va.isString())
            v.Date = va.toString();
    }
    if(obj.contains("Des")){
        QJsonValue va = obj.value("Des");
        if(va.isString())
            v.Des = va.toString();
    }
    return v;
}
