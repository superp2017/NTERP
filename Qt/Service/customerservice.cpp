#include "customerservice.h"
#include "http.h"
#include "orderservice.h"
#include <QDebug>
#include "excelservice.h"



CustomerService::CustomerService()
{

}

Customer CustomerService::newCustomer(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
#if 0
    Customer cus;
    ok = true;
    return cus;
#endif
#if 1
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
        qDebug()<<"newCustomer ret is not 0"<<endl;
    ok = false;
    return cus;
#endif
}

Customer CustomerService::modCustomer(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
#if 0
    Customer cus;
    ok = true;
    return cus;
#endif
#if 1
    Customer cus;
    std::string url = Net_ModCustomer;
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
        qDebug()<<"modCustomer ret is not 0"<<endl;
    ok = false;
    return cus;
#endif
}

QString CustomerService::delCustomer(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
    QString cus;
    std::string url = Net_DelCustomer;
    bool r   = false;
    Ret ret  = Http::fetch(url,para,r,hostname,hostport);
    if(r && ret.ret){
        if(ret.data.isString()){
            cus = ret.data.toString();
            ok = true;
            return  cus;
        }
    }
    if(!ret.ret)
        qDebug()<<"delCustomer ret is not 0"<<endl;
    ok = false;
    return cus;
}

Customer CustomerService::upDownCustomer(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
    Customer cus;
    std::string url = Net_upDownCustomer;
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
        qDebug()<<"upDownCustomer ret is not 0"<<endl;
    ok = false;
    return cus;
}

QVector<Customer> CustomerService::getAllCustomer(bool &ok, QString hostname, QString hostport)
{
    QVector<Customer> cus;
    std::string url = Net_GlobalCustomers;
    bool r   = false;
    Ret ret  = Http::fetch(url,QJsonObject(),r,hostname,hostport);
    if(r&&ret.ret){
        if(ret.data.isArray()){
            for(QJsonValue v:ret.data.toArray()){
                Customer  c = fromJsonObject(v.toObject());
                cus.push_back(c);
            }
            ok = true;
            return  cus;
        }
    }
    if(!ret.ret)
        qDebug()<<"getAllCustomer ret is not 0"<<endl;
    ok = false;
    return cus;
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
    obj.insert("Status",customer.Status);
    obj.insert("CreatTime",customer.CreatTime);
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
    if(obj.contains("Status")){
        QJsonValue value = obj.value("Status");
        if(value.isString())
            customer.Status = value.toString();
    }
    if(obj.contains("CreatTime")){
        QJsonValue value = obj.value("CreatTime");
        if(value.isString())
            customer.CreatTime = value.toString();
    }
    return customer;
}

bool CustomerService::exportCustomer(QVector<Customer> list, QString filepath,bool isOpen)
{
    QVector<QVariant> datalist;
    datalist<<"客户编号"<<"客户姓名"<<"公司地址"\
           <<"联系人"<<"联系电话"<<"开户行"<<"银行卡号"\
          <<"支行名称" <<"税号"<<"备注";
    QVector<QVector<QVariant>> data;

    for(int i=0;i<list.size();++i){
        Customer ma  = list.at(i);
        QVector<QVariant> datalist;
        datalist<<"'"+ma.CID<<ma.Name<<ma.Addr\
              <<ma.ContactName<<"'"+ma.ContactCell\
             <<ma.BankName<<"'"+ma.BankNumber\
            <<ma.Bankbranch<<"'"+ma.CertificatesNum\
           <<ma.Note;
        data.push_back(datalist);
    }
    return  ExcelService::dataExport(filepath,datalist,data,isOpen);
}
