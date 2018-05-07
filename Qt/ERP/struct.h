#ifndef STRUCT_H
#define STRUCT_H

#include <QString>

#pragma execution_character_set("utf-8")

struct User {
    QString UID;
    QString Name;
    QString Author;
};

struct Order{
    QString UID;            //用户id
    QString OrderID;        //订单id
    QString MaterielID;     //材料id
    QString MaterielDes;    //材料描述
    QString OrderNum;       //订单数量
    QString Unit;           //单位
    QString CustomBatch;    //客户批次
    QString ProduceID;      //生产编号
    QString status;         //状态
    int  Money;             //价格
    QString SuccessTime;    //创建时间
    QString CreatTime;      //创建时间
};

#endif // STRUCT_H
