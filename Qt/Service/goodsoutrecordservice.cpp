#include "goodsoutrecordservice.h"
#include "excelservice.h"
#include <QJsonObject>
#include "http.h"
GoodsOutRecordService::GoodsOutRecordService()
{

}

GoodsOutRecord GoodsOutRecordService::newGoodsRecord(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
    GoodsOutRecord goods;
    std::string url = Net_NewGoodsOutRecord;
    bool r   = false;
    Ret ret  = Http::fetch(url,para,r,hostname,hostport);
    if(r&&ret.ret){
        if(ret.data.isObject()){
            goods = fromJsonObject(ret.data.toObject());
            ok = true;
            return  goods;
        }
    }
    if(!ret.ret)
        qDebug()<<"newGoods ret is not 0"<<endl;
    ok = false;
    return goods;
}

QVector<GoodsOutRecord> GoodsOutRecordService::GetAllRecords(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
    QVector<GoodsOutRecord> data;
    std::string url = Net_GetAllGoodsOutRecord;
    bool r   = false;
    Ret ret  = Http::fetch(url,para,r,hostname,hostport);
    if(r&&ret.ret){
        if(ret.data.isArray()){
            QJsonArray arr = ret.data.toArray();
            for(QJsonValue v:arr){
                GoodsOutRecord r = fromJsonObject(v.toObject());
                data.push_back(r);
            }
        }
        ok = true;
        return data;
    }
    if(!ret.ret)
        qDebug()<<"GetAllRecords ret is not 0"<<endl;
    ok = false;
    return data;
}

QJsonObject GoodsOutRecordService::toJsonObject(GoodsOutRecord record)
{
    QJsonObject obj;
    obj.insert("OutID",record.OutID);
    obj.insert("Factory",record.Factory);
    obj.insert("Department",record.Department);
    obj.insert("CreatDate",record.CreatDate);
    obj.insert("GoodsID",record.GoodsID);
    obj.insert("GoodsName",record.GoodsName);
    obj.insert("Type",record.Type);
    obj.insert("StrorageName",record.StrorageName);
    obj.insert("Nums",record.Nums);
    obj.insert("Unit",record.Unit);
    obj.insert("Note",record.Note);
    obj.insert("UserName",record.UserName);
    obj.insert("UserID",record.UserID);
    return obj;
}

GoodsOutRecord GoodsOutRecordService::fromJsonObject(QJsonObject obj)
{
    GoodsOutRecord record;
    if(obj.contains("OutID")){
        QJsonValue value = obj.value("OutID");
        if(value.isString())
            record.OutID = value.toString();
    }
    if(obj.contains("Factory")){
        QJsonValue value = obj.value("Factory");
        if(value.isString())
            record.Factory = value.toString();
    }
    if(obj.contains("Department")){
        QJsonValue value = obj.value("Department");
        if(value.isString())
            record.Department = value.toString();
    }
    if(obj.contains("CreatDate")){
        QJsonValue value = obj.value("CreatDate");
        if(value.isString())
            record.CreatDate = value.toString();
    }
    if(obj.contains("GoodsID")){
        QJsonValue value = obj.value("GoodsID");
        if(value.isString())
            record.GoodsID = value.toString();
    }
    if(obj.contains("GoodsName")){
        QJsonValue value = obj.value("GoodsName");
        if(value.isString())
            record.GoodsName = value.toString();
    }
    if(obj.contains("Type")){
        QJsonValue value = obj.value("Type");
        if(value.isString())
            record.Type = value.toString();
    }
    if(obj.contains("StrorageName")){
        QJsonValue value = obj.value("StrorageName");
        if(value.isString())
            record.StrorageName = value.toString();
    }
    if(obj.contains("Nums")){
        QJsonValue value = obj.value("Nums");
        if(value.isDouble())
            record.Nums = value.toDouble();
    }
    if(obj.contains("Unit")){
        QJsonValue value = obj.value("Unit");
        if(value.isString())
            record.Unit = value.toString();
    }
    if(obj.contains("Note")){
        QJsonValue value = obj.value("Note");
        if(value.isString())
            record.Note = value.toString();
    }
    if(obj.contains("UserName")){
        QJsonValue value = obj.value("UserName");
        if(value.isString())
            record.UserName = value.toString();
    }
    if(obj.contains("UserID")){
        QJsonValue value = obj.value("UserID");
        if(value.isString())
            record.UserID = value.toString();
    }
    return record;
}

bool GoodsOutRecordService::exportGoodsRecord(QVector<GoodsOutRecord> list, QString filepath, bool isOpen)
{
    QVector<QVariant> datalist;
    datalist<<"凭证号"<<"分厂"<<"领用部门"<<"凭证日期"\
          <<"物料号"<<"物料描述"<<"数量"<<"单位"\
         <<"物料组"<<"库存地点"<<"领用人工号"<<"领用人姓名";
    QVector<QVector<QVariant>> data;
    for(int i=0;i<list.size();++i){
        GoodsOutRecord record  = list.at(i);
        QVector<QVariant> datalist;
        datalist<<"'"+record.OutID<<record.Factory<<record.Department\
               <<"'"+record.CreatDate<<record.GoodsID<<record.GoodsName\
              <<QString("%1").arg(record.Nums)<<record.Unit<<record.Type\
             <<record.StrorageName<<record.UserID<<record.UserName;
        data.push_back(datalist);
    }
    return  ExcelService::dataExport(filepath,datalist,data,isOpen);
}
