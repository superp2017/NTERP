#include "datacenter.h"

dataCenter::dataCenter(QObject *parent) : QObject(parent)
{

}

void dataCenter::setCurUser(User *u)
{
    cur_user = u;
}

User* dataCenter::CurUser()
{
    return cur_user;
}

QVector<Order *> dataCenter::getStatusOrder(QString status)
{
    QVector<Order *> ls;
    if (status==""){
        return ls;
    }
    if(status==Status_All){
        return m_orders;
    }

    foreach (Order *o, m_orders) {
        if (o->Current.Status.compare(status)==0){
            ls.append(o);
        }
    }
    return ls;
}

//获取单个订单
Order *dataCenter::getOrder(QString OrderID)
{
    foreach (Order *o, m_orders) {
        if (o->OrderID.compare(OrderID)==0){
            return o;
        }
    }
    return NULL;
}

void dataCenter::appendOrder(Order *order)
{
    m_orders.append(order);
}
