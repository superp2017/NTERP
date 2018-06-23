#include "orderservice.h"
#include <QJsonArray>
#include <QDebug>
#include "http.h"
#include <QAxObject>
#include <QDir>
#include <QFileDialog>
#include "excelservice.h"
#include "boost/thread.hpp"

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

QString OrderService::delOrder(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
    QString order;
    std::string url = Net_DelOrder;
    bool r   = false;
    Ret ret  = Http::fetch(url,para,r,hostname,hostport);
    if(r&&ret.ret){
        if(ret.data.isString()){
            order = ret.data.toString();
            ok = true;
            return  order;
        }
    }
    if(!ret.ret)
        qDebug()<<"delOrder ret is not 0"<<endl;
    ok = false;
    return order;
}

QVector<Order> OrderService::getAllOrders(bool &ok,QString hostname, QString hostport)
{
    QVector<Order> data;
    std::string url = Net_GlobalOrders;
    bool r   = false;
    Ret ret  = Http::fetch(url,QJsonObject(),r,hostname,hostport);
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


QJsonObject OrderService::toJsonObject(Order order)
{
    QJsonObject obj;
    obj.insert("UID",order.UID);
    obj.insert("UserName",order.UserName);
    obj.insert("OrderID",order.OrderID);
    obj.insert("OrderType",order.OrderType);
    obj.insert("Factory",order.Factory);
    obj.insert("FactoryNumber",order.FactoryNumber);
    obj.insert("MaterielID",order.MaterielID);
    obj.insert("MaterielDes",order.MaterielDes);
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
    obj.insert("Money",order.Money);
    obj.insert("TotleMoney",order.TotleMoney);

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
            order.OrderNum = value.toInt();
    }
    if(obj.contains("Money")){
        QJsonValue value = obj.value("Money");
        if(value.isDouble())
            order.Money = value.toInt();
    }

    if(obj.contains("TotleMoney")){
        QJsonValue value = obj.value("TotleMoney");
        if(value.isDouble())
            order.TotleMoney = value.toInt();
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

    return order;
}



bool OrderService::exportOrders(QVector<Order> list, QString filepath, bool isOpen)
{
    QVector<QVariant> datalist;
    datalist<<"生产批号"<<"分厂名称"<<"物料描述"<<"订单数量"<<"单位"<<"客户名称"<<"客户备注"<<"价格(元)"<<"状态"<<"创建时间";
    QVector<QVector<QVariant>> data;
    for(int i=0;i<list.size();++i){
        Order order  = list.at(i);
        QString status;
        if(order.Current.Status=="Status_New")
            status="新建";
        if(order.Current.Status=="Status_Success")
            status="已出货";
        if(order.Current.Status=="Status_Cancle")
            status="已取消";

        QVector<QVariant> datalist;
        datalist<<"'"+order.OrderID<<order.Factory\
               <<"'"+order.MaterielDes<<"'"+order.OrderNum\
              <<order.Unit<<order.CustomName<<"'"+order.CustomNote\
              <<"'"+QString("%1").arg(order.Money/100.0)<<status<<"'"+order.CreatTime;
        data.push_back(datalist);
    }
    return  ExcelService::dataExport(filepath,datalist,data,isOpen);
}


