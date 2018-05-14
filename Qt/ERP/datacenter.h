#ifndef DATACENTER_H
#define DATACENTER_H

#include <QObject>

#include "global.h"
#include "loadding.h"
#include "nsysconfig.h"
class dataCenter : public QObject
{
    Q_OBJECT
private:
    explicit dataCenter(QObject *parent = 0);

public:
    static dataCenter* instance(){
        static dataCenter*u =new dataCenter();
        return u;
    }
    ///////////////////////////////////////////////////////
    void newOrder(const QJsonObject para);

    /////////////////////////////////////////////////////
    void showMessage(QString msg,int sec=0);
    ////////////////////////////////////////////////////
    void showLoadding(QString str="",int sec=4000,QColor c =Qt::black);
    void hideLoadding();
    ////////////////////////////////////////////////////
    void setCurUser(User u);
    User CurUser();
    ////////////////////////////////////////////////////
    QVector<Order> StatusOrders(QString status);
    Order getOrder(QString OrderID);
    ////////////////////////////////////////////////////
    QVector<QString> Units();
    ////////////////////////////////////////////////////
    QVector<Materiel>Materiels();
    bool checkMaterielID(QString id);
    ////////////////////////////////////////////////////
    QVector<QString> Batchs();
    ////////////////////////////////////////////////////

    QVector<Customer>Customers();
signals:
    void showStatusMessage(QString msg,int sec);
    void sig_newPlan(Order,bool);
public slots:

private:
    void appendOrder(Order order);
    void appendUnit(QString u);
    void appendMaters(Materiel m);
    void appendBatch(QString b);
    void appendCustomer(Customer c);
private:
    User  cur_user;
    QVector<Order>     m_orders;     //所有订单
    QVector<QString>   m_units;      //所有计量单位
    QVector<QString>   m_batch;      //所有用户批次
    QVector<Materiel>  m_maters;     //所有物料
    QVector<Customer>  m_customers;  //所有客户
    Loadding           m_load;
    nSysConfig         m_Config;   //保存系统配置
};

#endif // DATACENTER_H
