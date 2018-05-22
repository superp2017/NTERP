#include "orderservice.h"
#include <QJsonArray>
#include <QDebug>
#include "http.h"
#include <QAxObject>
#include <QDir>
#include <QFileDialog>
OrderService::OrderService()
{

}

Order OrderService::newOrder(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
#if 1
    Order order = fromJsonObject(para);
    ok = true;
    return order;
#endif
#if 0
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
        qDebug()<<"newPlan ret is not 0";
    ok = false;
    return order;
#endif
}

Order OrderService::modOrder(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
#if 1
    Order order = fromJsonObject(para);
    ok = true;
    return order;
#endif
#if 0
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
        qDebug()<<"modOrder ret is not 0";
    ok = false;
    return order;
#endif
}

Order OrderService::cancleOrder(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
#if 1
    Order order = fromJsonObject(para);
    ok = true;
    return order;
#endif
#if 0
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
        qDebug()<<"cancleOrder ret is not 0";
    ok = false;
    return order;
#endif
}

Order OrderService::finishOrder(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
#if 1
    Order order = fromJsonObject(para);
    ok = true;
    return order;
#endif
#if 0
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
        qDebug()<<"finishOrder ret is not 0";
    ok = false;
    return order;
#endif
}

Order OrderService::modOrderPrice(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
#if 1
    Order order = fromJsonObject(para);
    ok = true;
    return order;
#endif
#if 0
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
        qDebug()<<"modOrder ret is not 0";
    ok = false;
    return order;
#endif
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

bool OrderService::printOrders(QVector<Order> list)
{

    return true;
}

bool OrderService::exportOrders(QVector<Order> list)
{
    QString filepath= QFileDialog::getSaveFileName(NULL,"Save orders",".","Microsoft Office 2007 (*.xlsx)");//获取保存路径
    if(!filepath.isEmpty()){

        QAxObject *excel = new QAxObject();
        excel->setControl("Excel.Application");//连接Excel控件
        excel->dynamicCall("SetVisible (bool Visible)","false");//不显示窗体
        excel->setProperty("DisplayAlerts", false);//不显示任何警告信息。如果为true那么在关闭是会出现类似“文件已修改，是否保存”的提示

        QAxObject *workbooks = excel->querySubObject("WorkBooks");//获取工作簿集合
        workbooks->dynamicCall("Add");//新建一个工作簿
        QAxObject *workbook   = excel->querySubObject("ActiveWorkBook");//获取当前工作簿
        QAxObject *worksheets = workbook->querySubObject("Sheets");//获取工作表集合
        QAxObject *worksheet  = worksheets->querySubObject("Item(int)",1);//获取工作表集合的工作表1，即sheet1


        QString A="A"+QString::number(1);//设置要操作的单元格，如A1
        QString B="B"+QString::number(1);
        QString C="C"+QString::number(1);
        QString D="D"+QString::number(1);
        QString E="E"+QString::number(1);
        QString F="F"+QString::number(1);
        QString G="G"+QString::number(1);
        QString H="H"+QString::number(1);
        QString I="I"+QString::number(1);
        QString J="J"+QString::number(1);
        QAxObject *cellA = worksheet->querySubObject("Range(QVariant, QVariant)",A);//获取单元格
        QAxObject *cellB = worksheet->querySubObject("Range(QVariant, QVariant)",B);
        QAxObject *cellC = worksheet->querySubObject("Range(QVariant, QVariant)",C);
        QAxObject *cellD = worksheet->querySubObject("Range(QVariant, QVariant)",D);
        QAxObject *cellE = worksheet->querySubObject("Range(QVariant, QVariant)",E);
        QAxObject *cellF = worksheet->querySubObject("Range(QVariant, QVariant)",F);
        QAxObject *cellG = worksheet->querySubObject("Range(QVariant, QVariant)",G);
        QAxObject *cellH = worksheet->querySubObject("Range(QVariant, QVariant)",H);
        QAxObject *cellI = worksheet->querySubObject("Range(QVariant, QVariant)",I);
        QAxObject *cellJ = worksheet->querySubObject("Range(QVariant, QVariant)",J);

        cellA->dynamicCall("SetValue(const QVariant&)",QVariant("生产订单"));//设置单元格的值
        cellB->dynamicCall("SetValue(const QVariant&)",QVariant("物料编码"));
        cellC->dynamicCall("SetValue(const QVariant&)",QVariant("物料描述"));
        cellD->dynamicCall("SetValue(const QVariant&)",QVariant("订单数量"));
        cellE->dynamicCall("SetValue(const QVariant&)",QVariant("单位"));
        cellF->dynamicCall("SetValue(const QVariant&)",QVariant("客户批次"));
        cellG->dynamicCall("SetValue(const QVariant&)",QVariant("客户备注"));
        cellH->dynamicCall("SetValue(const QVariant&)",QVariant("生产批号"));
        cellI->dynamicCall("SetValue(const QVariant&)",QVariant("价格(元)"));
        cellJ->dynamicCall("SetValue(const QVariant&)",QVariant("状态"));


        for(int i=0;i<list.size();++i){
            Order order  = list.at(i);
            int cellrow = i+2;

            QString A="A"+QString::number(cellrow);//设置要操作的单元格，如A1
            QString B="B"+QString::number(cellrow);
            QString C="C"+QString::number(cellrow);
            QString D="D"+QString::number(cellrow);
            QString E="E"+QString::number(cellrow);
            QString F="F"+QString::number(cellrow);
            QString G="G"+QString::number(cellrow);
            QString H="H"+QString::number(cellrow);
            QString I="I"+QString::number(cellrow);
            QString J="J"+QString::number(cellrow);
            QAxObject *cellA = worksheet->querySubObject("Range(QVariant, QVariant)",A);//获取单元格
            QAxObject *cellB = worksheet->querySubObject("Range(QVariant, QVariant)",B);
            QAxObject *cellC = worksheet->querySubObject("Range(QVariant, QVariant)",C);
            QAxObject *cellD = worksheet->querySubObject("Range(QVariant, QVariant)",D);
            QAxObject *cellE = worksheet->querySubObject("Range(QVariant, QVariant)",E);
            QAxObject *cellF = worksheet->querySubObject("Range(QVariant, QVariant)",F);
            QAxObject *cellG = worksheet->querySubObject("Range(QVariant, QVariant)",G);
            QAxObject *cellH = worksheet->querySubObject("Range(QVariant, QVariant)",H);
            QAxObject *cellI = worksheet->querySubObject("Range(QVariant, QVariant)",I);
            QAxObject *cellJ = worksheet->querySubObject("Range(QVariant, QVariant)",J);


            cellA->dynamicCall("SetValue(const QVariant&)",QVariant("'"+order.OrderID));//设置单元格的值
            cellB->dynamicCall("SetValue(const QVariant&)",QVariant("'"+order.MaterielID));
            cellC->dynamicCall("SetValue(const QVariant&)",QVariant("'"+order.MaterielDes));
            cellD->dynamicCall("SetValue(const QVariant&)",QVariant(order.OrderNum));
            cellE->dynamicCall("SetValue(const QVariant&)",QVariant(order.Unit));
            cellF->dynamicCall("SetValue(const QVariant&)",QVariant("'"+order.CustomBatch));
            cellG->dynamicCall("SetValue(const QVariant&)",QVariant("'"+order.CustomNote));
            cellH->dynamicCall("SetValue(const QVariant&)",QVariant("'"+order.ProduceID));
            cellI->dynamicCall("SetValue(const QVariant&)",QVariant(order.Money));
            QString status;
            if(order.Current.Status=="Status_New"){
                status="新建";
            }
            if(order.Current.Status=="Status_Success"){
                status="已出货";
            }
            if(order.Current.Status=="Status_Cancle"){
                status="已取消";
            }
            cellJ->dynamicCall("SetValue(const QVariant&)",QVariant(status));
        }

        QString merge_cell = QString("A1:I%1").arg(list.size()+1); // 设置A1至Z10范围内的单元格的属性
        QAxObject *merge_range = worksheet->querySubObject("Range(const QString&)", merge_cell);
        merge_range->setProperty("HorizontalAlignment", -4108); // 水平居中
        merge_range->setProperty("VerticalAlignment", -4108);  // 垂直居中
        merge_range->setProperty("NumberFormatLocal", "@");  // 设置为文本
        merge_range->setProperty("WrapText", true);


        workbook->dynamicCall("SaveAs(const QString&)",QDir::toNativeSeparators(filepath));//保存至filepath，注意一定要用QDir::toNativeSeparators将路径中的"/"转换为"\"，不然一定保存不了。
        workbook->dynamicCall("Close()");//关闭工作簿
        excel->dynamicCall("Quit()");//关闭excel
        delete excel;
        excel=NULL;
        return true;
    }
    return false;
}

