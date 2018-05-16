﻿#include "customerservice.h"
#include "http.h"
#include "orderservice.h"
#include <QDebug>
CustomerService::CustomerService()
{

}

Customer CustomerService::newCustomer(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
#if 1
     Customer cus;
     ok = true;
     return cus;
#endif
#if 0
    Customer cus;
    std::string url = Net_NewCustomer;
    bool r   = false;
    Ret ret  = Http::fetch(url,para,r,hostname,hostport);
    if(r&&ret.ret){
        if(ret.data.isObject()){
            cus = fromJsonObject(ret.data.toObject());
            ok = true;
            return  cus;
        }
    }
    if(!ret.ret)
        qDebug()<<"newCustomer ret is not 0";
    ok = false;
    return cus;
#endif
}

QJsonObject CustomerService::toJsonObject(Customer customer)
{
    QJsonObject obj;
    obj.insert("CID",customer.CID);
    obj.insert("Name",customer.Name);
    obj.insert("Icon",customer.Icon);
    obj.insert("Addr",customer.Addr);
    obj.insert("Tel",customer.Tel);
    obj.insert("ContactName",customer.ContactName);
    obj.insert("ContactCell",customer.ContactCell);
    obj.insert("BankName",customer.BankName);
    obj.insert("BankNumber",customer.BankNumber);
    obj.insert("Bankbranch",customer.Bankbranch);
    obj.insert("CertificatesNum",customer.CertificatesNum);
    obj.insert("Certificates",customer.Certificates);
    obj.insert("Note",customer.Note);
    return obj;
}

Customer CustomerService::fromJsonObject(QJsonObject obj)
{
    Customer customer;
    if(obj.contains("CID")){
        QJsonValue value = obj.value("CID");
        if(value.isString())
            customer.CID = value.toString();
    }
    if(obj.contains("Name")){
        QJsonValue value = obj.value("Name");
        if(value.isString())
            customer.Name = value.toString();
    }
    if(obj.contains("Icon")){
        QJsonValue value = obj.value("Icon");
        if(value.isString())
            customer.Icon = value.toString();
    }
    if(obj.contains("Addr")){
        QJsonValue value = obj.value("Addr");
        if(value.isString())
            customer.Addr = value.toString();
    }
    if(obj.contains("Tel")){
        QJsonValue value = obj.value("Tel");
        if(value.isString())
            customer.Tel = value.toString();
    }
    if(obj.contains("ContactName")){
        QJsonValue value = obj.value("ContactName");
        if(value.isString())
            customer.ContactName = value.toString();
    }
    if(obj.contains("ContactCell")){
        QJsonValue value = obj.value("ContactCell");
        if(value.isString())
            customer.ContactCell = value.toString();
    }
    if(obj.contains("BankName")){
        QJsonValue value = obj.value("BankName");
        if(value.isString())
            customer.BankName = value.toString();
    }
    if(obj.contains("BankNumber")){
        QJsonValue value = obj.value("BankNumber");
        if(value.isString())
            customer.BankNumber = value.toString();
    }
    if(obj.contains("Bankbranch")){
        QJsonValue value = obj.value("Bankbranch");
        if(value.isString())
            customer.Bankbranch = value.toString();
    }
    if(obj.contains("CertificatesNum")){
        QJsonValue value = obj.value("CertificatesNum");
        if(value.isString())
            customer.CertificatesNum = value.toString();
    }
    if(obj.contains("Certificates")){
        QJsonValue value = obj.value("Certificates");
        if(value.isString())
            customer.Certificates = value.toString();
    }
    if(obj.contains("Note")){
        QJsonValue value = obj.value("Note");
        if(value.isString())
            customer.Note = value.toString();
    }
    return customer;
}