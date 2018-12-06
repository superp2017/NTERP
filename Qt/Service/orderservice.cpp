#include "orderservice.h"
#include <QJsonArray>
#include <QDebug>
#include "http.h"
#include <QAxObject>
#include <QDir>
#include <QFileDialog>
#include "excelservice.h"
#include "boost/thread.hpp"
#include <QDateTime>


OrderService::OrderService()
{

}

Order OrderService::newOrder(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
    Order order;
    std::string url = Net_NewOrder;
    bool r   = false;
    Ret ret  = Http::fetch(url,para,r,hostname,hostport);
    if(r&&ret.ret){
        if(ret.data.isObject()){
            order = fromJsonObject(ret.data.toObject());
            ok = true;
            return  order;
        }
    }
    if(!ret.ret)
        qDebug()<<"newOrder ret is not 0"<<endl;
    ok = false;
    return order;
}

Order OrderService::modOrder(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
    Order order;
    std::string url = Net_ModOrder;
    bool r   = false;
    Ret ret  = Http::fetch(url,para,r,hostname,hostport);
    if(r&&ret.ret){
        if(ret.data.isObject()){
            order = fromJsonObject(ret.data.toObject());
            ok = true;
            return  order;
        }
    }
    if(!ret.ret)
        qDebug()<<"modOrder ret is not 0"<<endl;
    ok = false;
    return order;
}

Order OrderService::cancleOrder(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
    Order order;
    std::string url = Net_CancelOrder;
    bool r   = false;
    Ret ret  = Http::fetch(url,para,r,hostname,hostport);
    if(r&&ret.ret){
        if(ret.data.isObject()){
            order = fromJsonObject(ret.data.toObject());
            ok = true;
            return  order;
        }
    }
    if(!ret.ret)
        qDebug()<<"cancleOrder ret is not 0"<<endl;
    ok = false;
    return order;
}

Order OrderService::produceOrder(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
    Order order;
    std::string url = Net_ProductOrder;
    bool r   = false;
    Ret ret  = Http::fetch(url,para,r,hostname,hostport);
    if(r&&ret.ret){
        if(ret.data.isObject()){
            order = fromJsonObject(ret.data.toObject());
            ok = true;
            return  order;
        }
    }
    if(!ret.ret)
        qDebug()<<"produceOrder ret is not 0"<<endl;
    ok = false;
    return order;
}

Order OrderService::finishOrder(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
    Order order;
    std::string url = Net_SuccessOrder;
    bool r   = false;
    Ret ret  = Http::fetch(url,para,r,hostname,hostport);
    if(r&&ret.ret){
        if(ret.data.isObject()){
            order = fromJsonObject(ret.data.toObject());
            ok = true;
            return  order;
        }
    }
    if(!ret.ret)
        qDebug()<<"finishOrder ret is not 0"<<endl;
    ok = false;
    return order;
}

Order OrderService::modOrderPrice(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
    Order order;
    std::string url = Net_ModOrderPrice;
    bool r   = false;
    Ret ret  = Http::fetch(url,para,r,hostname,hostport);
    if(r&&ret.ret){
        if(ret.data.isObject()){
            order = fromJsonObject(ret.data.toObject());
            ok = true;
            return  order;
        }
    }
    if(!ret.ret)
        qDebug()<<"modOrderPrice ret is not 0"<<endl;
    ok = false;
    return order;
}

Order OrderService::delOrder(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
    Order order;
    std::string url = Net_DelOrder;
    bool r   = false;
    Ret ret  = Http::fetch(url,para,r,hostname,hostport);
    if(r&&ret.ret){
        if(ret.data.isObject()){
            order = fromJsonObject(ret.data.toObject());
            ok = true;
            return  order;
        }
    }
    if(!ret.ret)
        qDebug()<<"delOrder ret is not 0"<<endl;
    ok = false;
    return order;
}

QVector<Order> OrderService::getAllOrders(const QJsonObject para, bool &ok,QString hostname, QString hostport)
{
    QVector<Order> data;
    std::string url = Net_GlobalOrders;
    bool r   = false;
    Ret ret  = Http::fetch(url,para,r,hostname,hostport);
    if(r&&ret.ret){
        if(ret.data.isArray()){
            QJsonArray arr = ret.data.toArray();
            for(QJsonValue v:arr){
                Order r = fromJsonObject(v.toObject());
                data.push_back(r);
            }
        }
        ok = true;
        return data;
    }
    if(!ret.ret)
        qDebug()<<"getAllOrders ret is not 0"<<endl;
    ok = false;
    return data;
}

