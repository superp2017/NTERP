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
    void newUser(const QJsonObject para);
    void modUser(const QJsonObject para);
    void outUser(const QJsonObject para);
    void delUser(const QJsonObject para);
    ///////////////////////////////////////////////////////
    void newOrder(const QJsonObject para);
    void modOrder(const QJsonObject para);
    void cancleOrder(const QJsonObject para);
    void finishOrder(const QJsonObject para);
    void modOrderPrice(const QJsonObject para);
    /////////////////////////////////////////////////////
    void newCustomer(const QJsonObject para);
    void modCustomer(const QJsonObject para);
    void delCustomer(const QJsonObject para);
    /////////////////////////////////////////////////////
    void newUnit(const QJsonObject para);
    void delUnit(const QJsonObject para);

    /////////////////////////////////////////////////////
    /////////////////////////////////////////////////////
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
    Order getOrder(QString OrderID,bool &ok);
    ////////////////////////////////////////////////////
    QVector<QString> Units();
    bool checkUnitExist(QString unit);
    ////////////////////////////////////////////////////
    QVector<Materiel>Materiels();
    bool checkMaterielID(QString id);
    Materiel getMateriel(QString MID,bool &ok);
    ////////////////////////////////////////////////////
    QVector<QString> Batchs();
    ////////////////////////////////////////////////////
    QVector<Customer>Customers();
    bool checkCustomerExist(QString name);
    Customer getCustomer(QString CID,bool &ok);
    ////////////////////////////////////////////////////
    QVector<QString> getDepartments() const;
    QVector<QString> getAuthors() const;
    ////////////////////////////////////////////////////
    QVector<User> employees()const;
    User getUser(QString UID,bool &ok);
signals:
    void showStatusMessage(QString msg,int sec);
    ///////////////////////////////////////////
    void sig_newOrder(Order,bool);
    void sig_modOrder(Order,bool);
    void sig_cancleOrder(Order,bool);
    void sig_finishOrder(Order,bool);
    void sig_modOrderPrice(Order,bool);
    //////////////////////////////////////
    void sig_newEmployee(User,bool);
    void sig_modEmployee(User,bool);
    void sig_outEmployee(User,bool);
    void sig_delEmployee(User,bool);
    ////////////////////////////////////
    void sig_newCustomer(Customer,bool);
    void sig_modCustomer(Customer,bool);
    void sig_delCustomer(Customer,bool);
    ////////////////////////////////////
    void sig_newUnit(QString,bool);
    void sig_delUnit(QString,bool);
public slots:

private:
    void appendEmployee(User user);
    void appendOrder(Order order);
    void appendUnit(QString u);
    void appendMaters(Materiel m);
    void appendBatch(QString b);
    void appendCustomer(Customer c);
private:
    User               cur_user;     //当前登录的账号
    QVector<User>      m_employee;   //所有的员工
    QVector<Order>     m_orders;     //所有订单
    QVector<QString>   m_units;      //所有计量单位
    QVector<QString>   m_batch;      //所有用户批次
    QVector<Materiel>  m_maters;     //所有物料
    QVector<Customer>  m_customers;  //所有客户
    QVector<QString>   m_authors;    //所有的权限
    QVector<QString>   m_departments;//所有的部门
    Loadding           m_load;       //加载动画
    nSysConfig         m_Config;     //保存系统配置
};

#endif // DATACENTER_H
