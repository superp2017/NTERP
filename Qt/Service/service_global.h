#ifndef SERVICE_GLOBAL_H
#define SERVICE_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QString>
#include <QVector>
#include <QJsonValue>
#include <QJsonArray>
#include <QDebug>

#pragma execution_character_set("utf-8")

#if defined(SERVICE_LIBRARY)
#  define SERVICESHARED_EXPORT Q_DECL_EXPORT
#else
#  define SERVICESHARED_EXPORT Q_DECL_IMPORT
#endif

const std::string  Net_Login="/login";                      //用户登录


const std::string  Net_NewOrder="/neworder";                //新建订单
const std::string  Net_ModOrder="/modorder";                //修改订单
const std::string  Net_DelOrder="/delorder";                //删除订单
const std::string  Net_ProductOrder="/produceorder";        //订单生产
const std::string  Net_CancelOrder="/cancleorder";          //取消订单
const std::string  Net_SuccessOrder="/successorder";        //完成订单
const std::string  Net_ModOrderPrice="/modorderprice";      //修改订单价格
const std::string  Net_GlobalOrders="/getglobalorders";     //获取所有订单


const std::string  Net_NewCustomer="/newcustomer";          //新建客户
const std::string  Net_ModCustomer="/modcustomer";          //修改客户
const std::string  Net_DelCustomer="/delcustomer";          //删除客户
const std::string  Net_upDownCustomer="/updowncustomer";       //解约或者合作客户
const std::string  Net_GlobalCustomers="/getallcustomers";  //获取所有客户


const std::string  Net_NewSupplier="/newsupplier";          //新增一个供应商
const std::string  Net_ModSupplier="/modsupplier";          //修改供应商
const std::string  Net_DelSupplier="/delsupplier";          //删除一个供应商
const std::string  Net_GlobalSuppliers="/getallsuppliers";  //获取所有供应商


const std::string  Net_NewEmployee="/newemployee";          //新建一个员工
const std::string  Net_ModEmployee="/modemployee" ;         //修改员工信息
const std::string  Net_DelEmployee="/delemployee" ;         //删除一个员工
const std::string  Net_OutEmployee="/outemployee" ;         //员工离职
const std::string  Net_GlobalEmployee="/getallemployeess";  //获取所有的员工信息


const std::string Net_NewMaterial="/newmaterial";           //"创建一个材料"
const std::string Net_QueryMaterial="/querymaterial";       //"查询一个材料"
const std::string Net_ModMaterial="/modMaterial";           //"修改材料"
const std::string Net_DelMaterial="/delmaterial";           //"删除材料"
const std::string Net_GetCustomerMaterial="/getcustomermaterial";//获取某个用户的所有物料
const std::string Net_GetAllMateril="/getallmaterials";    //获取所有材料


const std::string  Net_NewUnit="/newunit";                  //新建单位
const std::string  Net_DelUnit="/delunit";                  //删除单位
const std::string  Net_GlobalUnit="/getallunits";           //获取所有单位


const std::string  Net_NewPlating="/newplating";                  //新建镀种
const std::string  Net_DelPlating="/delplating";                  //删除镀种
const std::string  Net_GlobalPlating="/getallplating";           //获取所有镀种


const std::string Net_NewGoods="/newgoods";                 //新建一个商品
const std::string Net_QueryGoods="/querygoods";             //查询商品信息
const std::string Net_ModifyGoods="/modifygoods";           //修改商品
const std::string Net_InGoodsNum="/addgoodsnum";             //商品入库
const std::string Net_DelGoods="/delgoods";                 //删除商品
const std::string Net_SupplierGoods="/getsuppliergoods";    //获取供应商的供应的商品
const std::string Net_GlobalGoods="/getglobalgoods";        //获获取所有的商品

const std::string Net_NewGoodsType="/newgoodstype";         //新建一个商品分类
const std::string Net_RemoveGoodsType="/removegoodstype";   //删除一个商品分类
const std::string Net_GetAllGoodsType="/getallgoodstype";   //获取所有的商品分类

const std::string Net_GetAllGoodsOutRecord="/getallstorageoutrecords";   //获取所有的商品出库记录
const std::string Net_NewGoodsOutRecord="/newstorageoutrecord";   //新增一个商品出库记录



const std::string Net_NewDepartment="/newdepartment";       //获取所有的商品分类
const std::string Net_DelDepartment="/removedepartment";    //获取所有的商品分类
const std::string Net_GetAllDepartment="/getalldepartment"; //获取所有的商品分类






struct Ret{
    QString     msg;
    bool        ret;
    QJsonValue data;
};



struct OderFlow{
    QString UserName;   //用户姓名
    QString OpreatTime; //操作时间
    QString Action;     //动作
    QString Status;     //状态
};


