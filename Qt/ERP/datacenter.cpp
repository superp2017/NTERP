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
#include "update.h"


dataCenter::dataCenter(QObject *parent) : QObject(parent)//,m_notice(parent)
{
    m_authors.push_back("操作员");
    m_authors.push_back("仓库");
    m_authors.push_back("财务");
    m_authors.push_back("管理员");
    m_authors.push_back("超级管理员");

    m_first_block.t_timer = NULL;
    m_second_block.t_timer = NULL;
    m_thrid_block.t_timer = NULL;

    m_first_block.t_timeout  = 1000*60*m_Config.getFirstTime();
    m_second_block.t_timeout = 1000*60*m_Config.getSecondTime();
    m_thrid_block.t_timeout  = 1000*60*m_Config.getThirdTime();
    m_isOrderOver = false;

}


void dataCenter::initData()
{

    //获取最新版本
    QJsonObject obj;
    boost::thread (boost::bind(&dataCenter::net_getVersion,dataCenter::instance(),obj)).detach();

    //////////////初始化所有订单///////////////////
    pub_getAllOrders(2);

    //////////////初始化所有单位///////////////////
    boost::thread (boost::bind(&dataCenter::net_getglobalUnits,dataCenter::instance())).detach();

    //////////////初始化所有员工///////////////////
    pub_getAllEmployee(0);

    //////////////初始化所有客户///////////////////
    pub_GetAllCustomers(0);

    //////////////初始化所有部门///////////////////
    boost::thread (boost::bind(&dataCenter::net_getGlobalDepartment,dataCenter::instance())).detach();

    //////////////初始化所有供应商///////////////////
    pub_getAllSuppliers(0);

    //////////////初始化所有物料//////////////////
    pub_getAllMateriels(0);

    //////////////初始化所有商品//////////////////
    pub_getAllGoods(0);

    //////////////初始化所有商品的分类//////////////////
    boost::thread(boost::bind(&dataCenter::net_getGlobalGoodsType,dataCenter::instance())).detach();

    //////////////初始化所有镀种的分类//////////////////
    boost::thread(boost::bind(&dataCenter::net_getglobalPlating,dataCenter::instance())).detach();

    //////////////获取所有商品的出库记录//////////////////////////////
    pub_getAllOutRecord(0);
    //////////////获取打印数量/////////////////////////////
    boost::thread(boost::bind(&dataCenter::net_getPrintNumber,dataCenter::instance())).detach();


}

