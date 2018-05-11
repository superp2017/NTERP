#include "datacenter.h"

dataCenter::dataCenter(QObject *parent) : QObject(parent)
{

    cur_user = new User();
    cur_user->UID="1234";
    cur_user->Author = "admin";
    cur_user->Name = "admin";

    m_units.append("EA");
    m_units.append("KG");
    m_batch.append("180410001");
    m_batch.append("180410002");
    m_batch.append("180410003");
    m_batch.append("180410004");
    m_batch.append("132645004");
    m_batch.append("52516464");
    Materiel* m = new Materiel();
    m->MaterID = "61013567";
    m->MaterDes="IMCN/5261097/银白色锌铝涂层";
    m_maters.append(m);

    Materiel* m1 = new Materiel();
    m1->MaterID = "610134207";
    m1->MaterDes="C924/项目12法兰焊接/银白色锌铝涂层";
    m_maters.append(m1);

    Materiel* m2 = new Materiel();
    m2->MaterID = "61013112";
    m2->MaterDes="IMCN/5261097/银白色锌铝涂层";
    m_maters.append(m2);

    Materiel* m3 = new Materiel();
    m3->MaterID = "61013204";
    m3->MaterDes="C407/0SEA003061/黑色锌铝涂层";
    m_maters.append(m3);

    Customer* c1=new Customer();
    c1->CID = "C102";
    c1->Name="上海某某科技公司";

    Customer* c2=new Customer();
    c2->CID = "C103";
    c2->Name="昆山某某科技公司";

    Customer* c3=new Customer();
    c3->CID = "C102";
    c3->Name="南通某某科技公司";
    m_customers.append(c1);
    m_customers.append(c2);
    m_customers.append(c3);
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



void dataCenter::setCurUser(User *u)
{
    cur_user = u;
}

User* dataCenter::CurUser()
{
    return cur_user;
}

QVector<Order *> dataCenter::StatusOrders(QString status)
{
    QVector<Order *> ls;
    if (status==""){
        return ls;
    }
    if(status==Status_All){
        return m_orders;
    }

    for(Order *o: m_orders) {
        if (o->Current.Status.compare(status)==0){
            ls.append(o);
        }
    }
    return ls;
}

//获取单个订单
Order *dataCenter::getOrder(QString OrderID)
{
    for(Order *o: m_orders) {
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

void dataCenter::appendUnit(QString u)
{
    m_units.append(u);
}

QVector<QString> dataCenter::Units()
{
    return m_units;
}

void dataCenter::appendMaters(Materiel *m)
{
    m_maters.append(m);
}

QVector<Materiel *> dataCenter::Materiels()
{
    return m_maters;
}

bool dataCenter::checkMaterielID(QString id)
{
    for(Materiel *m:m_maters){
        if (m->MaterID==id){
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


void dataCenter::appendCustomer(Customer *c)
{
    m_customers.append(c);
}

QVector<Customer *> dataCenter::Customers()
{
    return m_customers;
}