QVector<Order> OrderService::updatePrintNum(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
    QVector<Order> list;
    std::string url = Net_ModPrintNum;
    bool r   = false;
    Ret ret  = Http::fetch(url,para,r,hostname,hostport);
    if(r&&ret.ret){
        if(ret.data.isArray()){
            QJsonArray arr = ret.data.toArray();
            for(QJsonValue v:arr){
                Order r = fromJsonObject(v.toObject());
                list.push_back(r);
            }
        }
        ok = true;
        return  list;
    }
    if(!ret.ret)
        qDebug()<<"modOrderPrice ret is not 0"<<endl;
    ok = false;
    return list;
}

QString OrderService::setPrintNumber(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
    QString num =0;
    std::string url = Net_SetPrintNum;
    bool r   = false;
    Ret ret  = Http::fetch(url,para,r,hostname,hostport);
    if(r&&ret.ret){
        if(ret.data.isString()){
            num = ret.data.toString();
        }
        ok = true;
        return  num;
    }
    if(!ret.ret)
        qDebug()<<"modOrderPrice ret is not 0"<<endl;
    ok = false;
    return "20189999";
}


QJsonObject OrderService::toJsonObject(Order order)
{
    QJsonObject obj;
    obj.insert("UID",order.UID);
    obj.insert("UserName",order.UserName);
    obj.insert("OrderID",order.OrderID);
    obj.insert("OrderType",order.OrderType);
    obj.insert("Factory",order.Factory);
    obj.insert("FactoryNumber",order.FactoryNumber);
    obj.insert("ProductionLine",order.ProductionLine);
    obj.insert("MaterielID",order.MaterielID);
    obj.insert("MaterielDes",order.MaterielDes);
    obj.insert("Plating",order.Plating);
    obj.insert("Friction",order.Friction);
    obj.insert("Thickness",order.Thickness);
    obj.insert("Salt",order.Salt);
    obj.insert("ComponentSolid",order.ComponentSolid);
    obj.insert("ComponentFormat",order.ComponentFormat);
    obj.insert("Unit",order.Unit);
    obj.insert("CustomID",order.CustomID);
    obj.insert("CustomName",order.CustomName);
    obj.insert("CustomBatch",order.CustomBatch);
    obj.insert("CustomNote",order.CustomNote);
    obj.insert("ProduceID",order.ProduceID);
    obj.insert("SuccessTime",order.SuccessTime);
    obj.insert("ProduceTime",order.ProduceTime);
    obj.insert("CreatTime",order.CreatTime);
    obj.insert("OrderNum",order.OrderNum);
    obj.insert("ProduceNum",order.ProduceNum);
    obj.insert("SuccessNum",order.SuccessNum);
    obj.insert("Money",order.Money);
    obj.insert("TotleMoney",order.TotleMoney);
    obj.insert("PrintNum",order.PrintNum);
    QJsonArray flow;
    for (OderFlow s: order.Flow) {
        QJsonObject m;
        m.insert("UserName",s.UserName);
        m.insert("OpreatTime",s.OpreatTime);
        m.insert("Action",s.Action);
        m.insert("Status",s.Status);
        flow.append(m);
    }
    obj.insert("Flow",flow);
    QJsonObject cur;
    cur.insert("UserName",order.Current.UserName);
    cur.insert("OpreatTime",order.Current.OpreatTime);
    cur.insert("Action",order.Current.Action);
    cur.insert("Status",order.Current.Status);
    obj.insert("Current",cur);

    QJsonArray print;
    for (PrintDetail s: order.Print) {
        QJsonObject m;
        m.insert("UserName",s.UserName);
        m.insert("UserID",s.UserID);
        m.insert("PrintDate",s.PrintDate);
        flow.append(m);
    }
    obj.insert("Print",print);

    obj.insert("CreatStamp",order.CreatStamp);
    obj.insert("LastTime",order.LastTime);

    return obj;
}