struct Order{
    QString           UID;            //用户id
    QString           UserName;       //用户姓名
    QString           OrderID;        //订单id
    QString           OrderType;      //订单类型（普通订单、批量订单、试样订单）
    QString           MaterielID;     //材料id
    QString           MaterielDes;    //材料描述
    QString           Plating;        //镀种
    QString           Friction;       //摩擦系数
    QString           Thickness;      //厚度
    QString           Salt;           //盐度
    QString           ComponentSolid; //组件固号
    QString           ComponentFormat;//组件规格
    QString           Factory;        //分厂名称
    QString           FactoryNumber;  //分厂号
    QString           ProductionLine; //产线名称
    QString           Unit;           //单位
    QString           CustomID;       //客户ID
    QString           CustomName;     //客户姓名
    QString           CustomBatch;    //客户批次
    QString           CustomNote;     //客户备注
    QString           ProduceID;      //生产编号
    QString           SuccessTime;    //出货时间
    QString           ProduceTime;    //生产完成时间
    QString           CreatTime;      //创建时间
    OderFlow          Current;        //当前状态
    QVector<OderFlow> Flow;           //订单流程
    double            OrderNum;       //订单数量
    double            ProduceNum;     //生产完成数量
    double            SuccessNum;     //出库数量
    double            Money;          //价格
    double            TotleMoney;     //总价
    int               PrintNum;       //打印次数
};

struct User {
    QString UID;            //用户id
    QString Name;           //用户姓名
    QString Sex;            //性别
    QString Cell;           //联系方式
    QString CID;            //公司ID
    QString CommpanyName;   //公司名称
    QString Department;     //部门
    QString Author;         //用户权限
    QString InTime;         //入职时间
    QString OutTime;        //离职时间
    QString CreatTime;      //创建时间
    QString Status;         //状态
    QString Account;        //账号
    QString Code;           //密码
    int     Age;            //年龄
    int     Salary;          //薪水

};


//客户
struct Customer{
    QString CID;                    //客户编号
    QString Name;                   //客户公司名称
    QString Icon;                   //客户公司logo
    QString Addr;                   //客户公司地址
    QString Tel;                    //公司电话
    QString ContactName;            //联系人
    QString ContactCell;            //联系人电话
    QString BankName;               //开户行
    QString BankNumber;             //银行卡号
    QString Bankbranch;             //银行支行
    QString CertificatesNum;        //税号
    QString Certificates;           //客户证件（营业执照）
    QString Note;                   //备注
    QString Status;                 //状态 0:正常 1:删除
    QString CreatTime;              //创建时间
};

//供应商
struct Supplier{
    QString SID;                    //供应商编号
    QString Name;                   //供应商姓名
    QString Icon;                   //供应商logo
    QString Addr;                   //供应商地址
    QString Tel;                    //联系电话
    QString ContactName;            //联系人
    QString ContactCell;            //联系人电话
    QString BankName;               //开户行
    QString BankNumber;             //银行卡号
    QString Bankbranch;             //银行支行
    QString CertificatesNum;        //税号
    QString Certificates;           //供应商证件（营业执照）
    QString Note;                   //供应商备注
    QString Goods;                  //供应的商品列表
    QString Status;                 //状态 0:正常 1:删除
    QString CreatTime;              //创建时间
};

//物料
struct Materiel{
    QString MaterID;            //物料编号
    QString MaterDes;           //物料描述
    QString CID;                //客户ID
    QString CustomName;         //客户姓名
    QString Plating;            //镀种
    QString Friction;           //摩擦系数
    QString Thickness;          //厚度
    QString Salt;               //盐度
    QString ComponentSolid;     //组件固号
    QString ComponentFormat;    //组件规格
    QString Factory;            //分厂名称
    QString FactoryNumber;      //分厂号
    QString ProductionLine;     //产线名称
    QString Unit;               //单位
    QString CreatTime;          //创建时间
    double  Money;              //价格
};


struct Goods{
    QString ID;             //商品ID
    QString Name;           //商品名称
    QString Type;           //类别
    QString Unit;           //单位
    QString Format;         //规格
    QString	SID;            //供应商id
    QString	SupplierName;   //供应商名称
    QString	CreatTime;      //创建时间
    double   Num;            //库存
};


struct GoodsOutRecord{
    QString   OutID;          //进出凭证id
    QString   Factory;        //分厂名称
    QString   Department;     //部门名称
    QString   CreatDate;      //领用时间
    QString   GoodsID;        //商品ID
    QString   GoodsName;      //商品名称
    QString   Type;           //类别
    QString   StrorageName;   //仓库名称
    double    Nums;           //数量
    QString   Unit;           //单位
    QString   Note;           //备注
    QString   UserName;       //领用人姓名
    QString   UserID;         //领用人工号
};









#endif // SERVICE_GLOBAL_H