void dataCenter::clearData()
{
    TimerUpdate(true);

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
    User user = UserService::delUser(para,isOK,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(isOK){
        for(int i=0;i<m_employee.size();++i){
            if(m_employee[i].UID==user.UID){
                m_employee.remove(i);
                break;
            }
        }
    }
    emit sig_delEmployee(user.UID,isOK);
}

void dataCenter::net_getGlobalUsers(const QJsonObject para)
{
    bool ok =false;
    QVector<User>  list = UserService::getAllUsers(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(ok){
        if(para.value("type")==0){
            m_employee = list;
        }else{
            for(User s:list){
                m_employee.push_back(s);
            }
        }
    }
    emit sig_globalEmployees(ok);
}

void dataCenter::net_searchOrder(const QJsonObject para)
{
    bool ok =false;
    QVector<Order>  list = OrderService::SearchOrder(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    emit sig_searchOrder(list,ok);
}


void dataCenter::net_newDepartment(const QJsonObject para)
{
    bool ok = false;
    QString de = UserService::newDepartment(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    m_departments.push_back(de);
    emit sig_newDepartment(de,ok);

}

void dataCenter::net_delDepartment(const QJsonObject para)
{
    bool ok = false;
    QString de = UserService::delDepartment(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    m_departments.removeOne(de);
    emit sig_delDepartment(de,ok);

}

void dataCenter::net_getGlobalDepartment()
{
    bool ok = false;
    m_departments = UserService::getAllDepartment(QJsonObject(),ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    emit sig_globalDepartment(ok);
}




void dataCenter::net_newOrder(const QJsonObject para)
{
    bool isOK   = false;
    Order order = OrderService::newOrder(para,isOK,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(isOK){
        m_orders.append(order);
        m_batch.insert(order.CustomBatch);
        m_orders_set.insert(order.OrderID);
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
    Order order = OrderService::delOrder(para,isOK,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(isOK){
        for(int i=0;i<m_orders.size();++i){
            if(m_orders[i].OrderID==order.OrderID){
                m_orders.remove(i);
                break;
            }
        }
    }
    emit sig_delOrder(order,isOK);
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

void dataCenter::net_updatePrintNum(const QJsonObject para)
{
    bool isOK   = false;
    QVector<Order> list = OrderService::updatePrintNum(para,isOK,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(isOK){
        for (Order o:list){
            for(int i=0;i<m_orders.size();++i){
                if(m_orders[i].OrderID==o.OrderID){
                    m_orders[i] = o;
                    break;
                }
            }
        }
    }
    emit sig_updatePrintNum(list,isOK);
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

void dataCenter::net_getglobalOrders(const QJsonObject para)
{
    bool ok =false;
    bool isov = false;
    QVector<Order>  list = OrderService::getAllOrders(para,ok,isov,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    int type = para.value("Type").toInt();
    if(ok){
        if(type==0){
            m_orders_set.clear();
            m_orders = list;
            for(Order s:list){
                m_orders_set.insert(s.OrderID);
            }
            m_isOrderOver = true;
        }else{
            for(Order o:list){
                if(m_orders_set.contains(o.OrderID)){
                    continue;
                }
                m_orders.push_back(o);
                m_orders_set.insert(o.OrderID);
            }
            if(type==2){
                m_isOrderOver = isov;
            }
        }

        if(m_orders.size()>0){
            m_last_orderID = m_orders.at(m_orders.size()-1).OrderID;
        }
    }

    pri_initBath();
    emit sig_globalOrders(ok,type,list.size()>0);
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
    Customer  cus =CustomerService::delCustomer(para,isOK,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(isOK){
        for(int i=0;i<m_customers.size();++i){
            if(m_customers[i].CID==cus.CID){
                m_customers.remove(i);
                break;
            }
        }
    }
    emit sig_delCustomer(cus.CID,isOK);
}

void dataCenter::net_getGlobalCustomers(const QJsonObject para)
{
    bool ok = false;
    QVector<Customer> list = CustomerService::getAllCustomer(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(ok){
        if(para.value("type")==0){
            m_customers = list;
        }else{
            for(Customer s:list){
                m_customers.push_back(s);
            }
        }
    }
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
    Supplier sup = SupplierService::newSupplier(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(ok){
        m_suppliers.push_back(sup);
    }
    emit sig_newSupplier(sup,ok);
}

void dataCenter::net_modSupplier(const QJsonObject para)
{
    bool ok = false;
    Supplier sup =SupplierService::modSupplier(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(ok){
        for(int i=0;i<m_suppliers.size();++i){
            if(m_suppliers[i].SID==sup.SID){
                m_suppliers[i] = sup;
                break;
            }
        }
    }
    emit sig_modSUpplier(sup,ok);
}

void dataCenter::net_delSupplier(const QJsonObject para)
{
    bool ok = false;
    Supplier sup = SupplierService::delSupplier(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(ok){
        for(int i=0;i<m_suppliers.size();++i){
            if(m_suppliers[i].SID==sup.SID){
                m_suppliers.remove(i);
                break;
            }
        }
    }
    emit sig_delSUpplier(sup.SID,ok);
}

void dataCenter::net_getglobalSuppliers(const QJsonObject para)
{
    bool ok = false;
    QVector<Supplier>list = SupplierService::getAllSupplierls(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(ok){
        if(para.value("type")==0){
            m_suppliers = list;
        }else{
            for(Supplier s:list){
                m_suppliers.push_back(s);
            }
        }
    }
    emit sig_globalSUppliers(ok);
}


void dataCenter::net_newMaterial(const QJsonObject para)
{
    bool ok = false;
    Materiel ma = MaterialService::newMaterial(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(ok){
        m_maters.push_back(ma);
        pri_addCustomerMaterial(ma.CID,ma.MaterID);
    }
    emit sig_newMaterial(ma,ok);
}

void dataCenter::net_modMaterial(const QJsonObject para)
{
    bool ok = false;
    Materiel mater =MaterialService::modMaterial(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(ok){
        for(int i=0;i<m_maters.size();++i){
            if(m_maters[i].MaterID==mater.MaterID){
                m_maters[i] = mater;
                break;
            }
        }
    }
    emit sig_modMaterial(mater,ok);
}

void dataCenter::net_delMaterial(const QJsonObject para)
{
    bool ok = false;
    Materiel mater = MaterialService::delMaterial(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(ok){
        for(int i=0;i<m_maters.size();++i){
            if(m_maters[i].MaterID==mater.MaterID){
                m_maters.remove(i);
                pri_removeCustomerMaterial(mater.CID,mater.MaterID);
                break;
            }
        }
    }
    emit sig_delMaterial(mater.MaterID,ok);
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

void dataCenter::net_getglobalMateriels(const QJsonObject para)
{
    bool ok =false;
    QVector<Materiel>  list = MaterialService::getAllMateriels(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(ok){
        if(para.value("type")==0){
            m_maters = list;
        }else{
            for(Materiel s:list){
                m_maters.push_back(s);
            }
        }
    }
    emit sig_globalMateriels(ok);

}



void dataCenter::net_newGoods(const QJsonObject para)
{
    bool ok = false;
    Goods goods = GoodsService::newGoods(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if (ok){
        m_goods.push_back(goods);
        pri_checkGoodType(goods.Type);
    }
    emit sig_newGoods(goods,ok);
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
    if(ok){
        bool ex =false;
        for(int i=0;i<m_goods.size();++i){
            if(m_goods[i].ID==goods.ID){
                m_goods[i] = goods;
                break;
            }
        }
        if(!ex) m_goods.push_back(goods);
        pri_checkGoodType(goods.Type);
    }
    emit sig_modGoods(goods,ok);
}

void dataCenter::net_delGoods(const QJsonObject para)
{
    bool ok = false;
    Goods goods = GoodsService::delGoods(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(ok){
        for(int i=0;i<m_goods.size();++i){
            if(m_goods[i].ID==goods.ID){
                m_goods.remove(i);
                break;
            }
        }
    }
    emit sig_delGoods(goods.ID,ok);
}

void dataCenter::net_addOutGoodsNum(const QJsonObject para)
{
    bool ok = false;
    Goods goods = GoodsService::addOutGoodsNum(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(ok){
        for(int i=0;i<m_goods.size();++i){
            if(m_goods[i].ID==goods.ID){
                m_goods[i] = goods;
                break;
            }
        }
    }
    emit sig_inGoods(goods,ok);
}

void dataCenter::net_getglobalGoods(const QJsonObject para)
{
    bool ok = false;
    QVector<Goods> list = GoodsService::getAllGoods(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(ok){
        if(para.value("type")==0){
            m_goods = list;
        }else{
            for(Goods s:list){
                m_goods.push_back(s);
            }
        }
    }
    emit sig_globalGoods(ok);
}


void dataCenter::net_newGoodsType(const QJsonObject para)
{
    bool ok = false;
    QString type = GoodsService::newGoodsType(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(ok){
        m_goodsType.push_back(type);
    }
    emit sig_newGoodsType(type,ok);
}

void dataCenter::net_delGoodsType(const QJsonObject para)
{
    bool ok = false;
    QString type = GoodsService::delGoodsType(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    m_goodsType.removeOne(type);
    emit sig_delGoodsType(type,ok);
}

void dataCenter::net_getGlobalGoodsType()
{
    bool ok = false;
    m_goodsType = GoodsService::getAllGoodsType(ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    emit sig_globalGoodsType(ok);
}



void dataCenter::net_newGoodsOut(const QJsonObject para)
{
    bool ok = false;
    GoodsOutRecord record = GoodsOutRecordService::newGoodsRecord(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if (ok){
        m_goodsRecords.push_back(record);
    }
    emit sig_newGoodsRecord(record,ok);
}

void dataCenter::net_getAllOutRecords(const QJsonObject para)
{
    bool ok = false;
    QVector<GoodsOutRecord>list = GoodsOutRecordService::GetAllRecords(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(ok){
        if(para.value("type")==0){
            m_goodsRecords = list;
        }else{
            for(GoodsOutRecord s:list){
                m_goodsRecords.push_back(s);
            }
        }
    }
    emit sig_getAllOutRecord(ok);
}


void dataCenter::net_newPlating(const QJsonObject para)
{
    bool ok = false;
    QString platting =  PlatingService::newPlating(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(ok){
        m_Platings.append(platting);
    }
    emit sig_newPlating(platting,ok);
}

void dataCenter::net_delPlating(const QJsonObject para)
{
    bool ok = false;
    QString platting = PlatingService::delPlating(para,ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    if(ok){
        m_Platings.removeOne(platting);
    }
    emit sig_delPlating(platting,ok);
}

void dataCenter::net_getglobalPlating()
{
    bool ok =false;
    m_Platings = PlatingService::getAllPlating(ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    emit sig_globalPlating(ok);
}


void dataCenter::net_HeartBeat(int index)
{
    bool ok;
    int time =update::HeartBeat(QJsonObject(),ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    switch (index) {
    case 1:
        m_first_block.t_stamp = time;
        break;
    case 2:
        m_second_block.t_stamp = time;
        break;
    case 3:
        m_thrid_block.t_stamp = time;
        break;
    case -1:
        m_first_block.t_stamp = time;
        m_second_block.t_stamp = time;
        m_thrid_block.t_stamp = time;
        break;
    default:
        break;
    }
    qDebug()<<"HeartBeat:"<<ok;
}

void dataCenter::net_getVersion(const QJsonObject para)
{
    bool ok;
    VersionInfo v = update::GetVersion(QJsonObject(),ok,m_Config.HOST_NAME(),m_Config.HOST_PORT());
    m_version_manager.setNetVersion(v,ok);
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
        }
        if(status==Status_Produce){
            if(o.Current.Status==Status_PartProduce||o.Current.Status==Status_Part_Part||o.Current.Status==Status_Produce){
                ls.append(o);
                continue;
            }
        }
        if(status==Status_Success){
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

QVector<Materiel> dataCenter::pub_getMaterielFromSolidID(QString sID)
{
    QVector<Materiel> ls;
    for(Materiel m:m_maters){
        if (m.ComponentSolid==sID){
           ls.push_back(m);
        }
    }
    return ls;
}


QSet<QString> dataCenter::pub_Batchs()
{
    return m_batch;
}

int dataCenter::pub_checkComponentSolid(QString solid, QString CID)
{
    for(Materiel m:m_maters){
        if (m.ComponentSolid==solid){
            if(m.CID==CID){
                return -1;
            }else{
                return 1;
            }
        }
    }
    return 100;
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

void dataCenter::TimerUpdate(bool istart)
{
    if(!istart){
        if(m_first_block.t_timer!=NULL&&m_first_block.t_timer->isActive()) m_first_block.t_timer->stop();
        if(m_second_block.t_timer!=NULL&&m_second_block.t_timer->isActive()) m_second_block.t_timer->stop();
        if(m_thrid_block.t_timer!=NULL&&m_thrid_block.t_timer->isActive()) m_thrid_block.t_timer->stop();
    }else{
        m_first_block.t_timer = new QTimer(this);
        connect(m_first_block.t_timer, SIGNAL(timeout()), this, SLOT(update_first()));
        m_first_block.t_timer->start(m_first_block.t_timeout);

        m_second_block.t_timer = new QTimer(this);
        connect(m_second_block.t_timer, SIGNAL(timeout()), this, SLOT(update_second()));
        m_second_block.t_timer->start(m_second_block.t_timeout);

        m_thrid_block.t_timer = new QTimer(this);
        connect(m_thrid_block.t_timer, SIGNAL(timeout()), this, SLOT(update_third()));
        m_thrid_block.t_timer->start(m_thrid_block.t_timeout);
        ////////////心跳//////////////////
        boost::thread(boost::bind(&dataCenter::net_HeartBeat,dataCenter::instance(),-1)).detach();
    }
}

void dataCenter::pub_getAllOrders(int type,QString start,int num)
{
    //////////////初始化所有订单///////////////////
    QJsonObject order_obj;
    order_obj.insert("Type",type);
    order_obj.insert("Start",start);
    if(type ==2){
        if(m_isOrderOver) {
            qDebug()<<"pub_getAllOrders load more is over!";
            return;
        }
        order_obj.insert("Start",m_last_orderID);
    }
    order_obj.insert("Num",num);
    order_obj.insert("Stamp",m_first_block.t_stamp);
    boost::thread (boost::bind(&dataCenter::net_getglobalOrders,dataCenter::instance(),order_obj)).detach();
}

int dataCenter::checkVersion(QWidget *W)
{
  return  m_version_manager.checkVersion(W);
}


void dataCenter::pub_getAllMateriels(int type,QString start,int num)
{
    //////////////初始化所有物料//////////////////
    QJsonObject mater_obj;
    mater_obj.insert("Type",type);
    mater_obj.insert("Start",start);
    mater_obj.insert("Num",num);
    mater_obj.insert("Stamp",m_second_block.t_stamp);
    boost::thread (boost::bind(&dataCenter::net_getglobalMateriels,dataCenter::instance(),mater_obj)).detach();
}

void dataCenter::pub_getAllOutRecord(int type,QString start,int num)
{
    //////////////获取所有商品的出库记录//////////////////////////////
    QJsonObject out_obj;
    out_obj.insert("Type",type);
    out_obj.insert("Start",start);
    out_obj.insert("Num",num);
    out_obj.insert("Stamp",m_second_block.t_stamp);
    boost::thread(boost::bind(&dataCenter::net_getAllOutRecords,dataCenter::instance(),out_obj)).detach();
}

void dataCenter::pub_getAllGoods(int type, QString start, int num)
{
    //////////////初始化所有商品//////////////////
    QJsonObject goods_obj;
    goods_obj.insert("Type",type);
    goods_obj.insert("Start",start);
    goods_obj.insert("Num",num);
    goods_obj.insert("Stamp",m_thrid_block.t_stamp);
    boost::thread(boost::bind(&dataCenter::net_getglobalGoods,dataCenter::instance(),goods_obj)).detach();
}

void dataCenter::pub_getAllSuppliers(int type,QString start,int num)
{
    //////////////初始化所有供应商///////////////////
    QJsonObject sup_obj;
    sup_obj.insert("Type",type);
    sup_obj.insert("Start",start);
    sup_obj.insert("Num",num);
    sup_obj.insert("Stamp",m_thrid_block.t_stamp);
    boost::thread (boost::bind(&dataCenter::net_getglobalSuppliers,dataCenter::instance(),sup_obj)).detach();
}

void dataCenter::pub_GetAllCustomers(int type,QString start,int num)
{
    //////////////初始化所有客户///////////////////
    QJsonObject cus_obj;
    cus_obj.insert("Type",type);
    cus_obj.insert("Start",start);
    cus_obj.insert("Num",num);
    cus_obj.insert("Stamp",m_thrid_block.t_stamp);
    boost::thread (boost::bind(&dataCenter::net_getGlobalCustomers,dataCenter::instance(),cus_obj)).detach();
}

void dataCenter::pub_getAllEmployee(int type,QString start,int num)
{
    //////////////初始化所有员工///////////////////
    QJsonObject user_obj;
    user_obj.insert("Type",type);
    user_obj.insert("Start",start);
    user_obj.insert("Num",num);
    user_obj.insert("Stamp",m_thrid_block.t_stamp);
    boost::thread (boost::bind(&dataCenter::net_getGlobalUsers,dataCenter::instance(),user_obj)).detach();
}

void dataCenter::update_first()
{
    qDebug()<<"update_first";
    //////////////初始化所有订单///////////////////
    pub_getAllOrders(1);
    //////////////获取打印数量/////////////////////////////
    boost::thread(boost::bind(&dataCenter::net_getPrintNumber,dataCenter::instance())).detach();
    m_first_block.t_timer->start(m_first_block.t_timeout);

    //////////////心跳//////////////////
    boost::thread(boost::bind(&dataCenter::net_HeartBeat,dataCenter::instance(),1)).detach();

    pub_getAllOrders(2);
}

void dataCenter::update_second()
{
    qDebug()<<"update_second";
    //////////////初始化所有单位///////////////////
    boost::thread (boost::bind(&dataCenter::net_getglobalUnits,dataCenter::instance())).detach();

    //////////////初始化所有物料//////////////////
    pub_getAllMateriels(1);

    //////////////获取所有商品的出库记录//////////////////////////////
    pub_getAllOutRecord(1);

    //////////////初始化所有镀种的分类//////////////////
    boost::thread(boost::bind(&dataCenter::net_getglobalPlating,dataCenter::instance())).detach();

    //////////////心跳//////////////////
    boost::thread(boost::bind(&dataCenter::net_HeartBeat,dataCenter::instance(),2)).detach();

    m_second_block.t_timer->start(m_second_block.t_timeout);
}

void dataCenter::update_third()
{
    qDebug()<<"update_third";
    //////////////初始化所有商品//////////////////
    pub_getAllGoods(1);

    //////////////初始化所有供应商///////////////////
    pub_getAllSuppliers(1);

    //////////////初始化所有客户///////////////////
    pub_GetAllCustomers(1);

    //////////////初始化所有员工///////////////////
    pub_getAllEmployee(1);

    //////////////初始化所有部门///////////////////
    boost::thread (boost::bind(&dataCenter::net_getGlobalDepartment,dataCenter::instance())).detach();

    //////////////初始化所有商品的分类//////////////////
    boost::thread(boost::bind(&dataCenter::net_getGlobalGoodsType,dataCenter::instance())).detach();

    //////////////心跳//////////////////
    boost::thread(boost::bind(&dataCenter::net_HeartBeat,dataCenter::instance(),3)).detach();

    m_thrid_block.t_timer->start(m_thrid_block.t_timeout);
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

bool dataCenter::isOrderOver() const
{
    return m_isOrderOver;
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