Order OrderService::fromJsonObject(QJsonObject obj)
{
    Order order;
    if(obj.contains("UID")){
        QJsonValue value = obj.value("UID");
        if(value.isString())
            order.UID = value.toString();
    }
    if(obj.contains("UserName")){
        QJsonValue value = obj.value("UserName");
        if(value.isString())
            order.UserName = value.toString();
    }
    if(obj.contains("OrderID")){
        QJsonValue value = obj.value("OrderID");
        if(value.isString())
            order.OrderID = value.toString();
    }

    if(obj.contains("Factory")){
        QJsonValue value = obj.value("Factory");
        if(value.isString())
            order.Factory = value.toString();
    }
    if(obj.contains("FactoryNumber")){
        QJsonValue value = obj.value("FactoryNumber");
        if(value.isString())
            order.FactoryNumber = value.toString();
    }
    if(obj.contains("ProductionLine")){
        QJsonValue value = obj.value("ProductionLine");
        if(value.isString())
            order.ProductionLine = value.toString();
    }
    if(obj.contains("OrderType")){
        QJsonValue value = obj.value("OrderType");
        if(value.isString())
            order.OrderType = value.toString();
    }
    if(obj.contains("MaterielID")){
        QJsonValue value = obj.value("MaterielID");
        if(value.isString())
            order.MaterielID = value.toString();
    }
    if(obj.contains("MaterielDes")){
        QJsonValue value = obj.value("MaterielDes");
        if(value.isString())
            order.MaterielDes = value.toString();
    }

    if(obj.contains("Plating")){
        QJsonValue value = obj.value("Plating");
        if(value.isString())
            order.Plating = value.toString();
    }
    if(obj.contains("Friction")){
        QJsonValue value = obj.value("Friction");
        if(value.isString())
            order.Friction = value.toString();
    }
    if(obj.contains("Thickness")){
        QJsonValue value = obj.value("Thickness");
        if(value.isString())
            order.Thickness = value.toString();
    }
    if(obj.contains("Salt")){
        QJsonValue value = obj.value("Salt");
        if(value.isString())
            order.Salt = value.toString();
    }
    if(obj.contains("ComponentSolid")){
        QJsonValue value = obj.value("ComponentSolid");
        if(value.isString())
            order.ComponentSolid = value.toString();
    }
    if(obj.contains("ComponentFormat")){
        QJsonValue value = obj.value("ComponentFormat");
        if(value.isString())
            order.ComponentFormat = value.toString();
    }

    if(obj.contains("Unit")){
        QJsonValue value = obj.value("Unit");
        if(value.isString())
            order.Unit = value.toString();
    }
    if(obj.contains("CustomID")){
        QJsonValue value = obj.value("CustomID");
        if(value.isString())
            order.CustomID = value.toString();
    }
    if(obj.contains("CustomName")){
        QJsonValue value = obj.value("CustomName");
        if(value.isString())
            order.CustomName = value.toString();
    }
    if(obj.contains("CustomBatch")){
        QJsonValue value = obj.value("CustomBatch");
        if(value.isString())
            order.CustomBatch = value.toString();
    }
    if(obj.contains("CustomNote")){
        QJsonValue value = obj.value("CustomNote");
        if(value.isString())
            order.CustomNote = value.toString();
    }
    if(obj.contains("ProduceID")){
        QJsonValue value = obj.value("ProduceID");
        if(value.isString())
            order.ProduceID = value.toString();
    }
    if(obj.contains("SuccessTime")){
        QJsonValue value = obj.value("SuccessTime");
        if(value.isString())
            order.SuccessTime = value.toString();
    }
    if(obj.contains("CreatTime")){
        QJsonValue value = obj.value("CreatTime");
        if(value.isString())
            order.CreatTime = value.toString();
    }
    if(obj.contains("ProduceTime")){
        QJsonValue value = obj.value("ProduceTime");
        if(value.isString())
            order.ProduceTime = value.toString();
    }

    if(obj.contains("OrderNum")){
        QJsonValue value = obj.value("OrderNum");
        if(value.isDouble())
            order.OrderNum = value.toDouble();
    }

    if(obj.contains("ProduceNum")){
        QJsonValue value = obj.value("ProduceNum");
        if(value.isDouble())
            order.ProduceNum = value.toDouble();
    }

    if(obj.contains("SuccessNum")){
        QJsonValue value = obj.value("SuccessNum");
        if(value.isDouble())
            order.SuccessNum = value.toDouble();
    }

    if(obj.contains("Money")){
        QJsonValue value = obj.value("Money");
        if(value.isDouble())
            order.Money = value.toDouble();
    }

    if(obj.contains("TotleMoney")){
        QJsonValue value = obj.value("TotleMoney");
        if(value.isDouble())
            order.TotleMoney = value.toDouble();
    }


    if(obj.contains("PrintNum")){
        QJsonValue value = obj.value("PrintNum");
        if(value.isDouble())
            order.PrintNum = value.toInt();
    }

    if(obj.contains("Flow")){
        QJsonValue value = obj.value("Flow");
        if(value.isArray()){
            QJsonArray array = value.toArray();
            for(QJsonValue v:array){
                OderFlow flow;
                if(v.isObject()){
                    QJsonObject o = v.toObject();
                    if(o.contains("UserName")){
                        QJsonValue s = o.value("UserName");
                        if(s.isString())
                            flow.UserName = s.toString();
                    }
                    if(o.contains("OpreatTime")){
                        QJsonValue s = o.value("OpreatTime");
                        if(s.isString())
                            flow.OpreatTime = s.toString();
                    }
                    if(o.contains("Action")){
                        QJsonValue s = o.value("Action");
                        if(s.isString())
                            flow.Action = s.toString();
                    }
                    if(o.contains("Status")){
                        QJsonValue s = o.value("Status");
                        if(s.isString())
                            flow.Status = s.toString();
                    }
                    order.Flow.append(flow);
                }
            }
        }
    }

    if(obj.contains("Current")){
        QJsonValue v = obj.value("Current");
        if(v.isObject()){
            QJsonObject o = v.toObject();
            if(o.contains("UserName")){
                QJsonValue s = o.value("UserName");
                if(s.isString())
                    order.Current.UserName = s.toString();
            }
            if(o.contains("OpreatTime")){
                QJsonValue s = o.value("OpreatTime");
                if(s.isString())
                    order.Current.OpreatTime = s.toString();
            }
            if(o.contains("Action")){
                QJsonValue s = o.value("Action");
                if(s.isString())
                    order.Current.Action = s.toString();
            }
            if(o.contains("Status")){
                QJsonValue s = o.value("Status");
                if(s.isString())
                    order.Current.Status = s.toString();
            }
        }
    }


    if(obj.contains("Print")){
        QJsonValue value = obj.value("Print");
        if(value.isArray()){
            QJsonArray array = value.toArray();
            for(QJsonValue v:array){
                PrintDetail detail;
                if(v.isObject()){
                    QJsonObject o = v.toObject();
                    if(o.contains("UserName")){
                        QJsonValue s = o.value("UserName");
                        if(s.isString())
                            detail.UserName = s.toString();
                    }
                    if(o.contains("UserID")){
                        QJsonValue s = o.value("UserID");
                        if(s.isString())
                            detail.UserID = s.toString();
                    }
                    if(o.contains("PrintDate")){
                        QJsonValue s = o.value("PrintDate");
                        if(s.isString())
                            detail.PrintDate = s.toString();
                    }
                    order.Print.append(detail);
                }
            }
        }
    }


    if(obj.contains("CreatStamp")){
        QJsonValue s = obj.value("CreatStamp");
        if(s.isDouble())
            order.CreatStamp =s.toInt();
    }
    if(obj.contains("LastTime")){
        QJsonValue s = obj.value("LastTime");
        if(s.isDouble())
            order.LastTime =s.toInt();
    }

    return order;
}



