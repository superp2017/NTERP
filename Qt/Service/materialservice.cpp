#include "materialservice.h"
#include "excelservice.h"
#include "http.h"

MaterialService::MaterialService()
{

}

Materiel MaterialService::newMaterial(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
    std::string url = Net_NewMaterial;
    bool r   = false;
    Materiel c;
    Ret ret  = Http::fetch(url,para,r,hostname,hostport);
    if(r&&ret.ret){
        if(ret.data.isObject()){
            c = fromJsonObject(ret.data.toObject());
            ok = true;
            return c;
        }
    }
    if(!ret.ret)
        qDebug()<<"newMaterial ret is not 0"<<endl;
    ok = false;
    return c;
}

Materiel MaterialService::modMaterial(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
    std::string url = Net_ModMaterial;
    Materiel c;
    bool r   = false;
    Ret ret  = Http::fetch(url,para,r,hostname,hostport);
    if(r&&ret.ret){
        if(ret.data.isObject()){
            c = fromJsonObject(ret.data.toObject());
            ok = true;
            return c;
        }
    }
    if(!ret.ret)
        qDebug()<<"modMaterial ret is not 0"<<endl;
    ok = false;
    return c;
}

QString MaterialService::delMaterial(const QJsonObject para, bool &ok,QString hostname, QString hostport)
{
    std::string url = Net_DelMaterial;
    QString c;
    bool r   = false;
    Ret ret  = Http::fetch(url,para,r,hostname,hostport);
    if(r&&ret.ret){
        if(ret.data.isString()){
            c = ret.data.toString();
        }
        ok = true;
        return c;
    }
    if(!ret.ret)
        qDebug()<<"delMaterial ret is not 0"<<endl;
    ok = false;
    return c;
}

Materiel MaterialService::queryMaterial(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
    std::string url = Net_QueryMaterial;
    Materiel c;
    bool r   = false;
    Ret ret  = Http::fetch(url,para,r,hostname,hostport);
    if(r&&ret.ret){
        if(ret.data.isObject()){
            c = fromJsonObject(ret.data.toObject());
            ok = true;
            return c;
        }
    }
    if(!ret.ret)
        qDebug()<<"queryMaterial ret is not 0"<<endl;
    ok = false;
    return c;
}

QVector<QString> MaterialService::getCustomerMaterID(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
    QVector<QString> data;
    std::string url = Net_GetCustomerMaterial;
    bool r   = false;
    Ret ret  = Http::fetch(url,QJsonObject(),r,hostname,hostport);
    if(r&&ret.ret){
        if(ret.data.isArray()){
            for(QJsonValue v:ret.data.toArray()){
                QString  c = v.toString();
                data.push_back(c);
            }
            ok = true;
            return  data;
        }
    }
    if(!ret.ret)
        qDebug()<<"getAllMateriels ret is not 0"<<endl;
    ok = false;
    return data;
}


QVector<Materiel> MaterialService::getAllMateriels(bool &ok, QString hostname, QString hostport)
{
    QVector<Materiel> data;
    std::string url = Net_GetAllMateril;
    bool r   = false;
    Ret ret  = Http::fetch(url,QJsonObject(),r,hostname,hostport);
    if(r&&ret.ret){
        if(ret.data.isArray()){
            for(QJsonValue v:ret.data.toArray()){
                Materiel  c = fromJsonObject(v.toObject());
                data.push_back(c);
            }
            ok = true;
            return  data;
        }
    }
    if(!ret.ret)
        qDebug()<<"getAllMateriels ret is not 0"<<endl;
    ok = false;
    return data;
}



