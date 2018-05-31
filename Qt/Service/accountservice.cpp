#include "accountservice.h"
#include "http.h"
#include <QDebug>
#include "userservice.h"
accountService::accountService()
{

}

User accountService::login(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
   User u;
    std::string url = Net_Login;
    bool r   = false;
    Ret ret  = Http::fetch(url,para,r,hostname,hostport);
    if(r&&ret.ret){
        if(ret.data.isObject()){
            u = UserService::fromJsonObject(ret.data.toObject());
            ok = true;
            return  u;
        }
    }
    if(!ret.ret)
        qDebug()<<"login ret is not 0"<<endl;
    ok = false;
    return u;
}