bool OrderService::exportOrders(QString curstatus,QVector<Order> list, QString filepath, int author, bool isOpen)
{
    QVector<QVariant> datalist;
    datalist<<"分厂名称"<<"产线名称"<<"生产批号"<<"订单类型"<<"客户名称"<<"物料描述"<<"订单数量"<<"单位";
    if(curstatus=="Status_New") {
        datalist<<"未成品";
    }
    if(curstatus=="Status_Success") {
        datalist<<"已出库";
    }
    if(curstatus=="Status_Produce") {
        datalist<<"已成品"<<"未出库";
    }
    if(curstatus=="Status_All") {
        datalist<<"未成品"<<"已成品"<<"未出库"<<"已出库";
    }

    datalist<<"客户批次"<<"客户备注";
    if(author>1)
        datalist<<"未税单价(元)"<<"未税总价(元)";
    datalist<<"状态"<<"创建时间";

    QVector<QVector<QVariant>> data;
    for(int i=0;i<list.size();++i){
        Order order  = list.at(i);
        QString status;
        if(curstatus=="Status_New")
            status="新建";
        if(curstatus=="Status_Produce")
            status="已成品";
        if(curstatus=="Status_Success")
            status="已出货";

        if(curstatus=="Status_Cancle")
            status="已取消";
        if(curstatus=="Status_All")
        {
            if(order.Current.Status=="Status_Cancle"){
                status="已取消";
            }
            if(order.Current.Status=="Status_PartSuccess"){
                status="部分出库";
            }
            if(order.Current.Status=="Status_PartProduce"){
                status="部分成品";
            }
            if(order.Current.Status=="Status_New"){
                status="新建";
            }
            if(order.Current.Status=="Status_Produce"){
                status="全部成品";
            }
            if(order.Current.Status=="Status_Success"){
                status="全部出库";
            }
            if(order.Current.Status=="Status_Part_Part"){
                status="部分成品部分出库";
            }
        }

        QString type;
        if(order.OrderType=="1"){
            type="普通订单";
        }
        if(order.OrderType=="2"){
            type="试样订单";
        }
        if(order.OrderType=="3"){
            type="返工订单";
        }

        QVector<QVariant> datalist;
        datalist<<order.Factory<<order.ProductionLine<<"'"+order.OrderID<<type<<order.CustomName\
               <<"'"+order.MaterielDes<<QString("%1").arg(order.OrderNum)<<order.Unit;
        if(curstatus=="Status_New")
            datalist<<"'"+QString("%1").arg(order.OrderNum-order.ProduceNum);
        if(curstatus=="Status_Produce"){
            datalist<<"'"+QString("%1").arg(order.ProduceNum);
            datalist<<"'"+QString("%1").arg(order.ProduceNum-order.SuccessNum);
        }
        if(curstatus=="Status_Success"){
            datalist<<"'"+QString("%1").arg(order.SuccessNum);
        }
        if(curstatus=="Status_All"){
            datalist<<"'"+QString("%1").arg(order.OrderNum-order.ProduceNum);
            datalist<<"'"+QString("%1").arg(order.ProduceNum);
            datalist<<"'"+QString("%1").arg(order.ProduceNum-order.SuccessNum);
            datalist<<"'"+QString("%1").arg(order.SuccessNum);
        }

        datalist<<"'"+order.CustomBatch<<"'"+order.CustomNote;
        if(author>=2){
            datalist<<"'"+QString("%1").arg(order.Money)<<"'"+QString("%1").arg(order.TotleMoney);
        }
        datalist<<status <<"'"+QDateTime::fromString(order.CreatTime,"yyyy-MM-dd HH:mm:ss").toString("yyyy-MM-dd");
        data.push_back(datalist);
    }
    return  ExcelService::dataExport(filepath,datalist,data,isOpen);
}

