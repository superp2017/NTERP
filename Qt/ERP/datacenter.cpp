#include "datacenter.h"
#include  "orderservice.h"
#include <QDebug>
#include <QDateTime>


dataCenter::dataCenter(QObject *parent) : QObject(parent)
{
    cur_user.UID="1234";
    cur_user.Author = "admin";
    cur_user.Name = "admin";

    m_units.append("EA");
    m_units.append("KG");
    m_batch.append("180410001");
    m_batch.append("180410002");
    m_batch.append("180410003");
    m_batch.append("180410004");
    m_batch.append("132645004");
    m_batch.append("52516464");
    Materiel m ;
    m.MaterID = "61013567";
    m.MaterDes="IMCN/5261097/银白色锌铝涂层";
    m_maters.append(m);

    Materiel m1 ;
    m1.MaterID = "610134207";
    m1.MaterDes="C924/项目12法兰焊接/银白色锌铝涂层";
    m_maters.append(m1);

    Materiel m2 ;
    m2.MaterID = "61013112";
    m2.MaterDes="IMCN/5261097/银白色锌铝涂层";
    m_maters.append(m2);

    Materiel m3 ;
    m3.MaterID = "61013204";
    m3.MaterDes="C407/0SEA003061/黑色锌铝涂层";
    m_maters.append(m3);

    Customer c1;
    c1.CID = "C102";
    c1.Name="上海某某科技公司";

    Customer c2;
    c2.CID = "C103";
    c2.Name="昆山某某科技公司";

    Customer c3;
    c3.CID = "C102";
    c3.Name="南通某某科技公司";
    m_customers.append(c1);
    m_customers.append(c2);
    m_customers.append(c3);
}

void dataCenter::newOrder(const QJsonObject para)
{
#if 0
    bool isOK   = false;
    Order order = OrderService::newOrder(para,isOK,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(isOK){
        this->appendOrder(order);
        emit sig_newPlan(order,true);
        return;
    }
    emit sig_newPlan(order,false);
#endif
#if 1
    Order order = OrderService::fromJsonObject(para);
    order.OrderID = QString("%1").arg(QDateTime::currentDateTime().toMSecsSinceEpoch());
    order.ProduceID =  QString("%1").arg(QDateTime::currentDateTime().toMSecsSinceEpoch()/130);
    order.CreatTime = QDateTime::currentDateTime().toString("yyyy:MM:dd hh:mm:ss");
    OderFlow flow;
    flow.UserName =cur_user.Name;
    flow.Action="新建订单";
    flow.Status = Status_New;
    flow.OpreatTime =order.CreatTime;
    order.Current = flow;
    order.Flow.push_back(flow);



    this->appendOrder(order);
    emit sig_newPlan(order,true);
#endif
}

void dataCenter::showMessage(QString msg, int sec)
{
    emit showStatusMessage(msg,sec);
}


void dataCenter::showLoadding(QString str,int sec,QColor c )
{
    m_load.showTime(str,sec,c);
}

void dataCenter::hideLoadding()
{
    m_load.showOver();
}


void dataCenter::setCurUser(User u)
{
    cur_user = u;
}

User dataCenter::CurUser()
{
    return cur_user;
}

QVector<Order> dataCenter::StatusOrders(QString status)
{
    QVector<Order > ls;
    if (status==""){
        return ls;
    }
    if(status==Status_All){
        return m_orders;
    }

    for(Order o: m_orders) {
        if (o.Current.Status.compare(status)==0){
            ls.append(o);
        }
    }
    return ls;
}

//获取单个订单
Order dataCenter::getOrder(QString OrderID, bool &ok)
{
    Order a;
    for(Order o: m_orders) {
        if (o.OrderID.compare(OrderID)==0){
            ok = true;
            return o;
        }
    }
    ok = false;
    return a;
}

void dataCenter::appendOrder(Order order)
{
    m_orders.append(order);
}

void dataCenter::appendUnit(QString u)
{
    m_units.append(u);
}

QVector<QString> dataCenter::Units()
{
    return m_units;
}

bool dataCenter::checkUnitExist(QString unit)
{
    for(QString m:m_units){
        if (m==unit){
            return true;
        }
    }
    return false;
}

void dataCenter::appendMaters(Materiel m)
{
    m_maters.append(m);
}

QVector<Materiel> dataCenter::Materiels()
{
    return m_maters;
}

bool dataCenter::checkMaterielID(QString id)
{
    for(Materiel m:m_maters){
        if (m.MaterID==id){
            return true;
        }
    }
    return false;
}

void dataCenter::appendBatch(QString b)
{
    m_batch.append(b);
}

QVector<QString> dataCenter::Batchs()
{
    return m_batch;
}


void dataCenter::appendCustomer(Customer c)
{
    m_customers.append(c);
}

QVector<Customer> dataCenter::Customers()
{
    return m_customers;
}

bool dataCenter::checkCustomerExist(QString name)
{
    for(Customer m:m_customers){
        if (m.Name==name){
            return true;
        }
    }
    return false;
}
