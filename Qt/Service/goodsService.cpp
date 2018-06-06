#include "goodsService.h"

GoodsService::GoodsService()
{

}

Goods GoodsService::newGoods(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{

}

Goods GoodsService::modGoods(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{

}

Goods GoodsService::delGoods(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{

}

QVector<Goods> GoodsService::getAllGoods(bool &ok, QString hostname, QString hostport)
{

}

QJsonObject GoodsService::toJsonObject(Goods goods)
{
    QJsonObject obj;
    obj.insert("ID",goods.ID);
    obj.insert("Name",goods.Name);
    obj.insert("Unit",goods.Unit);
    obj.insert("Format",goods.Format);
    obj.insert("Color",goods.Color);
    obj.insert("SID",goods.SID);
    obj.insert("SupplierName",goods.SupplierName);
    obj.insert("Status",goods.Status);
    obj.insert("Note",goods.Note);
    obj.insert("CreatTime",goods.CreatTime);
    obj.insert("TotalPrice",goods.TotalPrice);
    obj.insert("Price",goods.Price);
    obj.insert("Num",goods.Num);
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
    if(obj.contains("Format")){
        QJsonValue value = obj.value("Format");
        if(value.isString())
            goods.Format = value.toString();
    }
    if(obj.contains("Color")){
        QJsonValue value = obj.value("Color");
        if(value.isString())
            goods.Color = value.toString();
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
    if(obj.contains("Status")){
        QJsonValue value = obj.value("Status");
        if(value.isString())
            goods.Status = value.toString();
    }
    if(obj.contains("Note")){
        QJsonValue value = obj.value("Note");
        if(value.isString())
            goods.Note = value.toString();
    }
    if(obj.contains("CreatTime")){
        QJsonValue value = obj.value("CreatTime");
        if(value.isString())
            goods.CreatTime = value.toString();
    }
    if(obj.contains("Note")){
        QJsonValue value = obj.value("Note");
        if(value.isString())
            goods.Note = value.toString();
    }
    if(obj.contains("TotalPrice")){
        QJsonValue value = obj.value("TotalPrice");
        if(value.isDouble())
            order.TotalPrice = value.toInt();
    }
    if(obj.contains("Price")){
        QJsonValue value = obj.value("Price");
        if(value.isDouble())
            goods.Price = value.toInt();
    }
    if(obj.contains("Num")){
        QJsonValue value = obj.value("Num");
        if(value.isDouble())
            goods.Num = value.toInt();
    }
    return goods;
}

bool GoodsService::exportGoods(QVector<Goods> list, QString filepath, bool isOpen)
{
    QVector<QVariant> datalist;
    datalist<<"商品编号"<<"商品名称"<<"类别"\
           <<"进价(元)"<<"库存数量"<<"单位"<<"库存总价(元)"\
          <<"规格"<<"颜色"<<"供应商"<<"状态"<<"备注";
    QVector<QVector<QVariant>> data;
    for(int i=0;i<list.size();++i){
        Goods goods  = list.at(i);
        QVector<QVariant> datalist;
        datalist<<"'"+goods.ID<<"'"+goods.Name<<goods.Type\
               <<"'"+goods.Price<<"'"+goods.Num<<goods.Unit\
             <<"'"+goods.TotalPrice<<goods.Format<<goods.Color\
            <<goods.SupplierName<<goods.Status<<goods.Goods;
        data.push_back(datalist);
    }
    return  ExcelService::dataExport(filepath,datalist,data,isOpen);

}
