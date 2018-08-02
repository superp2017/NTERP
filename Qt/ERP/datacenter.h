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
    void initData();
    void net_login(const QJsonObject para);
    ///////////////////////////////////////////////////////
    void net_newUser(const QJsonObject para);
    void net_modUser(const QJsonObject para);
    void net_outUser(const QJsonObject para);
    void net_delUser(const QJsonObject para);
    void net_getGlobalUsers();
    //////////////////////////////////////////////////////
    void net_newDepartment(const QJsonObject para);
    void net_delDepartment(const QJsonObject para);
    void net_getGlobalDepartment();
    ///////////////////////////////////////////////////////
    void net_newOrder(const QJsonObject para);
    void net_modOrder(const QJsonObject para);
    void net_cancleOrder(const QJsonObject para);
    void net_delOrder(const QJsonObject para);
    void net_produceOrder(const QJsonObject para);
    void net_finishOrder(const QJsonObject para);
    void net_modOrderPrice(const QJsonObject para);
    void net_getglobalOrders();
    /////////////////////////////////////////////////////
    void net_newCustomer(const QJsonObject para);
    void net_modCustomer(const QJsonObject para);
    void net_delCustomer(const QJsonObject para);
    void net_getGlobalCustomers();
    /////////////////////////////////////////////////////
    void net_newUnit(const QJsonObject para);
    void net_delUnit(const QJsonObject para);
    void net_getglobalUnits();
    /////////////////////////////////////////////////////
    void net_newSupplier(const QJsonObject para);
    void net_modSupplier(const QJsonObject para);
    void net_delSupplier(const QJsonObject para);
    void net_getglobalSuppliers();
    /////////////////////////////////////////////////////
    void net_getglobalMateriels();
    //////////////////////////////////////////////////////
    void net_newGoods(const QJsonObject para);
    void net_modGoods(const QJsonObject para);
    void net_delGoods(const QJsonObject para);
    void net_inOutGoods(const QJsonObject para);
    void net_getglobalGoods();
    void net_newGoodsType(const QJsonObject para);
    void net_delGoodsType(const QJsonObject para);
    void net_getGlobalGoodsType();
    /////////////////////////////////////////////////////
    void net_newPlating(const QJsonObject para);
    void net_delPlating(const QJsonObject para);
    void net_getglobalPlating();
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void pub_showMessage(QString msg,int sec=0);
    ////////////////////////////////////////////////////
    void pub_showLoadding(QString str="",int sec=4000,QColor c =Qt::black);
    void pub_hideLoadding();
    ////////////////////////////////////////////////////
    User pub_CurUser();
    ////////////////////////////////////////////////////
    QVector<Order> pub_StatusOrders(QString status);
    Order pub_getOrder(QString OrderID,bool &ok);
    ////////////////////////////////////////////////////
    QVector<QString> pub_Units();
    bool pub_checkUnitExist(QString unit);
    ////////////////////////////////////////////////////
    QVector<QString> pub_Platings();
    bool pub_checkPlatingExist(QString unit);
    ////////////////////////////////////////////////////
    QVector<Materiel>pub_Materiels();
    bool pub_checkMaterielID(QString id);
    Materiel pub_getMateriel(QString MID,bool &ok);
    ////////////////////////////////////////////////////
    QSet<QString> pub_Batchs();
    ////////////////////////////////////////////////////
    QVector<Customer>pub_Customers();
    bool pub_checkCustomerExist(QString name);
    Customer pub_getCustomer(QString CID,bool &ok);
    ////////////////////////////////////////////////////
    QVector<QString> pub_getDepartments() const;
    bool pub_checkDepartment(QString depart);
    ////////////////////////////////////////////////////
    QVector<QString> pub_getAuthors() const;
    ////////////////////////////////////////////////////
    QVector<User> pub_employees()const;
    User pub_getUser(QString UID,bool &ok);
    ///////////////////////////////////////////////////
    QVector<Supplier>pub_Suppliers();
    Supplier pub_getSupplier(QString CID,bool &ok);
    bool pub_checkSuppliser(QString name);
    ////////////////////////////////////////////////////
    QVector<Goods> pub_goods() const;
    Goods pub_getGoods(QString id,bool &ok);
    QVector<QString> pub_goodsType() const;
    QVector<Goods> pub_GetTypeGoods(QString type);
    bool pub_checkTypeExist(QString type);
    ////////////////////////////////////////////////////
    SysSetting CurSettings();
    void setCurSettings(SysSetting set);
    //////////////////////////////////////////////////

signals:
    void sig_showStatusMessage(QString msg,int sec);
    ///////////////////////////////////////////
    void sig_login(bool);
    ///////////////////////////////////////////
    void sig_newOrder(Order,bool);
    void sig_modOrder(Order,bool);
    void sig_cancleOrder(Order,bool);
    void sig_produceOrder(Order,bool);
    void sig_finishOrder(Order,bool);
    void sig_modOrderPrice(Order,bool);
    void sig_delOrder(Order,bool);
    void sig_globalOrders(bool);
    //////////////////////////////////////
    void sig_newEmployee(User,bool);
    void sig_modEmployee(User,bool);
    void sig_outEmployee(User,bool);
    void sig_delEmployee(QString,bool);
    void sig_globalEmployees(bool);
    ////////////////////////////////////
    void sig_newDepartment(QString,bool);
    void sig_delDepartment(QString,bool);
    void sig_globalDepartment(bool);
    ////////////////////////////////////
    void sig_newCustomer(Customer,bool);
    void sig_modCustomer(Customer,bool);
    void sig_delCustomer(QString,bool);
    void sig_globalcustomers(bool);
    ////////////////////////////////////
    void sig_newUnit(QString,bool);
    void sig_delUnit(QString,bool);
    void sig_globalUnits(bool);
    ////////////////////////////////////
    void sig_newSupplier(Supplier,bool);
    void sig_modSUpplier(Supplier,bool);
    void sig_delSUpplier(QString,bool);
    void sig_globalSUppliers(bool);
    ///////////////////////////////////
    void sig_globalMateriels(bool);
    ////////////////////////////////////
    void sig_newPlating(QString,bool);
    void sig_delPlating(QString,bool);
    void sig_globalPlating(bool);
    //////////////////////////////////
    void sig_newGoods(Goods,bool);
    void sig_modGoods(Goods,bool);
    void sig_delGoods(QString,bool);
    void sig_inoutGoods(Goods,bool);
    void sig_globalGoods(bool);
    void sig_newGoodsType(QString,bool);
    void sig_delGoodsType(QString,bool);
    void sig_globalGoodsType(bool);
private:
    void pri_initBath();
    void pri_checkGoodType(QString type);
private:
    User               cur_user;     //当前登录的账号
    QVector<User>      m_employee;   //所有的员工
    QVector<Order>     m_orders;     //所有订单
    QVector<QString>   m_units;      //所有计量单位
    QVector<QString>   m_Platings;      //所有镀种
    QSet<QString>      m_batch;      //所有用户批次
    QVector<Materiel>  m_maters;     //所有物料
    QVector<Customer>  m_customers;  //所有客户
    QVector<Supplier>  m_suppliers;  //所有供应商
    QVector<QString>   m_authors;    //所有的权限
    QVector<QString>   m_departments;//所有的部门
    QVector<Goods>     m_goods;      //所有的商品
    QVector<QString>   m_goodsType;  //所有商品的分类
    Loadding           m_load;       //加载动画
    nSysConfig         m_Config;     //保存系统配置
};

#endif // DATACENTER_H
