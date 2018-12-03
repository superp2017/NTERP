#include "datacenter.h"
#include "orderservice.h"
#include <QDebug>
#include <QDateTime>
#include "userservice.h"
#include "customerservice.h"
#include "unitservice.h"
#include "supplierservice.h"
#include "accountservice.h"
#include "materialservice.h"
#include "goodsService.h"
#include "platingservice.h"
#include "boost/thread.hpp"
#include "goodsoutrecordservice.h"


dataCenter::dataCenter(QObject *parent) : QObject(parent)//,m_notice(parent)
{
    m_authors.push_back("操作员");
    m_authors.push_back("仓库");
    m_authors.push_back("财务");
    m_authors.push_back("管理员");
    m_authors.push_back("超级管理员");
    m_first_timer=NULL;
    m_second_timer=NULL;
    m_third_timer=NULL;
    m_heartbeat_timer =NULL;
    connect(&m_notice,SIGNAL(newNOtice(QJsonObject&)),this,SLOT(newNotice(QJsonObject&)));
}

void dataCenter::ListenNotice()
{
    //开始监听通知
    m_notice.Listen(m_Config.noticePort());
    m_heartbeat_timer = new QTimer(this);
    connect(m_heartbeat_timer, SIGNAL(timeout()), this, SLOT(heartbeat()));
    m_heartbeat_timer->start(1000*3*1);
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

    //////////////初始化所有部门///////////////////
    boost::thread (boost::bind(&dataCenter::net_getGlobalDepartment,dataCenter::instance())).detach();

    //////////////初始化所有供应商///////////////////
    boost::thread (boost::bind(&dataCenter::net_getglobalSuppliers,dataCenter::instance())).detach();

    //////////////初始化所有物料//////////////////
    boost::thread (boost::bind(&dataCenter::net_getglobalMateriels,dataCenter::instance())).detach();

    //////////////初始化所有商品//////////////////
    boost::thread(boost::bind(&dataCenter::net_getglobalGoods,dataCenter::instance())).detach();

    //////////////初始化所有商品的分类//////////////////
    boost::thread(boost::bind(&dataCenter::net_getGlobalGoodsType,dataCenter::instance())).detach();

    //////////////初始化所有镀种的分类//////////////////
    boost::thread(boost::bind(&dataCenter::net_getglobalPlating,dataCenter::instance())).detach();

    //////////////获取所有商品的出库记录//////////////////////////////
    boost::thread(boost::bind(&dataCenter::net_getAllOutRecords,dataCenter::instance())).detach();

    //////////////获取打印数量/////////////////////////////
    boost::thread(boost::bind(&dataCenter::net_getPrintNumber,dataCenter::instance())).detach();


}

void dataCenter::clearData()
{
    qDebug()<<"dataCenter clear";
    TimerUpdate(true);

    if(m_heartbeat_timer!=NULL&&m_heartbeat_timer->isActive()) m_heartbeat_timer->stop();
    m_employee.clear();   //所有的员工
    m_orders.clear();     //所有订单
    m_units.clear();      //所有计量单位
    m_Platings.clear();   //所有镀种
    m_batch.clear();      //所有用户批次
    m_maters.clear();     //所有物料
    m_hashMaterials.clear();//不同客户的物料ID
    m_customers.clear();  //所有客户
    m_suppliers.clear();  //所有供应商
    m_authors.clear();    //所有的权限
    m_departments.clear();//所有的部门
    m_goods.clear();      //所有的商品
    m_goodsType.clear();  //所有商品的分类
    m_goodsRecords.clear();//所有商品出库记录
    m_load.showOver();       //加载动画
    m_Config.clear();     //保存系统配置
    m_first_timer =NULL;   //订单定时器
    m_second_timer=NULL;    //第二定时器
    m_third_timer=NULL; //第三定时器
    m_heartbeat_timer=NULL;
}


