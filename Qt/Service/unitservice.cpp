#include "unitservice.h"
#include "http.h"
UnitService::UnitService()
{

}

QString UnitService::newUnit(const QString para, bool &ok, QString hostname, QString hostport)
{
#if 1
    ok= true;
    return para;
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

