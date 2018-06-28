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
	Status_New     string = "Status_New"     //新建订单
	Status_Produce string = "Status_Produce" //新建订单
	Status_Success string = "Status_Success" //订单完成(出库)
	Status_Cancle  string = "Status_Cancle"  //订单取消
	Status_All     string = "Status_All"     //所有订单
)
