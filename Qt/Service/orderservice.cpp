#include "orderservice.h"
#include <QJsonArray>
OrderService::OrderService()
{

}

Order OrderService::newOrder(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
   Order order;
    ok = true;
    return order;
}


QJsonObject OrderService::toJsonObject(Order order)
{
    QJsonObject obj;
    obj.insert("UID",order.UID);
    obj.insert("UserName",order.UserName);
    obj.insert("OrderID",order.OrderID);
    obj.insert("OrderType",order.OrderType);
    obj.insert("MaterielID",order.MaterielID);
    obj.insert("MaterielDes",order.MaterielDes);
    obj.insert("Unit",order.Unit);
    obj.insert("CustomID",order.CustomID);
    obj.insert("CustomName",order.CustomName);
    obj.insert("CustomBatch",order.CustomBatch);
    obj.insert("CustomNote",order.CustomNote);
    obj.insert("ProduceID",order.ProduceID);
    obj.insert("SuccessTime",order.SuccessTime);
    obj.insert("CreatTime",order.CreatTime);
    obj.insert("OrderNum",order.OrderNum);
    obj.insert("Money",order.Money);
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

