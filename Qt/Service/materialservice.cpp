#include "materialservice.h"
#include "excelservice.h"
#include "http.h"

MaterialService::MaterialService()
{

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
    obj.insert("CID",ma.CID);
    obj.insert("CustomName",ma.CustomName);
    obj.insert("OrderNum",ma.OrderNum);
    obj.insert("Unit",ma.Unit);
    obj.insert("Status",ma.Status);
    obj.insert("CreatTime",ma.CreatTime);
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
    if(obj.contains("OrderNum")){
        QJsonValue value = obj.value("OrderNum");
        if(value.isDouble())
            ma.OrderNum = value.toInt();
    }
    if(obj.contains("Unit")){
        QJsonValue value = obj.value("Unit");
        if(value.isString())
            ma.Unit = value.toString();
    }
    if(obj.contains("Status")){
        QJsonValue value = obj.value("Status");
        if(value.isString())
            ma.Status = value.toString();
    }
    if(obj.contains("CreatTime")){
        QJsonValue value = obj.value("CreatTime");
        if(value.isString())
            ma.CreatTime = value.toString();
    }
    return ma;
}




bool MaterialService::exportMateriel(QVector<Materiel> list, QString filepath, bool isOpen)
{
    QVector<QVariant> datalist;
    datalist<<"物料编号"<<"物料描述"<<"客户姓名"<<"数量"<<"单位"<<"入库时间"<<"状态";
    QVector<QVector<QVariant>> data;
    for(int i=0;i<list.size();++i){
        Materiel ma  = list.at(i);
        QString status;
        if(ma.Status=="0")
            status="已入库";
        if(ma.Status=="1")
            status="已出库";

        QVector<QVariant> datalist;
        datalist<<"'"+ma.MaterID<<"'"+ma.MaterDes\
               <<"'"+ma.CustomName<<ma.OrderNum\
              <<ma.Unit<<"'"+ma.CreatTime<<status;
        data.push_back(datalist);
    }
    return  ExcelService::dataExport(filepath,datalist,data,isOpen);
}
