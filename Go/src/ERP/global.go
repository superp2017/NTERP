package main

//表
const (
	Hash_Order         = "Order"
	Hash_Account       = "Account"
	Hash_Employee      = "Employee"
	Hash_Material      = "Material"
	Hash_Customer      = "Customer"
	Hash_Supplier      = "Supplier"
	Hash_CustomerOrder = "CustomerOrder"
	Hash_StorageOutIn  = "StorageOutIn"
	Key_GlobalOrder    = "GlobalOrder"
	Key_Unit           = "Unit"
	Key_Plating        = "Plating"
	Hash_Goods         = "Goods"
	Hash_SupplierGoods = "SupplierGoods"
	Key_GoodType       = "GoodsType"
	Key_Department     = "Department"
	Key_LastOrderDate  = "LastOrderDate"
)

//ider
const (
	Ider_Order    = "Ider_Order"
	Ider_Employee = "Ider_Employee"
	Ider_Material = "Ider_Material"
	Ider_Customer = "Ider_Customer"
	Ider_Supplier = "Ider_Supplier"
	Ider_Goods    = "Ider_Goods"
)

//order 状态
const (
	Status_New          string = "Status_New"         //新建订单
	Status_Part_Produce string = "Status_PartProduce" //部分生产完成
	Status_Produce      string = "Status_Produce"     //全部生产完成
	Status_Part_Success string = "Status_PartSuccess" //部分出库
	Status_Success      string = "Status_Success"     //订单完成(出库)
	Status_Part_Part    string = "Status_Part_Part"   // 订单部分生产部分出库
	Status_Cancle       string = "Status_Cancle"      //订单取消
	Status_Del          string = "Status_Del"         //订单删除
	Status_All          string = "Status_All"         //所有订单
)