bool OrderService::exportOrdersEx(QVector<Order> list, QString filepath, bool isOpen)
{
    QVector<QVariant> datalist;
    datalist<<"生产批号"<<"订单类型"<<"分厂名称"<<"客户名称"<<"物料描述"\
           <<"订单总量"<<"单位"<<"未税单价(元)"<<"未税总价(元)"<<"状态"<<"创建时间";

    QVector<QVector<QVariant>> data;
    double money = 0;
    for(int i=0;i<list.size();++i){
        Order order  = list.at(i);
        QString status;

        if(order.Current.Status=="Status_Cancle"){
            status="已取消";
        }
        if(order.Current.Status=="Status_PartSuccess"){
            status="部分出库";
        }
        if(order.Current.Status=="Status_PartProduce"){
            status="部分成品";
        }
        if(order.Current.Status=="Status_New"){
            status="新建";
        }
        if(order.Current.Status=="Status_Produce"){
            status="全部成品";
        }
        if(order.Current.Status=="Status_Success"){
            status="全部出库";
        }
        if(order.Current.Status=="Status_Part_Part"){
            status="部分成品部分出库";
        }

        QString type;
        if(order.OrderType=="1"){
            type="普通订单";
        }
        if(order.OrderType=="2"){
            type="试样订单";
        }
        if(order.OrderType=="3"){
            type="返工订单";
        }

        QVector<QVariant> datalist;
        datalist<<"'"+order.OrderID<<type<<order.Factory<<order.CustomName\
               <<"'"+order.MaterielDes<<QString("%1").arg(order.OrderNum)<<order.Unit\
              <<"'"+QString("%1").arg(order.Money)<<"'"+QString("%1").arg(order.TotleMoney)\
             <<status<<order.CreatTime;
        data.push_back(datalist);
        money+=order.TotleMoney;
    }
    QVector<QVector<QVariant>> res;
    QVector<QVariant> m;
    for(int i=0;i<8;++i){
        m.push_back("");
    }
    m.push_back("总金额");
    m.push_back(QString("%1").arg(money));

    QVector<QVariant> n;
    for(int i=0;i<8;++i){
        n.push_back("");
    }
    n.push_back("订单数量");
    n.push_back(QString("%1").arg(list.size()));
    res.push_back(n);
    res.push_back(m);

    return  ExcelService::dataExportEx(filepath,datalist,data,res,isOpen);
}

