package main

import (
	"JsGo/JsExit"
	"JsGo/JsHttp"
)

func exit() int {
	JsHttp.Close()
	return 0
}

func initRoute() {
	JsHttp.WhiteHttp("/login", Login)						//登录
	JsHttp.WhiteHttp("/neworder", NewOrder)                 //新建订单
	JsHttp.WhiteHttp("/modorder", ModOrder)                 //修改订单
	JsHttp.WhiteHttp("/delorder", DelOrder)                 //删除订单
	JsHttp.WhiteHttp("/modorderprice", ModOrderPrice)       //修改订单价格
	JsHttp.WhiteHttp("/cancleorder", CancelOrder)           //取消订单
	JsHttp.WhiteHttp("/produceorder", PorduceOrder)         //订单生产
	JsHttp.WhiteHttp("/successorder", SuccessOrder)         //订单完成
	JsHttp.WhiteHttp("/getglobalorders", GetGlobalOrders)   //获取所有订单
	JsHttp.WhiteHttp("/newcustomer", NewCustomer)           //新增一个客户
	JsHttp.WhiteHttp("/modcustomer", ModCustomer)           //修改客户
	JsHttp.WhiteHttp("/updowncustomer", UpDownCustomer)     //解约或合作客户
	JsHttp.WhiteHttp("/delcustomer", DelCustomer)           //删除一个客户
	JsHttp.WhiteHttp("/newsupplier", NewSupplier)           //新增一个供应商
	JsHttp.WhiteHttp("/modsupplier", ModSupplier)           //修改供应商
	JsHttp.WhiteHttp("/updownsupplier", UpDownSupplier)     //解约或合作客户
	JsHttp.WhiteHttp("/delsupplier", DelSupplier)           //删除一个供应商
	JsHttp.WhiteHttp("/newemployee", NewEmployee)           //新建一个员工
	JsHttp.WhiteHttp("/modemployee", ModEmployee)           //修改员工信息
	JsHttp.WhiteHttp("/delemployee", DelEmployee)           //删除一个员工
	JsHttp.WhiteHttp("/getallemployeess", GetAllEmployeess) //获取所有的员工信息
	JsHttp.WhiteHttp("/newunit", NewUnit)                   //新建一个单位
	JsHttp.WhiteHttp("/delunit", DelUnit)                   //删除一个单位
	JsHttp.WhiteHttp("/getallunts", GetAllUnit)             //获取所有的单位

}

func main() {
	JsExit.RegisterExitCb(exit)
	initRoute()
	JsHttp.Run()
}