QJsonObject MaterialService::toJsonObject(Materiel ma)
{
    QJsonObject obj;
    obj.insert("MaterID",ma.MaterID);
    obj.insert("MaterDes",ma.MaterDes);
    obj.insert("Plating",ma.Plating);
    obj.insert("Friction",ma.Friction);
    obj.insert("Thickness",ma.Thickness);
    obj.insert("Salt",ma.Salt);
    obj.insert("ComponentSolid",ma.ComponentSolid);
    obj.insert("ComponentFormat",ma.ComponentFormat);
    obj.insert("CID",ma.CID);
    obj.insert("CustomName",ma.CustomName);
    obj.insert("CreatTime",ma.CreatTime);
    obj.insert("Factory",ma.Factory);
    obj.insert("FactoryNumber",ma.FactoryNumber);
    obj.insert("ProductionLine",ma.ProductionLine);
    obj.insert("Unit",ma.Unit);
    obj.insert("Money",ma.Money);
    return obj;
}

Materiel MaterialService::fromJsonObject(QJsonObject obj)
{
    Materiel ma;
    if(obj.contains("MaterID")){
        QJsonValue value = obj.value("MaterID");
        if(value.isString())
            ma.MaterID = value.toString();
    }
    if(obj.contains("MaterDes")){
        QJsonValue value = obj.value("MaterDes");
        if(value.isString())
            ma.MaterDes = value.toString();
    }
    if(obj.contains("Plating")){
        QJsonValue value = obj.value("Plating");
        if(value.isString())
            ma.Plating = value.toString();
    }
    if(obj.contains("Friction")){
        QJsonValue value = obj.value("Friction");
        if(value.isString())
            ma.Friction = value.toString();
    }
    if(obj.contains("Thickness")){
        QJsonValue value = obj.value("Thickness");
        if(value.isString())
            ma.Thickness = value.toString();
    }
    if(obj.contains("Salt")){
        QJsonValue value = obj.value("Salt");
        if(value.isString())
            ma.Salt = value.toString();
    }
    if(obj.contains("ComponentSolid")){
        QJsonValue value = obj.value("ComponentSolid");
        if(value.isString())
            ma.ComponentSolid = value.toString();
    }
    if(obj.contains("ComponentFormat")){
        QJsonValue value = obj.value("ComponentFormat");
        if(value.isString())
            ma.ComponentFormat = value.toString();
    }
    if(obj.contains("CID")){
        QJsonValue value = obj.value("CID");
        if(value.isString())
            ma.CID = value.toString();
    }
    if(obj.contains("CustomName")){
        QJsonValue value = obj.value("CustomName");
        if(value.isString())
            ma.CustomName = value.toString();
    }
    if(obj.contains("CreatTime")){
        QJsonValue value = obj.value("CreatTime");
        if(value.isString())
            ma.CreatTime = value.toString();
    }
    if(obj.contains("Factory")){
        QJsonValue value = obj.value("Factory");
        if(value.isString())
            ma.Factory = value.toString();
    }
    if(obj.contains("FactoryNumber")){
        QJsonValue value = obj.value("FactoryNumber");
        if(value.isString())
            ma.FactoryNumber = value.toString();
    }
    if(obj.contains("ProductionLine")){
        QJsonValue value = obj.value("ProductionLine");
        if(value.isString())
            ma.ProductionLine = value.toString();
    }
    if(obj.contains("Unit")){
        QJsonValue value = obj.value("Unit");
        if(value.isString())
            ma.Unit = value.toString();
    }
    if(obj.contains("Money")){
        QJsonValue value = obj.value("Money");
        if(value.isDouble())
            ma.Money = value.toInt();
    }
    return ma;
}



bool MaterialService::exportMateriel(QVector<Materiel> list, QString filepath, bool isOpen)
{
    QVector<QVariant> datalist;
    datalist<<"物料编号"<<"物料描述"<<"客户姓名"<<"未税单价"<<"单位"<<"创建时间";
    QVector<QVector<QVariant>> data;
    for(int i=0;i<list.size();++i){
        Materiel ma  = list.at(i);
        QVector<QVariant> datalist;
        datalist<<"'"+ma.MaterID<<"'"+ma.MaterDes\
               <<"'"+ma.CustomName<<+"'"+QString("%1").arg(ma.Money/100.0)\
              <<ma.Unit<<"'"+ma.CreatTime;
        data.push_back(datalist);
    }
    return  ExcelService::dataExport(filepath,datalist,data,isOpen);
}
