#include "datacenter.h"
#include  "orderservice.h"
#include <QDebug>
#include <QDateTime>
#include "userservice.h"
#include "customerservice.h"
#include "unitservice.h"
#include "supplierservice.h"
#include "accountservice.h"
#include "materialservice.h"
#include "goodsService.h"
#include "boost/thread.hpp"


dataCenter::dataCenter(QObject *parent) : QObject(parent)
{
    m_authors.push_back("操作员");
    m_authors.push_back("仓库");
    m_authors.push_back("财务");
    m_authors.push_back("管理员");
    m_authors.push_back("超级管理员");

    m_departments.push_back("生产部");
    m_departments.push_back("仓库部");
    m_departments.push_back("行政部");
    m_departments.push_back("财务部");
    m_departments.push_back("销售部");
}

void dataCenter::initData()
{
    //////////////初始化所有订单///////////////////
    boost::thread (boost::bind(&dataCenter::net_getglobalOrders,dataCenter::instance())).detach();

    //////////////初始化所有单位///////////////////
    boost::thread (boost::bind(&dataCenter::net_getglobalUnits,dataCenter::instance())).detach();

    //////////////初始化所有员工///////////////////
    boost::thread (boost::bind(&dataCenter::net_getGlobalUsers,dataCenter::instance())).detach();

    //////////////初始化所有客户///////////////////
    boost::thread (boost::bind(&dataCenter::net_getGlobalCustomers,dataCenter::instance())).detach();

    //////////////初始化所有供应商///////////////////
    boost::thread (boost::bind(&dataCenter::net_getglobalSuppliers,dataCenter::instance())).detach();

    //////////////初始化所有物料//////////////////
    boost::thread (boost::bind(&dataCenter::net_getglobalMateriels,dataCenter::instance())).detach();

    //////////////初始化所有商品//////////////////
    boost::thread(boost::bind(&dataCenter::net_getglobalGoods,dataCenter::instance())).detach();

}

