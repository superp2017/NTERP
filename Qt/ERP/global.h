#ifndef STRUCT_H
#define STRUCT_H

#include <QString>
#include <QVector>
#include "service_global.h"

#pragma execution_character_set("utf-8")




const QString Status_New     = "Status_New";        //新建订单
const QString Status_Success = "Status_Success";    //订单完成(出库)
const QString Status_Cancle  = "Status_Cancle";     //订单取消
const QString Status_All     = "Status_All";        //所有订单

const QString ItemNewCustom("新增客户...");
const QString ItemNewUnit("新增单位...");

const QString ItemNewDepartment("部门设置...");
const QString ItemNewAuthor("权限设置...");


#endif // STRUCT_H
