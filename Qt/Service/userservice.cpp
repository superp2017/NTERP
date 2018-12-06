﻿#include "userservice.h"
#include <QJsonObject>
#include <QAxObject>
#include "excelservice.h"
#include <QFileDialog>
#include <QDir>
#include <QDebug>

#include "http.h"

UserService::UserService()
{

}


User UserService::newUser(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
#if 1
    User user;
    std::string url = Net_NewEmployee;
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
        qDebug()<<"newUser ret is not 0"<<endl;
    ok = false;
    return user;
#endif
}

User UserService::modUser(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
#if 1
    User user;
    std::string url = Net_ModEmployee;
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
        qDebug()<<"modUser ret is not 0"<<endl;
    ok = false;
    return user;
#endif
}

User UserService::outUser(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
#if 1
    User user;
    std::string url = Net_OutEmployee;
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
        qDebug()<<"outUser ret is not 0"<<endl;
    ok = false;
    return user;
#endif
}

User UserService::delUser(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
    User user;
    std::string url = Net_DelEmployee;
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
        qDebug()<<"delUser ret is not 0"<<endl;
    ok = false;
    return user;
}

QVector<User> UserService::getAllUsers(const QJsonObject para,bool &ok, QString hostname, QString hostport)
{
    QVector<User> data;
    std::string url = Net_GlobalEmployee;
    bool r   = false;
    Ret ret  = Http::fetch(url,para,r,hostname,hostport);
    if(r&&ret.ret){
        if(ret.data.isArray()){
            for(QJsonValue v:ret.data.toArray()){
                User  c = fromJsonObject(v.toObject());
                data.push_back(c);
            }
            ok = true;
            return  data;
        }
    }
    if(!ret.ret)
        qDebug()<<"getAllUsers ret is not 0"<<endl;
    ok = false;
    return data;
}

QString UserService::newDepartment(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
    QString user;
    std::string url = Net_NewDepartment;
    bool r   = false;
    Ret ret  = Http::fetch(url,para,r,hostname,hostport);
    if(r&&ret.ret){
        if(ret.data.isString()){
            user = ret.data.toString();
            ok = true;
            return  user;
        }
    }
    if(!ret.ret)
        qDebug()<<"newDepartment ret is not 0"<<endl;
    ok = false;
    return user;
}

QString UserService::delDepartment(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
    QString user;
    std::string url = Net_DelDepartment;
    bool r   = false;
    Ret ret  = Http::fetch(url,para,r,hostname,hostport);
    if(r&&ret.ret){
        if(ret.data.isString()){
            user = ret.data.toString();
            ok = true;
            return  user;
        }
    }
    if(!ret.ret)
        qDebug()<<"delDepartment ret is not 0"<<endl;
    ok = false;
    return user;
}

QVector<QString> UserService::getAllDepartment(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
    QVector<QString> list;
    std::string url = Net_GetAllDepartment;
    bool r   = false;
    Ret ret  = Http::fetch(url,QJsonObject(),r,hostname,hostport);
    if(r&&ret.ret){
        if(ret.data.isArray()){
            QJsonArray arr = ret.data.toArray();
            for(QJsonValue v:arr){
                if(v.isString()){
                    QString r = v.toString();
                    list.push_back(r);
                }
            }
            ok = true;
            return list;
        }
    }
    if(!ret.ret)
        qDebug()<<"getAllDepartment ret is not 0"<<endl;
    ok = false;
    return list;
}



