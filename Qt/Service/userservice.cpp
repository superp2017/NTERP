#include "userservice.h"
#include <QJsonObject>
UserService::UserService()
{

}


User UserService::newUser(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
#if 1
    User user;
    ok = true;
    return user;
#endif

#if 0
    User user;
    std::string url = Net_NewUser;
    bool r   = false;
    Ret ret  = Http::fetch(url,para,r,hostname,hostport);
    if(r&&ret.ret){
        if(ret.data.isObject()){
            user = fromJsonObject(ret.data.toObject());
            ok = true;
            return  user;
        }
    }
    if(!ret.ret)
        qDebug()<<"newUser ret is not 0";
    ok = false;
    return user;
#endif
}

QJsonObject UserService::toJsonObject(User user)
{
    QJsonObject obj;
    obj.insert("UID",user.UID);
    obj.insert("Name",user.Name);
    obj.insert("Department",user.Department);
    obj.insert("Author",user.Author);
    return obj;

}

User UserService::fromJsonObject(QJsonObject obj)
{
    User user;
    if(obj.contains("UID")){
        QJsonValue value = obj.value("UID");
        if(value.isString())
            user.UID = value.toString();
    }
    if(obj.contains("Name")){
        QJsonValue value = obj.value("Name");
        if(value.isString())
            user.Name = value.toString();
    }
    if(obj.contains("Department")){
        QJsonValue value = obj.value("Department");
        if(value.isString())
            user.Department = value.toString();
    }
    if(obj.contains("Author")){
        QJsonValue value = obj.value("Author");
        if(value.isString())
            user.Author = value.toString();
    }
    return user;
}
