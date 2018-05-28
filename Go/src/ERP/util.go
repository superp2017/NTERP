package main

import (
	"JsGo/JsStore/JsRedis"
	"JsGo/JsUuid"

	"strconv"
	"time"
)

//const Hash_Order = "Order"
//const Hash_Account = "Account"
//const Hash_Employee = "Employee"
//const Hash_Material = "Material"
//const Hash_Customer = "Customer"
//const Hash_Supplier = "Supplier"

//const Key_Unit = "Unit"
//
//const Ider_Order ="Ider_Order"
//const Ider_Employee ="Ider_Employee"
//const Ider_Material ="Ider_Material"
//const Ider_Customer ="Ider_Customer"
//const Ider_Supplier ="Ider_Supplier"

var gOderSeed int
var gEmployeeSeed int
var gMaterialSeed int
var gCustomerSeed int
var gSupplierSeed int

func init() {
	if err := JsRedis.Redis_get(Ider_Order, &gOderSeed); err != nil {
		log.Fatalln("Redis_get gOderSeed failed\n")
	}
	if err := JsRedis.Redis_get(Ider_Employee, &gEmployeeSeed); err != nil {
		log.Fatalln("Redis_get gOderSeed failed\n")
	}
	if err := JsRedis.Redis_get(Ider_Material, &gMaterialSeed); err != nil {
		log.Fatalln("Redis_get gOderSeed failed\n")
	}
	if err := JsRedis.Redis_get(Ider_Customer, &gCustomerSeed); err != nil {
		log.Fatalln("Redis_get gOderSeed failed\n")
	}
	if err := JsRedis.Redis_get(Ider_Supplier, &gSupplierSeed); err != nil {
		log.Fatalln("Redis_get gOderSeed failed\n")
	}
}

//获取订单id
func getOrderID() string {
	gOderSeed++
	go JsRedis.Redis_set(Ider_Order, &gOderSeed)
	return strconv.Itoa(gOderSeed)
}

//获取员工id
func getEmployeeID() string {
	gEmployeeSeed++
	go JsRedis.Redis_set(Ider_Employee, &gEmployeeSeed)
	return "E" + strconv.Itoa(gEmployeeSeed)
}

//获取材料id
func getMaterialID() string {
	gMaterialSeed++
	go JsRedis.Redis_set(Ider_Material, &gMaterialSeed)
	return "M" + strconv.Itoa(gMaterialSeed)
}

//获取客户id
func getCustomerID() string {
	gCustomerSeed++
	go JsRedis.Redis_set(Ider_Customer, &gCustomerSeed)
	return "C" + strconv.Itoa(gCustomerSeed)
}

//获取供应商id
func getSupplierID() string {
	gSupplierSeed++
	go JsRedis.Redis_set(Ider_Supplier, &gSupplierSeed)
	return "S" + strconv.Itoa(gMaterialSeed)
}

//返回当前时间：例如 2017-02-17 16:33
func CurTime() string {
	return time.Unix(time.Now().Unix(), 0).Format("2006-01-02 15:04:05")
}

//返回当前日期：例如：20170217
func CurDateEx() string {
	return time.Unix(time.Now().Unix(), 0).Format("20060102")
}

//返回当前日期：例如：2017-02-17
func CurDate() string {
	return time.Unix(time.Now().Unix(), 0).Format("2006-01-02")
}

///当前时间的时间戳
func CurStamp() int64 {
	return time.Now().Unix()
}

func IDer(db string) string {
	id := JsUuid.NewV4().String()
	for b, e := JsRedis.Redis_hexists(db, id); b; {
		id = JsUuid.NewV4().String()
		if e != nil {
			return id
		}
	}
	return id
}