QJsonObject UserService::toJsonObject(User user)
{
    QJsonObject obj;
    obj.insert("UID",user.UID);
    obj.insert("Name",user.Name);
    obj.insert("Sex",user.Sex);
    obj.insert("Cell",user.Cell);
    obj.insert("CID",user.CID);
    obj.insert("CommpanyName",user.CommpanyName);
    obj.insert("Department",user.Department);
    obj.insert("Author",user.Author);
    obj.insert("InTime",user.InTime);
    obj.insert("OutTime",user.OutTime);
    obj.insert("CreatTime",user.CreatTime);
    obj.insert("Status",user.Status);
    obj.insert("Account",user.Account);
    obj.insert("Code",user.Code);
    obj.insert("Salary",user.Salary);
    obj.insert("Age",user.Age);
    obj.insert("CreatStamp",user.CreatStamp);
    obj.insert("LastTime",user.LastTime);
    obj.insert("IsDel",user.IsDel);

    return obj;

}
#include <QDebug>
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
    if(obj.contains("Sex")){
        QJsonValue value = obj.value("Sex");
        if(value.isString())
            user.Sex = value.toString();
    }
    if(obj.contains("Cell")){
        QJsonValue value = obj.value("Cell");
        if(value.isString())
            user.Cell = value.toString();
    }
    if(obj.contains("CID")){
        QJsonValue value = obj.value("CID");
        if(value.isString())
            user.CID = value.toString();
    }
    if(obj.contains("CommpanyName")){
        QJsonValue value = obj.value("CommpanyName");
        if(value.isString())
            user.CommpanyName = value.toString();
    }
    if(obj.contains("Department")){
        QJsonValue value = obj.value("Department");
        if(value.isString())
            user.Department = value.toString();
    }
    if(obj.contains("Author")){
        QJsonValue value = obj.value("Author");
        if(value.isDouble())
            user.Author = value.toInt();
    }
    if(obj.contains("InTime")){
        QJsonValue value = obj.value("InTime");
        if(value.isString())
            user.InTime = value.toString();
    }
    if(obj.contains("OutTime")){
        QJsonValue value = obj.value("OutTime");
        if(value.isString())
            user.OutTime = value.toString();
    }
    if(obj.contains("CreatTime")){
        QJsonValue value = obj.value("CreatTime");
        if(value.isString())
            user.CreatTime = value.toString();
    }
    if(obj.contains("Status")){
        QJsonValue value = obj.value("Status");
        if(value.isString())
            user.Status = value.toString();
    }
    if(obj.contains("Salary")){
        QJsonValue value = obj.value("Salary");
        if(value.isDouble())
            user.Salary = value.toInt();
    }
    if(obj.contains("Age")){
        QJsonValue value = obj.value("Age");
        if(value.isDouble())
            user.Age = value.toInt();
    }
    if(obj.contains("Account")){
        QJsonValue value = obj.value("Account");
        if(value.isString())
            user.Account = value.toString();
    }
    if(obj.contains("Code")){
        QJsonValue value = obj.value("Code");
        if(value.isString())
            user.Code = value.toString();
    }

    if(obj.contains("CreatStamp")){
        QJsonValue s = obj.value("CreatStamp");
        if(s.isDouble())
            user.CreatStamp =s.toInt();
    }
    if(obj.contains("LastTime")){
        QJsonValue s = obj.value("LastTime");
        if(s.isDouble())
            user.LastTime =s.toInt();
    }
    if(obj.contains("IsDel")){
        QJsonValue s = obj.value("IsDel");
        if(s.isBool()){
            user.IsDel = s.toBool();
        }
    }

    return user;
}

bool UserService::exportUser(QVector<User> list,QString filepath,int author,bool isOpen)
{  
    QVector<QVariant> datalist;
    datalist<<"用户编号"<<"姓名"<<"性别"<<"联系方式"<<"部门";

    if(author>2){
        datalist<<"账号"<<"密码";
    }

    datalist<<"入职时间"<<"离职时间";
    if(author>1){
        datalist<<"基本工资";
    }

    QVector<QVector<QVariant>> data;
    for(int i=0;i<list.size();++i){
        User user  = list.at(i);
        QVector<QVariant> datalist;
        QString sex;
        if(user.Sex=="0")sex="男";else sex="女";
        datalist<<"'"+user.UID<<"'"+user.Name\
               <<"'"+sex<<user.Cell <<user.Department;
        if(author>2){
            datalist<<"'"+user.Account <<"'"+user.Code;
        }
        datalist<<"'"+user.InTime <<user.OutTime;
        if(author>1){
            datalist<<user.Salary;
        }
        data.push_back(datalist);
    }
    return ExcelService::dataExport(filepath,datalist,data,isOpen);
}