void dataCenter::net_login(const QJsonObject para)
{
    bool ok = false;
    User u= accountService::login(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(ok){
        cur_user = u;
        m_Config.setAccount(cur_user.Account,cur_user.Code);
        m_Config.reSave();
    }
    emit sig_login(ok);
}


void dataCenter::pri_opt_User(bool ok, User &user, enum_NoticeType noticeType)
{
    switch (noticeType) {
    case NoticeType_NEW:
        if(ok){
            m_employee.append(user);
        }
        emit sig_newEmployee(user,ok);
        break;
    case NoticeType_Modify:
        if(ok){
            bool exist =false;
            for(int i=0;i<m_employee.size();++i){
                if(m_employee[i].UID==user.UID){
                    m_employee[i] = user;
                    exist = true;
                    break;
                }
            }
            if(!exist) m_employee.push_back(user);
        }
        emit sig_modEmployee(user,ok);
        break;
    case NoticeType_Del:
        if(ok){
            for(int i=0;i<m_employee.size();++i){
                if(m_employee[i].UID==user.UID){
                    m_employee.remove(i);
                    break;
                }
            }
        }
        emit sig_delEmployee(user.UID,ok);
        break;
    default:
        break;
    }
}




void dataCenter::net_newUser(const QJsonObject para)
{
    bool isOK   = false;
    User user = UserService::newUser(para,isOK,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    //    if(isOK){
    //        m_employee.append(user);
    //    }
    //    emit sig_newEmployee(user,isOK);
    pri_opt_User(isOK,user,NoticeType_NEW);
}

void dataCenter::net_modUser(const QJsonObject para)
{
    bool isOK   = false;
    User user = UserService::modUser(para,isOK,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    //    if(isOK){
    //        for(int i=0;i<m_employee.size();++i){
    //            if(m_employee[i].UID==user.UID){
    //                m_employee[i] = user;
    //                break;
    //            }
    //        }
    //    }
    //    emit sig_modEmployee(user,isOK);
    pri_opt_User(isOK,user,NoticeType_Modify);
}

void dataCenter::net_outUser(const QJsonObject para)
{
    bool isOK   = false;
    User user = UserService::outUser(para,isOK,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    //    if(isOK){
    //        for(int i=0;i<m_employee.size();++i){
    //            if(m_employee[i].UID==user.UID){
    //                m_employee[i] = user;
    //                break;
    //            }
    //        }
    //    }
    //    emit sig_outEmployee(user,isOK);
    pri_opt_User(isOK,user,NoticeType_Modify);
}

void dataCenter::net_delUser(const QJsonObject para)
{
    bool isOK   = false;
    User user = UserService::delUser(para,isOK,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    //    if(isOK){
    //        for(int i=0;i<m_employee.size();++i){
    //            if(m_employee[i].UID==user){
    //                m_employee.remove(i);
    //                break;
    //            }
    //        }
    //    }
    //    emit sig_delEmployee(user,isOK);
    pri_opt_User(isOK,user,NoticeType_Del);
}

void dataCenter::net_getGlobalUsers()
{
    bool ok =false;
    m_employee = UserService::getAllUsers(ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    emit sig_globalEmployees(ok);
}

void dataCenter::pri_opt_DepartMent(bool ok, QString &type, enum_NoticeType noticeType)
{    switch (noticeType) {
    case NoticeType_NEW:
        break;
    case NoticeType_Del:
    default:
        break;
    }

}

void dataCenter::net_newDepartment(const QJsonObject para)
{
    bool ok = false;
    QString de = UserService::newDepartment(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    //    m_departments.push_back(de);
    //    emit sig_newDepartment(de,ok);

    pri_opt_DepartMent(ok,de,NoticeType_NEW);

}

void dataCenter::net_delDepartment(const QJsonObject para)
{
    bool ok = false;
    QString de = UserService::delDepartment(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    //    m_departments.removeOne(de);
    //    emit sig_delDepartment(de,ok);

    pri_opt_DepartMent(ok,de,NoticeType_Del);
}

void dataCenter::net_getGlobalDepartment()
{
    bool ok = false;
    m_departments = UserService::getAllDepartment(QJsonObject(),ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    emit sig_globalDepartment(ok);
}

void dataCenter::pri_opt_Order(bool ok, Order &order, enum_NoticeType noticeType)
{
    switch (noticeType) {
    case NoticeType_NEW:
        if(ok){
            m_orders.append(order);
            m_batch.insert(order.CustomBatch);
        }
        emit sig_newOrder(order,ok);
        break;
    case NoticeType_Modify:
        if(ok){
            bool exist = false;
            for(int i=0;i<m_orders.size();++i){
                if(m_orders[i].OrderID==order.OrderID){
                    m_orders[i] = order;
                    exist = true;
                    break;
                }
            }
            if(!exist) m_orders.push_back(order);
        }
        emit sig_modOrder(order,ok);
        break;
    case NoticeType_Del:
        if(ok){
            for(int i=0;i<m_orders.size();++i){
                if(m_orders[i].OrderID==order.OrderID){
                    m_orders.remove(i);
                    break;
                }
            }
        }
        emit sig_delOrder(order,ok);
        break;
    default:
        break;
    }
}



void dataCenter::net_newOrder(const QJsonObject para)
{
    bool isOK   = false;
    Order order = OrderService::newOrder(para,isOK,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    //    if(isOK){
    //        m_orders.append(order);
    //        m_batch.insert(order.CustomBatch);
    //    }
    //    emit sig_newOrder(order,isOK);
    pri_opt_Order(isOK,order,NoticeType_NEW);
}

void dataCenter::net_modOrder(const QJsonObject para)
{
    bool isOK   = false;
    Order order = OrderService::modOrder(para,isOK,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    //    if(isOK){
    //        for(int i=0;i<m_orders.size();++i){
    //            if(m_orders[i].OrderID==order.OrderID){
    //                m_orders[i] = order;
    //                break;
    //            }
    //        }
    //    }
    //    emit sig_modOrder(order,isOK);
    pri_opt_Order(isOK,order,NoticeType_Modify);
}

void dataCenter::net_cancleOrder(const QJsonObject para)
{
    bool isOK   = false;
    Order order = OrderService::cancleOrder(para,isOK,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    //    if(isOK){
    //        for(int i=0;i<m_orders.size();++i){
    //            if(m_orders[i].OrderID==order.OrderID){
    //                m_orders[i] = order;
    //                break;
    //            }
    //        }
    //    }
    //    emit sig_cancleOrder(order,isOK);
    pri_opt_Order(isOK,order,NoticeType_Modify);
}

void dataCenter::net_delOrder(const QJsonObject para)
{
    bool isOK   = false;
    Order order = OrderService::delOrder(para,isOK,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    //    if(isOK){
    //        for(int i=0;i<m_orders.size();++i){
    //            if(m_orders[i].OrderID==order.OrderID){
    //                m_orders.remove(i);
    //                break;
    //            }
    //        }
    //    }
    //    emit sig_delOrder(order,isOK);
    pri_opt_Order(isOK,order,NoticeType_Del);
}

void dataCenter::net_produceOrder(const QJsonObject para)
{
    bool isOK   = false;
    Order order = OrderService::produceOrder(para,isOK,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    //    if(isOK){
    //        for(int i=0;i<m_orders.size();++i){
    //            if(m_orders[i].OrderID==order.OrderID){
    //                m_orders[i] = order;
    //                break;
    //            }
    //        }
    //    }
    //    emit sig_produceOrder(order,isOK);
    pri_opt_Order(isOK,order,NoticeType_Modify);
}

void dataCenter::net_finishOrder(const QJsonObject para)
{
    bool isOK   = false;
    Order order = OrderService::finishOrder(para,isOK,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    //    if(isOK){
    //        for(int i=0;i<m_orders.size();++i){
    //            if(m_orders[i].OrderID==order.OrderID){
    //                m_orders[i] = order;
    //                break;
    //            }
    //        }
    //    }
    //    emit sig_finishOrder(order,isOK);

    pri_opt_Order(isOK,order,NoticeType_Modify);
}

void dataCenter::net_modOrderPrice(const QJsonObject para)
{
    bool isOK   = false;
    Order order = OrderService::modOrderPrice(para,isOK,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    //    if(isOK){
    //        for(int i=0;i<m_orders.size();++i){
    //            if(m_orders[i].OrderID==order.OrderID){
    //                m_orders[i] = order;
    //                break;
    //            }
    //        }
    //    }
    //    emit sig_modOrderPrice(order,isOK);
    pri_opt_Order(isOK,order,NoticeType_Modify);
}

void dataCenter::net_updatePrintNum(const QJsonObject para)
{
    bool isOK   = false;
    QVector<Order> list = OrderService::updatePrintNum(para,isOK,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    //    if(isOK){
    //        for (Order o:list){
    //            for(int i=0;i<m_orders.size();++i){
    //                if(m_orders[i].OrderID==o.OrderID){
    //                    m_orders[i] = o;
    //                    break;
    //                }
    //            }
    //        }
    //        //////////////获取打印数量/////////////////////////////
    //        boost::thread(boost::bind(&dataCenter::net_getPrintNumber,dataCenter::instance())).detach();
    //    }
    //    emit sig_updatePrintNum(list,isOK);
    for(Order o:list){
        pri_opt_Order(isOK,o,NoticeType_Modify);
    }
}



void dataCenter::net_getPrintNumber()
{
    QJsonObject para;
    bool isOK = false;
    QString ret =OrderService::setPrintNumber(para,isOK,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(isOK&&ret!=-1){
        m_print_number = ret;
    }else{
        m_print_number ="20189999";
    }
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

void dataCenter::pri_opt_Customer(bool ok, Customer &cus, enum_NoticeType noticeType)
{
    switch (noticeType) {
    case NoticeType_NEW:
        if(ok){
            m_customers.append(cus);
        }
        emit sig_newCustomer(cus,ok);
        break;
    case NoticeType_Modify:
        if(ok){
            bool exist = false;
            for(int i=0;i<m_customers.size();++i){
                if(m_customers[i].CID==cus.CID){
                    m_customers[i] = cus;
                    exist = true;
                    break;
                }
            }
            if(!exist){
                m_customers.push_back(cus);
            }
        }
        emit sig_modCustomer(cus,ok);
        break;
    case NoticeType_Del:
        if(ok){
            for(int i=0;i<m_customers.size();++i){
                if(m_customers[i].CID==cus.CID){
                    m_customers.remove(i);
                    break;
                }
            }
        }
        emit sig_delCustomer(cus.CID,ok);
        break;
    default:
        break;
    }
}




void dataCenter::net_newCustomer(const QJsonObject para)
{
    bool isOK    = false;
    Customer cus = CustomerService::newCustomer(para,isOK,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    //    if(isOK){
    //        m_customers.append(cus);
    //    }
    //    emit sig_newCustomer(cus,isOK);
    pri_opt_Customer(isOK,cus,NoticeType_NEW);
}

void dataCenter::net_modCustomer(const QJsonObject para)
{
    bool isOK    = false;
    Customer cus = CustomerService::modCustomer(para,isOK,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    //    if(isOK){
    //        for(int i=0;i<m_customers.size();++i){
    //            if(m_customers[i].CID==cus.CID){
    //                m_customers[i] = cus;
    //                break;
    //            }
    //        }
    //    }
    //    emit sig_modCustomer(cus,isOK);
    pri_opt_Customer(isOK,cus,NoticeType_Modify);
}

void dataCenter::net_delCustomer(const QJsonObject para)
{
    bool isOK    = false;
    Customer  cus =CustomerService::delCustomer(para,isOK,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    //    if(isOK){
    //        for(int i=0;i<m_customers.size();++i){
    //            if(m_customers[i].CID==cus){
    //                m_customers.remove(i);
    //                break;
    //            }
    //        }
    //    }
    //    emit sig_delCustomer(cus,isOK);
    pri_opt_Customer(isOK,cus,NoticeType_Del);
}

void dataCenter::net_getGlobalCustomers()
{
    bool ok = false;
    m_customers = CustomerService::getAllCustomer(ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    emit sig_globalcustomers(ok);
}

void dataCenter::pri_opt_Unit(bool ok, QString &unit, enum_NoticeType noticeType)
{
    switch (noticeType) {
    case NoticeType_NEW:
        if(ok){
            m_units.append(unit);
        }
        emit sig_newUnit(unit,ok);
        break;
    case NoticeType_Modify:

        break;
    case NoticeType_Del:
        if(ok){
            m_units.removeOne(unit);
        }
        emit sig_delUnit(unit,ok);
        break;
    default:
        break;
    }
}


void dataCenter::net_newUnit(const QJsonObject para)
{
    bool ok = false;
    QString unit =  UnitService::newUnit(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    //    if(ok){
    //        m_units.append(unit);
    //    }
    //    emit sig_newUnit(unit,ok);
    pri_opt_Unit(ok,unit,NoticeType_NEW);
}

void dataCenter::net_delUnit(const QJsonObject para)
{
    bool ok = false;
    QString unit = UnitService::delUnit(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    //    if(ok){
    //        m_units.removeOne(unit);
    //    }
    //    emit sig_delUnit(unit,ok);
    pri_opt_Unit(ok,unit,NoticeType_Del);
}

void dataCenter::net_getglobalUnits()
{
    bool ok =false;
    m_units = UnitService::getAllUnits(ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    emit sig_globalUnits(ok);
}

void dataCenter::pri_opt_supplier(bool ok, Supplier &sup, enum_NoticeType noticeType)
{
    switch (noticeType) {
    case NoticeType_NEW:
        if(ok){
            m_suppliers.push_back(sup);
        }
        emit sig_newSupplier(sup,ok);
        break;
    case NoticeType_Modify:
        if(ok){
            bool exist = false;
            for(int i=0;i<m_suppliers.size();++i){
                if(m_suppliers[i].SID==sup.SID){
                    m_suppliers[i] = sup;
                    exist = true;
                    break;
                }
            }
            if(!exist){
                m_suppliers.push_back(sup);
            }
        }
        emit sig_modSUpplier(sup,ok);
        break;
    case NoticeType_Del:
        if(ok){
            for(int i=0;i<m_suppliers.size();++i){
                if(m_suppliers[i].SID==sup.SID){
                    m_suppliers.remove(i);
                    break;
                }
            }
        }
        emit sig_delSUpplier(sup.SID,ok);
        break;
    default:
        break;
    }
}

void dataCenter::net_newSupplier(const QJsonObject para)
{
    bool ok =false;
    Supplier sup = SupplierService::newSupplier(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    //    if(ok){
    //        m_suppliers.push_back(sup);
    //    }
    //    emit sig_newSupplier(sup,ok);
    pri_opt_supplier(ok,sup,NoticeType_NEW);
}

void dataCenter::net_modSupplier(const QJsonObject para)
{
    bool ok = false;
    Supplier sup =SupplierService::modSupplier(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    //    if(ok){
    //        for(int i=0;i<m_suppliers.size();++i){
    //            if(m_suppliers[i].SID==cus.SID){
    //                m_suppliers[i] = sup;
    //                break;
    //            }
    //        }
    //    }
    //    emit sig_modSUpplier(sup,ok);
    pri_opt_supplier(ok,sup,NoticeType_Modify);
}

void dataCenter::net_delSupplier(const QJsonObject para)
{
    bool ok = false;
    Supplier sup = SupplierService::delSupplier(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    //    if(ok){
    //        for(int i=0;i<m_suppliers.size();++i){
    //            if(m_suppliers[i].SID==cus){
    //                m_suppliers.remove(i);
    //                break;
    //            }
    //        }
    //    }
    //    emit sig_delSUpplier(cus,ok);
    pri_opt_supplier(ok,sup,NoticeType_Del);
}

void dataCenter::net_getglobalSuppliers()
{
    bool ok = false;
    m_suppliers = SupplierService::getAllSupplierls(ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    emit sig_globalSUppliers(ok);
}

void dataCenter::pri_opt_Material(bool ok, Materiel &mater, enum_NoticeType noticeType)
{
    switch (noticeType) {
    case NoticeType_NEW:
        if(ok){
            m_maters.push_back(mater);
            pri_addCustomerMaterial(mater.CID,mater.MaterID);
        }
        emit sig_newMaterial(mater,ok);
        break;
    case NoticeType_Modify:
        if(ok){
            bool exist=false;
            for(int i=0;i<m_maters.size();++i){
                if(m_maters[i].MaterID==mater.MaterID){
                    m_maters[i] = mater;
                    exist = true;
                    break;
                }
            }
            if(!exist){
                m_maters.push_back(mater);
            }
        }
        emit sig_modMaterial(mater,ok);
        break;
    case NoticeType_Del:
        if(ok){
            for(int i=0;i<m_maters.size();++i){
                if(m_maters[i].MaterID==mater.MaterID){
                    pri_removeCustomerMaterial(m_maters[i].CID,m_maters[i].MaterID);
                    m_maters.remove(i);
                    break;
                }
            }
        }
        emit sig_delMaterial(mater.MaterID,ok);
        break;
    default:
        break;
    }
}




void dataCenter::net_newMaterial(const QJsonObject para)
{
    bool ok = false;
    Materiel ma = MaterialService::newMaterial(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    //    if(ok){
    //        m_maters.push_back(ma);
    //        pri_addCustomerMaterial(ma.CID,ma.MaterID);
    //    }
    //    emit sig_newMaterial(ma,ok);
    pri_opt_Material(ok,ma,NoticeType_NEW);
}

void dataCenter::net_modMaterial(const QJsonObject para)
{
    bool ok = false;
    Materiel mater =MaterialService::modMaterial(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    //    if(ok){
    //        for(int i=0;i<m_maters.size();++i){
    //            if(m_maters[i].MaterID==mater.MaterID){
    //                m_maters[i] = mater;
    //                break;
    //            }
    //        }
    //    }
    //    emit sig_modMaterial(mater,ok);
    pri_opt_Material(ok,mater,NoticeType_Modify);
}

void dataCenter::net_delMaterial(const QJsonObject para)
{
    bool ok = false;
    Materiel mater = MaterialService::delMaterial(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    //    if(ok){
    //        for(int i=0;i<m_maters.size();++i){
    //            if(m_maters[i].MaterID==mater.MID){
    //                m_maters.remove(i);
    //               pri_removeCustomerMaterial(mater.CID,mater.MID);
    //                break;
    //            }
    //        }
    //    }
    //    emit sig_delMaterial(mater.MID,ok);
    pri_opt_Material(ok,mater,NoticeType_Modify);
}

void dataCenter::net_queryMaterial(const QJsonObject para)
{
    bool isOK   = false;
    Materiel mater = MaterialService::queryMaterial(para,isOK,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    emit sig_modMaterial(mater,isOK);
}

void dataCenter::net_getCustomerMaterial(const QJsonObject para)
{
    bool ok =false;
    QVector<QString>data = MaterialService::getCustomerMaterID(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(para.contains("CID")){
        m_hashMaterials[para.value("CID").toString()] = data;
    }
    emit sig_getCustomerMaterial(data,ok);
}

void dataCenter::net_getglobalMateriels()
{
    bool ok =false;
    m_maters = MaterialService::getAllMateriels(ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    emit sig_globalMateriels(ok);

}

void dataCenter::pri_opt_Goods(bool ok, Goods &goods, enum_NoticeType noticeType)
{
    switch (noticeType) {
    case NoticeType_NEW:
        if (ok){
            m_goods.push_back(goods);
            pri_checkGoodType(goods.Type);
        }
        emit sig_newGoods(goods,ok);
        break;
    case NoticeType_Modify:
        if(ok){
            bool ex =false;
            for(int i=0;i<m_goods.size();++i){
                if(m_goods[i].ID==goods.ID){
                    m_goods[i] = goods;
                    ex = true;
                    break;
                }
            }
            if(!ex) m_goods.push_back(goods);
            pri_checkGoodType(goods.Type);
        }
        emit sig_modGoods(goods,ok);
        break;
    case NoticeType_Del:
        if(ok){
            for(int i=0;i<m_goods.size();++i){
                if(m_goods[i].ID==goods.ID){
                    m_goods.remove(i);
                    break;
                }
            }
        }
        emit sig_delGoods(goods.ID,ok);
        break;
    default:
        break;
    }
}




void dataCenter::net_newGoods(const QJsonObject para)
{
    bool ok = false;
    Goods goods = GoodsService::newGoods(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    //    if (ok){
    //        m_goods.push_back(goods);
    //        pri_checkGoodType(goods.Type);
    //    }
    //    emit sig_newGoods(goods,ok);
    pri_opt_Goods(ok,goods,NoticeType_NEW);
}

void dataCenter::net_getGoods(const QJsonObject para)
{
    bool ok = false;
    Goods goods = GoodsService::getGoods(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if (ok){
        bool ex =false;
        for(int i=0;i<m_goods.size();++i){
            if(m_goods[i].ID==goods.ID){
                m_goods[i] = goods;
                ex = true;
                break;
            }
        }
        if(!ex) m_goods.push_back(goods);
    }
    emit sig_queryGoods(goods);
}

void dataCenter::net_modGoods(const QJsonObject para)
{
    bool ok = false;
    Goods goods = GoodsService::modGoods(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    //    if(ok){
    //        bool ex =false;
    //        for(int i=0;i<m_goods.size();++i){
    //            if(m_goods[i].ID==goods.ID){
    //                m_goods[i] = goods;
    //                break;
    //            }
    //        }
    //        if(!ex) m_goods.push_back(goods);
    //        pri_checkGoodType(goods.Type);
    //    }
    //    emit sig_modGoods(goods,ok);
    pri_opt_Goods(ok,goods,NoticeType_Modify);
}

void dataCenter::net_delGoods(const QJsonObject para)
{
    bool ok = false;
    Goods goods = GoodsService::delGoods(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    //    if(ok){
    //        for(int i=0;i<m_goods.size();++i){
    //            if(m_goods[i].ID==id){
    //                m_goods.remove(i);
    //                break;
    //            }
    //        }
    //    }
    //    emit sig_delGoods(id,ok);
    pri_opt_Goods(ok,goods,NoticeType_Del);
}

void dataCenter::net_addOutGoodsNum(const QJsonObject para)
{
    bool ok = false;
    Goods goods = GoodsService::addOutGoodsNum(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    //    if(ok){
    //        for(int i=0;i<m_goods.size();++i){
    //            if(m_goods[i].ID==goods.ID){
    //                m_goods[i] = goods;
    //                break;
    //            }
    //        }
    //    }
    //    emit sig_inGoods(goods,ok);
    pri_opt_Goods(ok,goods,NoticeType_Modify);
}

void dataCenter::net_getglobalGoods()
{
    bool ok = false;
    m_goods = GoodsService::getAllGoods(ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    emit sig_globalGoods(ok);
}



void dataCenter::pri_opt_GoodsType(bool ok, QString &type, enum_NoticeType noticeType)
{
    switch (noticeType) {
    case NoticeType_NEW:
        if(ok){
            m_goodsType.push_back(type);
        }
        emit sig_newGoodsType(type,ok);
        break;
    case NoticeType_Modify:

        break;
    case NoticeType_Del:
        m_goodsType.removeOne(type);
        emit sig_delGoodsType(type,ok);
        break;
    default:
        break;
    }
}




void dataCenter::net_newGoodsType(const QJsonObject para)
{
    bool ok = false;
    QString type = GoodsService::newGoodsType(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    //    if(ok){
    //        m_goodsType.push_back(type);
    //    }
    //    emit sig_newGoodsType(type,ok);
    pri_opt_GoodsType(ok,type,NoticeType_NEW);
}

void dataCenter::net_delGoodsType(const QJsonObject para)
{
    bool ok = false;
    QString type = GoodsService::delGoodsType(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    //    m_goodsType.removeOne(type);
    //    emit sig_delGoodsType(type,ok);
    pri_opt_GoodsType(ok,type,NoticeType_Modify);
}

void dataCenter::net_getGlobalGoodsType()
{
    bool ok = false;
    m_goodsType = GoodsService::getAllGoodsType(ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    emit sig_globalGoodsType(ok);
}

void dataCenter::pri_opt_OutRecord(bool ok, GoodsOutRecord &record, enum_NoticeType noticeType)
{
    switch (noticeType) {
    case NoticeType_NEW:
        if (ok){
            m_goodsRecords.push_back(record);
        }
        emit sig_newGoodsRecord(record,ok);
        break;
    case NoticeType_Modify:

        break;
    case NoticeType_Del:

        break;
    default:
        break;
    }
}




void dataCenter::net_newGoodsOut(const QJsonObject para)
{
    bool ok = false;
    GoodsOutRecord record = GoodsOutRecordService::newGoodsRecord(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    //    if (ok){
    //        m_goodsRecords.push_back(record);
    //    }
    //    emit sig_newGoodsRecord(goods,ok);
    pri_opt_OutRecord(ok,record,NoticeType_NEW);
}

void dataCenter::net_getAllOutRecords()
{
    bool ok = false;
    m_goodsRecords = GoodsOutRecordService::GetAllRecords(ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    emit sig_getAllOutRecord(ok);
}


void dataCenter::pri_opt_Platting(bool ok, QString &platting, enum_NoticeType noticeType)
{
    switch (noticeType) {
    case NoticeType_NEW:
        if(ok){
            m_Platings.append(platting);
        }
        emit sig_newPlating(platting,ok);
        break;
    case NoticeType_Modify:

        break;
    case NoticeType_Del:
        if(ok){
            m_Platings.removeOne(platting);
        }
        emit sig_delPlating(platting,ok);
        break;
    default:
        break;
    }
}


void dataCenter::net_newPlating(const QJsonObject para)
{
    bool ok = false;
    QString platting =  PlatingService::newPlating(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    //    if(ok){
    //        m_Platings.append(unit);
    //    }
    //    emit sig_newPlating(unit,ok);
    pri_opt_Platting(ok,platting,NoticeType_NEW);
}

void dataCenter::net_delPlating(const QJsonObject para)
{
    bool ok = false;
    QString platting = PlatingService::delPlating(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    //    if(ok){
    //        m_Platings.removeOne(unit);
    //    }
    //    emit sig_delPlating(unit,ok);
    pri_opt_Platting(ok,platting,NoticeType_Del);
}

void dataCenter::net_getglobalPlating()
{
    bool ok =false;
    m_Platings = PlatingService::getAllPlating(ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    emit sig_globalPlating(ok);
}


void dataCenter::net_HeartBeat()
{
    bool ok;
    Notification::HeartBeat(QJsonObject(),ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    qDebug()<<"HeartBeat:"<<ok;
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
        if(status==Status_New){
            if(o.Current.Status==Status_New||o.Current.Status==Status_PartProduce||o.Current.Status==Status_Part_Part){
                ls.append(o);
                continue;
            }
            //            if(o.ProduceNum<o.OrderNum){
            //                ls.append(o);
            //                continue;
            //            }
        }
        if(status==Status_Produce){
            if(o.Current.Status==Status_PartProduce||o.Current.Status==Status_Part_Part||o.Current.Status==Status_Produce){
                ls.append(o);
                continue;
            }
            //            if(o.ProduceNum>0&&o.SuccessNum<o.OrderNum){
            //                ls.append(o);
            //                continue;
            //            }
        }
        if(status==Status_Success){
            //            if(o.SuccessNum>0){
            //                ls.append(o);
            //                continue;
            //            }
            if(o.Current.Status==Status_PartSuccess||o.Current.Status==Status_Part_Part||o.Current.Status==Status_Success){
                ls.append(o);
                continue;
            }
        }
    }
    return ls;
}

QVector<Order> dataCenter::pub_orders()
{
    return m_orders;
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

QString dataCenter::put_getPrintNumber()
{
    return m_print_number;;
}


QVector<QString> dataCenter::pub_Units()
{
    return m_units;
}

bool dataCenter::pub_checkUnitExist(QString unit)
{
    return m_units.contains(unit);
}

QVector<QString> dataCenter::pub_Platings()
{
    return m_Platings;
}

bool dataCenter::pub_checkPlatingExist(QString unit)
{
    return m_Platings.contains(unit);
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

Materiel dataCenter::pub_getMaterielFromSolidID(QString sID, bool &ok)
{
    ok = true;
    for(Materiel m:m_maters){
        if (m.ComponentSolid==sID){
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

bool dataCenter::pub_checkComponentSolid(QString solid)
{
    for(Materiel m:m_maters){
        if (m.ComponentSolid==solid){
            return true;
        }
    }
    return false;
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

bool dataCenter::pub_checkSuppliser(QString name)
{
    for(Supplier m:m_suppliers){
        if (m.Name==name){
            return true;
        }
    }
    return false;
}

SysSetting dataCenter::CurSettings()
{
    return m_Config.Setting();
}

QSet<QString> dataCenter::Accounts()
{
    return m_Config.accounts();
}

void dataCenter::setCurSettings(SysSetting set)
{
    m_Config.setSetting(set);
}



void dataCenter::newNotice(QJsonObject &obj)
{
    NoticeInfo info = Notification::NoticeformJson(obj);
    switch (info.DataType) {
    case STRTUCT_ORDER:
        if(info.Data.isObject()){
            Order o = OrderService::fromJsonObject(info.Data.toObject());
            pri_opt_Order(true,o,(enum_NoticeType)(info.NoticeType));
        }
        break;
    case STRUCT_MATERIAL:

        break;
    case STRUCT_GOODS:
        if(info.Data.isObject()){
            Goods goods = GoodsService::fromJsonObject(info.Data.toObject());
            pri_opt_Goods(true,goods,(enum_NoticeType)info.NoticeType);
        }
        break;
    case STRUCT_OUTRECORD:
        if(info.Data.isObject()){
            GoodsOutRecord record = GoodsOutRecordService::fromJsonObject(info.Data.toObject());
            pri_opt_OutRecord(true,record,(enum_NoticeType)info.NoticeType);
        }
        break;
    case STRUCT_USER:
        if(info.Data.isObject()){
            User user = UserService::fromJsonObject(info.Data.toObject());
            pri_opt_User(true,user,(enum_NoticeType)info.NoticeType);
        }
        break;
    case STRUCT_CUSTOMER:
        if(info.Data.isObject()){
            Customer cus = CustomerService::fromJsonObject(info.Data.toObject());
            pri_opt_Customer(true,cus,(enum_NoticeType)info.NoticeType);
        }
        break;
    case STRUCT_SUPPLIER:
        if(info.Data.isObject()){
            Supplier sup = SupplierService::fromJsonObject(info.Data.toObject());
            pri_opt_supplier(true,sup,(enum_NoticeType)info.NoticeType);
        }
        break;
    case STRUCT_UNIT:
        if(info.Data.isString()){
            QString unit = info.Data.toString();
            pri_opt_Unit(true,unit,(enum_NoticeType)info.NoticeType);
        }
        break;
    case STRUCT_DEPARTMENT:
        if(info.Data.isString()){
            QString de = info.Data.toString();
            pri_opt_DepartMent(true,de,(enum_NoticeType)info.NoticeType);
        }
        break;
    case STRUCT_GOODS_TYPE:
        if(info.Data.isString()){
            QString ty = info.Data.toString();
            pri_opt_GoodsType(true,ty,(enum_NoticeType)info.NoticeType);
        }
        break;
    case STRUCT_PLANTING:
        if(info.Data.isString()){
            QString platting = info.Data.toString();
            pri_opt_Platting(true,platting,(enum_NoticeType)info.NoticeType);
        }
        break;
    default:
        break;
    }
}
void dataCenter::TimerUpdate(bool isstop)
{
    if(isstop){
        if(m_first_timer!=NULL&&m_first_timer->isActive()) m_first_timer->stop();
        if(m_second_timer!=NULL&&m_second_timer->isActive()) m_second_timer->stop();
        if(m_third_timer!=NULL&&m_third_timer->isActive()) m_third_timer->stop();
    }else{
        m_first_timer = new QTimer(this);
        connect(m_first_timer, SIGNAL(timeout()), this, SLOT(update_first()));
        m_first_timer->start(1000*60*2);

        m_second_timer = new QTimer(this);
        connect(m_second_timer, SIGNAL(timeout()), this, SLOT(update_second()));
        m_second_timer->start(1000*60*10);

        m_third_timer = new QTimer(this);
        connect(m_third_timer, SIGNAL(timeout()), this, SLOT(update_third()));
        m_third_timer->start(1000*60*20);


    }
}

void dataCenter::update_first()
{
    //////////////初始化所有订单///////////////////
    boost::thread (boost::bind(&dataCenter::net_getglobalOrders,dataCenter::instance())).detach();
    //////////////获取打印数量/////////////////////////////
    boost::thread(boost::bind(&dataCenter::net_getPrintNumber,dataCenter::instance())).detach();
    m_first_timer->start(1000*60*2);
}

void dataCenter::update_second()
{
    //////////////初始化所有单位///////////////////
    boost::thread (boost::bind(&dataCenter::net_getglobalUnits,dataCenter::instance())).detach();

    //////////////初始化所有物料//////////////////
    boost::thread (boost::bind(&dataCenter::net_getglobalMateriels,dataCenter::instance())).detach();
    //////////////获取所有商品的出库记录//////////////////////////////
    boost::thread(boost::bind(&dataCenter::net_getAllOutRecords,dataCenter::instance())).detach();

    //////////////初始化所有镀种的分类//////////////////
    boost::thread(boost::bind(&dataCenter::net_getglobalPlating,dataCenter::instance())).detach();

    m_second_timer->start(1000*60*10);
}

void dataCenter::update_third()
{

    //////////////初始化所有商品//////////////////
    boost::thread(boost::bind(&dataCenter::net_getglobalGoods,dataCenter::instance())).detach();

    //////////////初始化所有供应商///////////////////
    boost::thread (boost::bind(&dataCenter::net_getglobalSuppliers,dataCenter::instance())).detach();

    //////////////初始化所有客户///////////////////
    boost::thread (boost::bind(&dataCenter::net_getGlobalCustomers,dataCenter::instance())).detach();

    //////////////初始化所有员工///////////////////
    boost::thread (boost::bind(&dataCenter::net_getGlobalUsers,dataCenter::instance())).detach();

    //////////////初始化所有部门///////////////////
    boost::thread (boost::bind(&dataCenter::net_getGlobalDepartment,dataCenter::instance())).detach();

    //////////////初始化所有商品的分类//////////////////
    boost::thread(boost::bind(&dataCenter::net_getGlobalGoodsType,dataCenter::instance())).detach();



    m_third_timer->start(1000*60*20);
}

void dataCenter::heartbeat()
{
    //////////////心跳//////////////////
    boost::thread(boost::bind(&dataCenter::net_HeartBeat,dataCenter::instance())).detach();
    m_heartbeat_timer->start(1000*60);
}

void dataCenter::pri_initBath()
{
    m_batch.clear();
    for(Order order:m_orders){
        m_batch.insert(order.CustomBatch);
    }
}

void dataCenter::pri_checkGoodType(QString type)
{
    for(QString t:m_goodsType){
        if(t == type){
            return;
        }
    }
    m_goodsType.push_back(type);
}

void dataCenter::pri_removeCustomerMaterial(QString cid, QString materialID)
{
    if(m_hashMaterials.contains(cid)){
        m_hashMaterials[cid].removeOne(materialID);
    }
}

void dataCenter::pri_addCustomerMaterial(QString cid, QString materialID)
{
    if(m_hashMaterials.contains(cid)){
        m_hashMaterials[cid].push_back(materialID);
    }else{
        QVector<QString> ls;
        ls.push_back(materialID);
        m_hashMaterials[cid] = ls;
    }
}



QVector<QString> dataCenter::pub_goodsType() const
{
    return m_goodsType;
}

QVector<Goods> dataCenter::pub_GetTypeGoods(QString type)
{
    QVector<Goods>  list;
    for(Goods g:m_goods){
        if(g.Type == type){
            list.push_back(g);
        }
    }
    return list;
}

bool dataCenter::pub_checkTypeExist(QString type)
{
    return m_goodsType.contains(type);
}

QVector<GoodsOutRecord> dataCenter::pub_AllOutRecord()
{
    return m_goodsRecords;
}

QVector<Goods> dataCenter::pub_goods() const
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
    ok = false;
    return  s;
}


QVector<QString> dataCenter::pub_getDepartments() const
{
    return m_departments;
}

bool dataCenter::pub_checkDepartment(QString depart)
{
    return  m_departments.contains(depart);
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
