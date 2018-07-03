#ifndef STRUCT_H
#define STRUCT_H

#include <QString>
#include <QVector>
#include "service_global.h"
#include <QDebug>

#pragma execution_character_set("utf-8")



const QString Status_New     = "Status_New";                //新建订单
const QString Status_PartSuccess = "Status_PartSuccess";    //订单部分出库
const QString Status_Success = "Status_Success";            //订单完成(出库)
const QString Status_Produce = "Status_Produce";            //全部生产完成
const QString Status_PartProduce = "Status_PartProduce";    //部分完成声场
const QString Status_Cancle  = "Status_Cancle";             //订单取消
const QString Status_All     = "Status_All";                //所有订单



const QString ItemNewCustom("新增客户...");

const QString ItemNewUnit("新增单位...");

const QString ItemNewAuthor("权限设置...");

const QString ItemNewType("新增分类...");


const QString ItemAllType("全部分类");




struct SysSetting{
    QString Host_name;
    QString Host_port;
    QString Account;
    QStringList MaterielColor;
    QStringList MaterielType;
    bool isExportOpen;
};



#endif // STRUCT_H
