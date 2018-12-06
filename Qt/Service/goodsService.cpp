#include "goodsService.h"
#include "http.h"
#include "excelservice.h"


GoodsService::GoodsService()
{

}

Goods GoodsService::newGoods(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
    Goods goods;
    std::string url = Net_NewGoods;
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

Goods GoodsService::getGoods(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
    Goods goods;
    std::string url = Net_QueryGoods;
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
        qDebug()<<"getGoods ret is not 0"<<endl;
    ok = false;
    return goods;
}

Goods GoodsService::modGoods(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
    Goods goods;
    std::string url = Net_ModifyGoods;
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
        qDebug()<<"modGoods ret is not 0"<<endl;
    ok = false;
    return goods;
}

Goods GoodsService::addOutGoodsNum(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
    Goods goods;
    std::string url = Net_InGoodsNum;
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
        qDebug()<<"inOutGoods ret is not 0"<<endl;
    ok = false;
    return goods;
}

Goods GoodsService::delGoods(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
    Goods goods;
    std::string url = Net_DelGoods;
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
        qDebug()<<"delGoods ret is not 0"<<endl;
    ok = false;
    return goods;
}

QVector<Goods> GoodsService::getAllGoods(const QJsonObject para,bool &ok, QString hostname, QString hostport)
{
    QVector<Goods> data;
    std::string url = Net_GlobalGoods;
    bool r   = false;
    Ret ret  = Http::fetch(url,para,r,hostname,hostport);
    if(r&&ret.ret){
        if(ret.data.isArray()){
            QJsonArray arr = ret.data.toArray();
            for(QJsonValue v:arr){
                Goods r = fromJsonObject(v.toObject());
                data.push_back(r);
            }
        }
        ok = true;
        return data;
    }
    if(!ret.ret)
        qDebug()<<"getAllGoods ret is not 0"<<endl;
    ok = false;
    return data;
}

QVector<Goods> GoodsService::getSupplierGoods(bool &ok, QString hostname, QString hostport)
{
    QVector<Goods> data;
    std::string url = Net_SupplierGoods;
    bool r   = false;
    Ret ret  = Http::fetch(url,QJsonObject(),r,hostname,hostport);
    if(r&&ret.ret){
        if(ret.data.isArray()){
            QJsonArray arr = ret.data.toArray();
            for(QJsonValue v:arr){
                Goods r = fromJsonObject(v.toObject());
                data.push_back(r);
            }
        }
        ok = true;
        return data;
    }
    if(!ret.ret)
        qDebug()<<"getSupplierGoods ret is not 0"<<endl;
    ok = false;
    return data;
}

QString GoodsService::newGoodsType(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
    QString  type;
    std::string url = Net_NewGoodsType;
    bool r   = false;
    Ret ret  = Http::fetch(url,para,r,hostname,hostport);
    if(r&&ret.ret){
        if(ret.data.isString()){
            type = ret.data.toString();
            ok = true;
            return  type;
        }
    }
    if(!ret.ret)
        qDebug()<<"newGoodsType ret is not 0"<<endl;
    ok = false;
    return type;
}

QString GoodsService::delGoodsType(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
    QString  type;
    std::string url = Net_RemoveGoodsType;
    bool r   = false;
    Ret ret  = Http::fetch(url,para,r,hostname,hostport);
    if(r&&ret.ret){
        if(ret.data.isString()){
            type = ret.data.toString();
            ok = true;
            return  type;
        }
    }
    if(!ret.ret)
        qDebug()<<"delGoodsType ret is not 0"<<endl;
    ok = false;
    return type;
}

QVector<QString> GoodsService::getAllGoodsType(bool &ok, QString hostname, QString hostport)
{
    QVector<QString> list;
    std::string url = Net_GetAllGoodsType;
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
        qDebug()<<"getAllGoodsType ret is not 0"<<endl;
    ok = false;
    return list;
}


QJsonObject GoodsService::toJsonObject(Goods goods)
{
    QJsonObject obj;
    obj.insert("ID",goods.ID);
    obj.insert("Name",goods.Name);
    obj.insert("Type",goods.Type);
    obj.insert("Unit",goods.Unit);
    obj.insert("Format",goods.Format);
    obj.insert("SID",goods.SID);
    obj.insert("SupplierName",goods.SupplierName);
    obj.insert("CreatTime",goods.CreatTime);
    obj.insert("Num",goods.Num);
    obj.insert("CreatStamp",goods.CreatStamp);
    obj.insert("LastTime",goods.LastTime);
    obj.insert("IsDel",goods.IsDel);
    return obj;
}

Goods GoodsService::fromJsonObject(QJsonObject obj)
{
    Goods goods;
    if(obj.contains("ID")){
        QJsonValue value = obj.value("ID");
        if(value.isString())
            goods.ID = value.toString();
    }
    if(obj.contains("Name")){
        QJsonValue value = obj.value("Name");
        if(value.isString())
            goods.Name = value.toString();
    }
    if(obj.contains("Unit")){
        QJsonValue value = obj.value("Unit");
        if(value.isString())
            goods.Unit = value.toString();
    }
    if(obj.contains("Type")){
        QJsonValue value = obj.value("Type");
        if(value.isString())
            goods.Type = value.toString();
    }
    if(obj.contains("Format")){
        QJsonValue value = obj.value("Format");
        if(value.isString())
            goods.Format = value.toString();
    }
    if(obj.contains("SID")){
        QJsonValue value = obj.value("SID");
        if(value.isString())
            goods.SID = value.toString();
    }
    if(obj.contains("SupplierName")){
        QJsonValue value = obj.value("SupplierName");
        if(value.isString())
            goods.SupplierName = value.toString();
    }
    if(obj.contains("CreatTime")){
        QJsonValue value = obj.value("CreatTime");
        if(value.isString())
            goods.CreatTime = value.toString();
    }
    if(obj.contains("Num")){
        QJsonValue value = obj.value("Num");
        if(value.isDouble())
            goods.Num = value.toDouble();
    }
    if(obj.contains("CreatStamp")){
        QJsonValue s = obj.value("CreatStamp");
        if(s.isDouble())
            goods.CreatStamp =s.toInt();
    }
    if(obj.contains("LastTime")){
        QJsonValue s = obj.value("LastTime");
        if(s.isDouble())
            goods.LastTime =s.toInt();
    }
    if(obj.contains("IsDel")){
        QJsonValue s = obj.value("IsDel");
        if(s.isBool()){
            goods.IsDel = s.toBool();
        }
    }
    return goods;
}

bool GoodsService::exportGoods(QVector<Goods> list, QString filepath, bool isOpen)
{
    QVector<QVariant> datalist;
    datalist<<"商品编号"<<"商品名称"<<"类别"<<"规格"\
          <<"库存数量"<<"单位"<<"供应商";
    QVector<QVector<QVariant>> data;
    for(int i=0;i<list.size();++i){
        Goods goods  = list.at(i);
        QVector<QVariant> datalist;
        datalist<<"'"+goods.ID<<"'"+goods.Name<<goods.Type\
              <<goods.Format<<QString("%1").arg(goods.Num)\
             <<goods.Unit<<goods.SupplierName;
        data.push_back(datalist);
    }
    return  ExcelService::dataExport(filepath,datalist,data,isOpen);
}
