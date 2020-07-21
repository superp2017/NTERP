package main

import (
	"JGo/JExit"
	"JGo/JHttp"
)

func exit() int {
	JHttp.Close()
	return 0
}

func initRoute() {
	JHttp.WhiteHttp("/login", Login)                             //登录
	JHttp.WhiteHttp("/neworder", NewOrder)                       //新建订单
	JHttp.WhiteHttp("/modorder", ModOrder)                       //修改订单
	JHttp.WhiteHttp("/delorder", DelOrder)                       //删除订单
	JHttp.WhiteHttp("/modorderprice", ModOrderPrice)             //修改订单价格
	JHttp.WhiteHttp("/updateprintnum", UpdatePrintNum)           //修改订单打印次数
	JHttp.WhiteHttp("/getprintnumber", GetPrintNum)              //获取打印编号
	JHttp.WhiteHttp("/cancleorder", CancelOrder)                 //取消订单
	JHttp.WhiteHttp("/produceorder", PorduceOrder)               //订单生产
	JHttp.WhiteHttp("/successorder", SuccessOrder)               //订单完成
	JHttp.WhiteHttp("/getglobalorders", GetGlobalOrders)         //获取所有订单
	JHttp.WhiteHttp("/newmaterial", NewMaterial)                 //创建一个物料
	JHttp.WhiteHttp("/modMaterial", ModMaterial)                 //修改物料
	JHttp.WhiteHttp("/querymaterial", QueryMaterial)             //查询物料
	JHttp.WhiteHttp("/delmaterial", DelMaterial)                 //删除一个物料
	JHttp.WhiteHttp("/newcustomer", NewCustomer)                 //新增一个客户
	JHttp.WhiteHttp("/modcustomer", ModCustomer)                 //修改客户
	JHttp.WhiteHttp("/updowncustomer", UpDownCustomer)           //解约或合作客户
	JHttp.WhiteHttp("/delcustomer", DelCustomer)                 //删除一个客户
	JHttp.WhiteHttp("/getallcustomers", GetAllCustomer)          //获取所有客户
	JHttp.WhiteHttp("/newsupplier", NewSupplier)                 //新增一个供应商
	JHttp.WhiteHttp("/modsupplier", ModSupplier)                 //修改供应商
	JHttp.WhiteHttp("/updownsupplier", UpDownSupplier)           //解约或合作客户
	JHttp.WhiteHttp("/delsupplier", DelSupplier)                 //删除一个供应商
	JHttp.WhiteHttp("/getallsuppliers", GetAllSupplier)          //获取所有供应商
	JHttp.WhiteHttp("/newemployee", NewEmployee)                 //新建一个员工
	JHttp.WhiteHttp("/modemployee", ModEmployee)                 //修改员工信息
	JHttp.WhiteHttp("/outemployee", OutEmployee)                 //员工离职
	JHttp.WhiteHttp("/delemployee", DelEmployee)                 //删除一个员工
	JHttp.WhiteHttp("/getallemployeess", GetAllEmployee)         //获取所有的员工信息
	JHttp.WhiteHttp("/newunit", NewUnit)                         //新建一个单位
	JHttp.WhiteHttp("/delunit", DelUnit)                         //删除一个单位
	JHttp.WhiteHttp("/getallunits", GetAllUnit)                  //获取所有的单位
	JHttp.WhiteHttp("/getallmaterials", GetAllMaterial)          //获取所有的物料
	JHttp.WhiteHttp("/getglobalgoods", GetGlobalGoods)           //获取仓库商品
	JHttp.WhiteHttp("/newgoods", NewGoods)                       //新建一个商品
	JHttp.WhiteHttp("/querygoods", QueryGoods)                   //获取商品信息
	JHttp.WhiteHttp("/delgoods", DelGoods)                       //删除一个商品
	JHttp.WhiteHttp("/addgoodsnum", AddGoodsNum)                 //商品入库
	JHttp.WhiteHttp("/newstorageoutrecord", NewOutRecord)        //新增一个仓库出库记录
	JHttp.WhiteHttp("/getallstorageoutrecords", GetAllOutRecord) //获取所有的仓库出库记录
	JHttp.WhiteHttp("/modifygoods", ModGoods)                    //修改商品
	JHttp.WhiteHttp("/removegoodstype", RemoveGoodsType)         //删除商品的分类
	JHttp.WhiteHttp("/newgoodstype", AddGoodsType)               //新建商品的分类
	JHttp.WhiteHttp("/getallgoodstype", GetAllGoodsType)         //获取所有商品分类
	JHttp.WhiteHttp("/newdepartment", NewDepartment)             //新建一个部门
	JHttp.WhiteHttp("/removedepartment", RemoveDepartment)       //删除一个部门
	JHttp.WhiteHttp("/getalldepartment", GetAllDepartment)       //获取所有的部门
	JHttp.WhiteHttp("/newplating", NewPlating)                   //新建一个镀种
	JHttp.WhiteHttp("/delplating", DelPlating)                   //删除一个镀种
	JHttp.WhiteHttp("/getallplating", GetAllPlating)             //获取所有的镀种
	JHttp.WhiteHttp("/heartbeat", HeartBeat)                     //客户端心跳
	JHttp.WhiteHttp("/getversion", GetVertion)                   //获取版本
	JHttp.WhiteHttp("/setversion", SetVersion)                   //设置版本
	JHttp.WhiteHttp("/searchorder", SearchOrder)                 //订单搜索
	JHttp.WhiteHttp("/searchoutorder", SearchOutOrder)           //出库的订单搜索
	JHttp.WhiteHttp("/updateall", updateAll)                     //更新所有时间戳
	JHttp.WhiteHttp("/gettodayorder", GetTodayOrder)             //获取当日订单
	JHttp.WhiteHttp("/getonedayorder", GetOneDayOrder)           //获取某一日的订单

}

func main() {
	JExit.RegisterExitCb(exit)
	initRoute()
	StartTimer()
	JHttp.Run()
}
