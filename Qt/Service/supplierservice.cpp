#include "supplierservice.h"
#include <QJsonArray>
#include "excelservice.h"


SupplierService::SupplierService()
{

}

Supplier SupplierService::newSupplier(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
    Supplier sup;
    ok = true;
    return sup;
}

Supplier SupplierService::modSupplier(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
    Supplier sup;
    ok = true;
    return sup;
}

QJsonObject SupplierService::toJsonObject(Supplier super)
{
    QJsonObject obj;
    obj.insert("SID",super.SID);
    obj.insert("Name",super.Name);
    obj.insert("Icon",super.Icon);
    obj.insert("Addr",super.Addr);
    obj.insert("Tel",super.Tel);
    obj.insert("ContactName",super.ContactName);
    obj.insert("ContactCell",super.ContactCell);
    obj.insert("BankName",super.BankName);
    obj.insert("BankNumber",super.BankNumber);
    obj.insert("Bankbranch",super.Bankbranch);
    obj.insert("CertificatesNum",super.CertificatesNum);
    obj.insert("Certificates",super.Certificates);
    obj.insert("Note",super.Note);
    obj.insert("Goods",super.Goods);
    return obj;
}

Supplier SupplierService::fromJsonObject(QJsonObject obj)
{
    Supplier sup;
    if(obj.contains("SID")){
        QJsonValue value = obj.value("SID");
        if(value.isString())
            sup.SID = value.toString();
    }
    if(obj.contains("Name")){
        QJsonValue value = obj.value("Name");
        if(value.isString())
            sup.Name = value.toString();
    }
    if(obj.contains("Icon")){
        QJsonValue value = obj.value("Icon");
        if(value.isString())
            sup.Icon = value.toString();
    }
    if(obj.contains("Addr")){
        QJsonValue value = obj.value("Addr");
        if(value.isString())
            sup.Addr = value.toString();
    }
    if(obj.contains("Tel")){
        QJsonValue value = obj.value("Tel");
        if(value.isString())
            sup.Tel = value.toString();
    }
    if(obj.contains("ContactName")){
        QJsonValue value = obj.value("ContactName");
        if(value.isString())
            sup.ContactName = value.toString();
    }
    if(obj.contains("ContactCell")){
        QJsonValue value = obj.value("ContactCell");
        if(value.isString())
            sup.ContactCell = value.toString();
    }
    if(obj.contains("BankName")){
        QJsonValue value = obj.value("BankName");
        if(value.isString())
            sup.BankName = value.toString();
    }
    if(obj.contains("BankNumber")){
        QJsonValue value = obj.value("BankNumber");
        if(value.isString())
            sup.BankNumber = value.toString();
    }
    if(obj.contains("Bankbranch")){
        QJsonValue value = obj.value("Bankbranch");
        if(value.isString())
            sup.Bankbranch = value.toString();
    }
    if(obj.contains("CertificatesNum")){
        QJsonValue value = obj.value("CertificatesNum");
        if(value.isString())
            sup.CertificatesNum = value.toString();
    }
    if(obj.contains("Certificates")){
        QJsonValue value = obj.value("Certificates");
        if(value.isString())
            sup.Certificates = value.toString();
    }
    if(obj.contains("Note")){
        QJsonValue value = obj.value("Note");
        if(value.isString())
            sup.Note = value.toString();
    }
    if(obj.contains("Goods")){
        QJsonValue value = obj.value("Goods");
        if(value.isString()){
            sup.Goods = value.toString();
        }
    }

    return sup;
}

bool SupplierService::exportSupplier(QVector<Supplier> list, QString filepath, bool isOpen)
{
    QVector<QVariant> datalist;
    datalist<<"供应商编号"<<"供应商姓名"<<"公司电话"<<"公司地址"\
           <<"联系人"<<"联系电话"<<"开户行"<<"银行卡号"\
          <<"支行名称" <<"税号"<<"供应物"<<"备注";
    QVector<QVector<QVariant>> data;

    for(int i=0;i<list.size();++i){
        Supplier ma  = list.at(i);
        QVector<QVariant> datalist;
        datalist<<"'"+ma.SID<<ma.Name\
               <<"'"+ma.Tel<<ma.Addr\
              <<ma.ContactName<<"'"+ma.ContactCell\
             <<ma.BankName<<"'"+ma.BankNumber\
            <<ma.Bankbranch<<"'"+ma.CertificatesNum\
           <<ma.Goods<<ma.Note;
        data.push_back(datalist);
    }
    return  ExcelService::dataExport(filepath,datalist,data,isOpen);
}
