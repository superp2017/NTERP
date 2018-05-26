package main

import (
	"JsGo/JsHttp"
	"JsGo/JsLogger"
	"JsGo/JsStore/JsRedis"
	"fmt"
	"time"
)

type OderFlow struct {
	UserName   string //用户姓名
	OpreatTime string //操作时间
	Action     string //动作
	Status     string //状态
}

type Order struct {
	UID         string     //用户id
	UserName    string     //用户姓名
	OrderID     string     //订单id
	OrderType   string     //订单类型（普通订单、批量订单、试样订单）
	MaterielID  string     //材料id
	MaterielDes string     //材料描述
	Unit        string     //单位
	CustomID    string     //客户ID
	CustomName  string     //客户姓名
	CustomBatch string     //客户批次
	CustomNote  string     //客户备注
	ProduceID   string     //生产编号
	SuccessTime string     //出货时间
	CreatTime   string     //创建时间
	Current     string     //当前状态
	Flow        []OderFlow //订单流程
	OrderNum    int        //订单数量
	Money       int        //价格
}

//新建订单
func NewOrder(session JsHttp.Session) {
	st := &Order{}
	if err := session.GetPara(st); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.UserName == "" || st.UID == "" {
		str := fmt.Sprintf("UserName = %s,UID = %s\n", st.UserName, st.UID)
		JsLogger.Error(str)
		session.Forward("1", str, nil)
		return
	}
	if st.MaterielDes == "" {
		JsLogger.Error("MaterielDes is empty\n!")
		session.Forward("1", "MaterielDes is empty\n!", nil)
		return
	}
	/////////////创建物料/////////////////////
	ma, err := newMaterial(st.MaterielDes, st.Unit, st.CustomID, st.CustomName, st.OrderNum)
	if err != nil {
		JsLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	st.MaterielID = ma.MaterID
	id := CurDateEx() + getOrderID()
	if st.OrderType == "0" {
		id = "0" + id
	} else if st.OrderType == "1" {
		id = "1" + id
	} else if st.OrderType == "2" {
		id = "2" + id
	} else {
		id = "0" + id
	}
	st.OrderID = id
	st.CreatTime = CurTime()
	////////////////添加状态///////////////////////////////
	appendStatus(st, st.UserName, st.CreatTime, "创建订单", Status_New)
	if err := JsRedis.Redis_hset(Hash_Order, st.OrderID, st); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	session.Forward("0", "success", st)
}

//修改订单
func ModOrder(session JsHttp.Session) {
	type Para struct {
		OrderID     string //订单id
		MaterielID  string //材料id
		MaterielDes string //材料描述
		OrderNum    int    //订单数量
		Unit        string //单位
		CustomID    string //客户ID
		CustomName  string //客户姓名
		CustomBatch string //客户批次
		CustomNote  string //客户备注
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.OrderID == "" {
		session.Forward("1", "ModOrder faild,OrderID is empty\n", nil)
		return
	}
	if st.MaterielID == "" || st.MaterielDes == "" {
		str := fmt.Sprintf("ModOrder faild,MaterielID =%s,MaterielDes=%s\n", st.MaterielID, st.MaterielDes)
		JsLogger.Error(str)
		session.Forward("1", str, nil)
		return
	}
	go modMaterial(st.MaterielID, st.MaterielDes, st.CustomID, st.CustomName)
	data := &Order{}
	if err := JsRedis.Redis_hget(Hash_Order, st.OrderID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}

	data.MaterielID  = st.MaterielID
	data.MaterielDes = st.MaterielDes
	data.OrderNum 	 = st.OrderNum
	data.Unit 		 = st.Unit
	data.CustomID 	 = st.CustomID
	data.CustomName  = st.CustomName
	data.CustomBatch = st.CustomBatch
	data.CustomNote  = st.CustomNote
	////////////////添加状态///////////////////////////////
	appendStatus(data, data.UserName, CurTime(), "修改订单", data.Current)

	if err := JsRedis.Redis_hset(Hash_Order, st.OrderID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	session.Forward("0", "success", data)
}

//修改订单价格
func ModOrderPrice(session JsHttp.Session) {
	type Para struct {
		OrderID string //订单id
		Money   int    //价格
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.OrderID == "" || st.Money <= 0 {
		str := fmt.Sprintf("ModOrderPrice faild,OrderID = %s,Money = %d\n", st.OrderID, st.Money)
		JsLogger.Error(str)
		session.Forward("1", str, nil)
		return
	}
	data := &Order{}
	if err := JsRedis.Redis_hget(Hash_Order, st.OrderID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	data.Money = st.Money

	////////////////添加状态///////////////////////////////
	appendStatus(data, data.UserName, CurTime(), "修改订单价格", data.Current)

	if err := JsRedis.Redis_hset(Hash_Order, st.OrderID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	session.Forward("0", "success", data)
}

//取消订单
func CancelOrder(session JsHttp.Session) {
	type Para struct {
		OrderID string //订单id
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.OrderID == "" {
		session.Forward("1", "CancelOrder failed,OrderID is empty\n", nil)
		return
	}
	data := &Order{}
	if err := JsRedis.Redis_hget(Hash_Order, st.OrderID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	////////////////添加状态///////////////////////////////
	appendStatus(data, data.UserName, CurTime(), "订单取消", Status_Cancle)

	if err := JsRedis.Redis_hset(Hash_Order, st.OrderID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	session.Forward("0", "success", data)
}

//删除订单
func DelOrder(session JsHttp.Session) {
	type Para struct {
		OrderID string //订单id
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.OrderID == "" {
		session.Forward("1", "DelOrder failed,OrderID is empty\n", nil)
		return
	}
	data := &Order{}
	if err := JsRedis.Redis_hget(Hash_Order, st.OrderID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}

	if err := JsRedis.Redis_hdel(Hash_Order, st.OrderID); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}

	go delMaterial(data.MaterielID)

	session.Forward("0", "success", data)
}

//订单进入生产
func PorduceOrder(session JsHttp.Session) {
	type Para struct {
		OrderID string //订单id
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.OrderID == "" {
		session.Forward("1", "PorduceOrder failed,OrderID is empty\n", nil)
		return
	}
	data := &Order{}
	if err := JsRedis.Redis_hget(Hash_Order, st.OrderID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}

	data.ProduceID = time.Unix(time.Now().Unix(), 0).Format("20060102150405")
	////////////////添加状态///////////////////////////////
	appendStatus(data, data.UserName, CurTime(), "订单进入生产", Status_Produce)

	if err := JsRedis.Redis_hset(Hash_Order, st.OrderID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	session.Forward("0", "success", data)
}

//订单完成
func SuccessOrder(session JsHttp.Session) {
	type Para struct {
		OrderID string //订单id
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.OrderID == "" {
		session.Forward("1", "SuccessOrder failed,OrderID is empty\n", nil)
		return
	}
	data := &Order{}
	if err := JsRedis.Redis_hget(Hash_Order, st.OrderID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	go successMaterial(data.MaterielID)
	////////////////添加状态///////////////////////////////
	appendStatus(data, data.UserName, CurTime(), "订单完成", Status_Success)

	if err := JsRedis.Redis_hset(Hash_Order, st.OrderID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	session.Forward("0", "success", data)
}

//获取所有订单列表
func GetGlobalOrders(session JsHttp.Session) {
	list,err:= JsRedis.Redis_hkeys(Hash_Order)
	if err!=nil{
		session.Forward("1",err.Error(),nil)
		return
	}
	data :=[]*Order{}
	for _,v:=range list{
		d := &Order{}
		if err := JsRedis.Redis_hget(Hash_Order, v, data); err == nil {
			data = append(data,d)
		}
	}
	session.Forward("0","success",data)
}


//添加状态
func appendStatus(order *Order, name, time, action, status string) {
	flow := OderFlow{
		UserName:   name,
		OpreatTime: time,
		Action:     action,
		Status:     status,
	}
	order.Current = status
	order.Flow = append(order.Flow, flow)
}