void dataCenter::net_login(const QJsonObject para)
{
    bool ok = false;
    User u= accountService::login(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(ok){
        cur_user = u;
    }
    emit sig_login(ok);
}

void dataCenter::net_newUser(const QJsonObject para)
{
    bool isOK   = false;
    User user = UserService::newUser(para,isOK,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(isOK){
        m_employee.append(user);
    }
    emit sig_newEmployee(user,isOK);
}

void dataCenter::net_modUser(const QJsonObject para)
{
    bool isOK   = false;
    User user = UserService::modUser(para,isOK,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(isOK){
        for(int i=0;i<m_employee.size();++i){
            if(m_employee[i].UID==user.UID){
                m_employee[i] = user;
                break;
            }
        }
    }
    emit sig_modEmployee(user,isOK);
}

void dataCenter::net_outUser(const QJsonObject para)
{
    bool isOK   = false;
    User user = UserService::outUser(para,isOK,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(isOK){
        for(int i=0;i<m_employee.size();++i){
            if(m_employee[i].UID==user.UID){
                m_employee[i] = user;
                break;
            }
        }
    }
    emit sig_outEmployee(user,isOK);
}

void dataCenter::net_delUser(const QJsonObject para)
{
    bool isOK   = false;
    QString user = UserService::delUser(para,isOK,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(isOK){
        for(int i=0;i<m_employee.size();++i){
            if(m_employee[i].UID==user){
                m_employee.remove(i);
                break;
            }
        }
    }
    emit sig_delEmployee(user,isOK);
}

void dataCenter::net_getGlobalUsers()
{
    bool ok =false;
    m_employee = UserService::getAllUsers(ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    emit sig_globalEmployees(ok);
}

void dataCenter::net_newOrder(const QJsonObject para)
{
    bool isOK   = false;
    Order order = OrderService::newOrder(para,isOK,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(isOK){
        m_orders.append(order);
        m_batch.insert(order.CustomBatch);
    }
    emit sig_newOrder(order,isOK);
}

void dataCenter::net_modOrder(const QJsonObject para)
{
    bool isOK   = false;
    Order order = OrderService::modOrder(para,isOK,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(isOK){
        for(int i=0;i<m_orders.size();++i){
            if(m_orders[i].OrderID==order.OrderID){
                m_orders[i] = order;
                break;
            }
        }
    }
    emit sig_modOrder(order,isOK);
}

void dataCenter::net_cancleOrder(const QJsonObject para)
{
    bool isOK   = false;
    Order order = OrderService::cancleOrder(para,isOK,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(isOK){
        for(int i=0;i<m_orders.size();++i){
            if(m_orders[i].OrderID==order.OrderID){
                m_orders[i] = order;
                break;
            }
        }
    }
    emit sig_cancleOrder(order,isOK);
}

void dataCenter::net_delOrder(const QJsonObject para)
{
    bool isOK   = false;
    QString orderID = OrderService::delOrder(para,isOK,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(isOK){
        for(int i=0;i<m_orders.size();++i){
            if(m_orders[i].OrderID==orderID){
                m_orders.remove(i);
                break;
            }
        }
    }
    emit sig_delOrder(orderID,isOK);
}

void dataCenter::net_produceOrder(const QJsonObject para)
{
    bool isOK   = false;
    Order order = OrderService::produceOrder(para,isOK,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(isOK){
        for(int i=0;i<m_orders.size();++i){
            if(m_orders[i].OrderID==order.OrderID){
                m_orders[i] = order;
                break;
            }
        }
    }
    emit sig_produceOrder(order,isOK);
}

void dataCenter::net_finishOrder(const QJsonObject para)
{
    bool isOK   = false;
    Order order = OrderService::finishOrder(para,isOK,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(isOK){
        for(int i=0;i<m_orders.size();++i){
            if(m_orders[i].OrderID==order.OrderID){
                m_orders[i] = order;
                break;
            }
        }
    }
    emit sig_finishOrder(order,isOK);
}

void dataCenter::net_modOrderPrice(const QJsonObject para)
{
    bool isOK   = false;
    Order order = OrderService::modOrderPrice(para,isOK,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(isOK){
        for(int i=0;i<m_orders.size();++i){
            if(m_orders[i].OrderID==order.OrderID){
                m_orders[i] = order;
                break;
            }
        }
    }
    emit sig_modOrderPrice(order,isOK);
}

void dataCenter::net_getglobalOrders()
{
    bool ok =false;
    m_orders = OrderService::getAllOrders(ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(ok){
        pri_initBath();
    }
    emit sig_globalOrders(ok);
}

void dataCenter::net_newCustomer(const QJsonObject para)
{
    bool isOK    = false;
    Customer cus = CustomerService::newCustomer(para,isOK,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(isOK){
        m_customers.append(cus);
    }
    emit sig_newCustomer(cus,isOK);
}

void dataCenter::net_modCustomer(const QJsonObject para)
{
    bool isOK    = false;
    Customer cus = CustomerService::modCustomer(para,isOK,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(isOK){
        for(int i=0;i<m_customers.size();++i){
            if(m_customers[i].CID==cus.CID){
                m_customers[i] = cus;
                break;
            }
        }
    }
    emit sig_modCustomer(cus,isOK);
}

void dataCenter::net_delCustomer(const QJsonObject para)
{
    bool isOK    = false;
    QString cus =CustomerService::delCustomer(para,isOK,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(isOK){
        for(int i=0;i<m_customers.size();++i){
            if(m_customers[i].CID==cus){
                m_customers.remove(i);
                break;
            }
        }
    }
    emit sig_delCustomer(cus,isOK);
}

void dataCenter::net_getGlobalCustomers()
{
    bool ok = false;
    m_customers = CustomerService::getAllCustomer(ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    emit sig_globalcustomers(ok);
}

void dataCenter::net_newUnit(const QJsonObject para)
{
    bool ok = false;
    QString unit =  UnitService::newUnit(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(ok){
        m_units.append(unit);
    }
    emit sig_newUnit(unit,ok);
}

void dataCenter::net_delUnit(const QJsonObject para)
{
    bool ok = false;
    QString unit = UnitService::delUnit(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(ok){
        m_units.removeOne(unit);
    }
    emit sig_delUnit(unit,ok);
}

void dataCenter::net_getglobalUnits()
{
    bool ok =false;
    m_units = UnitService::getAllUnits(ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    emit sig_globalUnits(ok);
}

void dataCenter::net_newSupplier(const QJsonObject para)
{
    bool ok =false;
    Supplier cus = SupplierService::newSupplier(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(ok){
        m_suppliers.push_back(cus);
    }
    emit sig_newSupplier(cus,ok);
}

void dataCenter::net_modSupplier(const QJsonObject para)
{
    bool ok = false;
    Supplier cus =SupplierService::modSupplier(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(ok){
        for(int i=0;i<m_suppliers.size();++i){
            if(m_suppliers[i].SID==cus.SID){
                m_suppliers[i] = cus;
                break;
            }
        }
    }
    emit sig_modSUpplier(cus,ok);
}

void dataCenter::net_delSupplier(const QJsonObject para)
{
    bool ok = false;
    QString cus = SupplierService::delSupplier(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(ok){
        for(int i=0;i<m_suppliers.size();++i){
            if(m_suppliers[i].SID==cus){
                m_suppliers.remove(i);
                break;
            }
        }
    }
    emit sig_delSUpplier(cus,ok);
}

void dataCenter::net_getglobalSuppliers()
{
    bool ok = false;
    m_suppliers = SupplierService::getAllSupplierls(ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    emit sig_globalSUppliers(ok);
}

void dataCenter::net_getglobalMateriels()
{
    bool ok =false;
    m_maters = MaterialService::getAllMateriels(ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    emit sig_globalMateriels(ok);

}

void dataCenter::net_newGoods(const QJsonObject para)
{
    bool ok = false;
    Goods goods = GoodsService::newGoods(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if (ok){
        m_goods.push_back(goods);
    }
    emit sig_modGoods(goods,ok);
}

void dataCenter::net_modGoods(const QJsonObject para)
{
    bool ok = false;
    Goods goods = GoodsService::modGoods(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(ok){
        for(int i=0;i<m_goods.size();++i){
            if(m_goods[i].ID==goods.ID){
                m_goods[i] = goods;
                break;
            }
        }
    }
    emit sig_modGoods(goods,ok);
}

void dataCenter::net_delGoods(const QJsonObject para)
{
    bool ok = false;
    QString id = GoodsService::delGoods(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(ok){
        for(int i=0;i<m_goods.size();++i){
            if(m_goods[i].ID==id){
                m_goods.remove(i);
                break;
            }
        }
    }
    emit sig_delGoods(id,ok);
}

void dataCenter::net_getglobalGoods()
{
    bool ok = false;
    m_goods = GoodsService::getAllGoods(ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    emit sig_globalGoods(ok);
}


void dataCenter::pub_showMessage(QString msg, int sec)
{
    emit sig_showStatusMessage(msg,sec);
}


void dataCenter::pub_showLoadding(QString str,int sec,QColor c )
{
    m_load.showTime(str,sec,c);
}

void dataCenter::pub_hideLoadding()
{
    m_load.showOver();
}


User dataCenter::pub_CurUser()
{
    return cur_user;
}

QVector<Order> dataCenter::pub_StatusOrders(QString status)
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
Order dataCenter::pub_getOrder(QString OrderID, bool &ok)
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


QVector<QString> dataCenter::pub_Units()
{
    return m_units;
}

bool dataCenter::pub_checkUnitExist(QString unit)
{
    for(QString m:m_units){
        if (m==unit){
            return true;
        }
    }
    return false;
}



QVector<Materiel> dataCenter::pub_Materiels()
{
    return m_maters;
}

bool dataCenter::pub_checkMaterielID(QString id)
{
    for(Materiel m:m_maters){
        if (m.MaterID==id){
            return true;
        }
    }
    return false;
}

Materiel dataCenter::pub_getMateriel(QString MID, bool &ok)
{
    ok = true;
    for(Materiel m:m_maters){
        if (m.MaterID==MID){
            ok = true;
            return m;
        }
    }
    ok = false;
    Materiel s;
    return s;
}


QSet<QString> dataCenter::pub_Batchs()
{
    return m_batch;
}


QVector<QString> dataCenter::pub_getAuthors() const
{
    return m_authors;
}

QVector<User> dataCenter::pub_employees() const
{
    return m_employee;
}

User dataCenter::pub_getUser(QString UID, bool &ok)
{
    User a;
    for(User o: m_employee) {
        if (o.UID.compare(UID)==0){
            ok = true;
            return o;
        }
    }
    ok = false;
    return a;
}

QVector<Supplier> dataCenter::pub_Suppliers()
{
    return m_suppliers;
}

Supplier dataCenter::pub_getSupplier(QString CID, bool &ok)
{
    Supplier a;
    for(Supplier o: m_suppliers) {
        if (o.SID == CID){
            ok = true;
            return o;
        }
    }
    ok = false;
    return a;
}

SysSetting dataCenter::CurSettings()
{
    return m_Config.Setting();
}

void dataCenter::setCurSettings(SysSetting set)
{
    m_Config.setSetting(set);
}

void dataCenter::pri_initBath()
{
    m_batch.clear();
    for(Order order:m_orders){
        m_batch.insert(order.CustomBatch);
    }
}

QVector<Goods> dataCenter::goods() const
{
    return m_goods;
}

Goods dataCenter::pub_getGoods(QString id, bool &ok)
{
    Goods s;
    for(Goods g:m_goods){
        if(g.ID == id){
            ok = true;
            return g;
        }
    }
    return  s;
}


QVector<QString> dataCenter::pub_getDepartments() const
{
    return m_departments;
}

QVector<Customer> dataCenter::pub_Customers()
{
    return m_customers;
}

bool dataCenter::pub_checkCustomerExist(QString name)
{
    for(Customer m:m_customers){
        if (m.Name==name){
            return true;
        }
    }
    return false;
}

Customer dataCenter::pub_getCustomer(QString CID, bool &ok)
{
    Customer a;
    for(Customer o: m_customers) {
        if (o.CID == CID){
            ok = true;
            return o;
        }
    }
    ok = false;
    return a;
}
