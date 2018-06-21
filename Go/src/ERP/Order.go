package main

import (
	"JGo/JHttp"
	"JGo/JLogger"
	"JGo/JStore/JRedis"
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
	UID           string     //用户id
	UserName      string     //用户姓名
	OrderID       string     //订单id
	OrderType     string     //订单类型（普通订单、试样订单、返工订单）
	Factory       string     //分厂名称
	FactoryNumber string     //分厂号
	MaterielID    string     //材料id
	MaterielDes   string     //材料描述
	Unit          string     //单位
	CustomID      string     //客户ID
	CustomName    string     //客户姓名
	CustomBatch   string     //客户批次
	CustomNote    string     //客户备注
	ProduceID     string     //生产编号
	SuccessTime   string     //出货时间
	CreatTime     string     //创建时间
	Current       OderFlow   //当前状态
	Flow          []OderFlow //订单流程
	OrderNum      int        //订单数量
	Money         int        //价格
}

//新建订单
func NewOrder(session *JHttp.Session) {
	st := &Order{}
	if err := session.GetPara(st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.UserName == "" || st.UID == "" {
		str := fmt.Sprintf("NewOrder failed,UserName = %s,UID = %s\n", st.UserName, st.UID)
		JLogger.Error(str)
		session.Forward("1", str, nil)
		return
	}
	if st.MaterielDes == "" {
		JLogger.Error("NewOrder MaterielDes is empty\n!")
		session.Forward("1", "NewOrder MaterielDes is empty\n!", nil)
		return
	}
	if st.Factory == "" || st.FactoryNumber == "" {
		str := fmt.Sprintf("NewOrder failed,Factory=%s,FactoryNumber=%s\n", st.Factory, st.FactoryNumber)
		JLogger.Error(str)
		session.Forward("1", str, nil)
		return
	}
	/////////////创建物料/////////////////////
	ma, err := newMaterial(st.MaterielDes, st.Unit, st.CustomID, st.CustomName, st.OrderNum)
	if err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	st.MaterielID = ma.MaterID
	id := CurDateEx() + getOrderID()
	id = st.FactoryNumber + id
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
	if err := JRedis.Redis_hset(Hash_Order, st.OrderID, st); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.CustomID != "" && st.OrderID != "" {
		go appendCustomerOrderID(st.CustomID, st.OrderID)
	}
	session.Forward("0", "success", st)
}

//修改订单
func ModOrder(session *JHttp.Session) {
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
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.OrderID == "" {
		session.Forward("1", "ModOrder faild,OrderID is empty\n", nil)
		return
	}
	if st.MaterielID == "" || st.MaterielDes == "" {
		str := fmt.Sprintf("ModOrder faild,MaterielID =%s,MaterielDes=%s\n", st.MaterielID, st.MaterielDes)
		JLogger.Error(str)
		session.Forward("1", str, nil)
		return
	}

	data := &Order{}
	if err := JRedis.Redis_hget(Hash_Order, st.OrderID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	if data.Current.Status == Status_Cancle || data.Current.Status == Status_Success {
		str := fmt.Sprintf("ModOrder faild,Current.Status=%s not support modify\n", data.Current.Status)
		JLogger.Error(str)
		session.Forward("1", str, nil)
		return
	}
	data.MaterielID = st.MaterielID
	data.MaterielDes = st.MaterielDes
	data.OrderNum = st.OrderNum
	data.Unit = st.Unit
	data.CustomID = st.CustomID
	data.CustomName = st.CustomName
	data.CustomBatch = st.CustomBatch
	data.CustomNote = st.CustomNote
	////////////////添加状态///////////////////////////////
	appendStatus(data, data.UserName, CurTime(), "修改订单", data.Current.Status)

	if err := JRedis.Redis_hset(Hash_Order, st.OrderID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	go modMaterial(st.MaterielID, st.MaterielDes, st.CustomID, st.CustomName)
	session.Forward("0", "success", data)
}

//修改订单价格
func ModOrderPrice(session *JHttp.Session) {
	type Para struct {
		OrderID string //订单id
		Money   int    //价格
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.OrderID == "" || st.Money <= 0 {
		str := fmt.Sprintf("ModOrderPrice faild,OrderID = %s,Money = %d\n", st.OrderID, st.Money)
		JLogger.Error(str)
		session.Forward("1", str, nil)
		return
	}
	data := &Order{}
	if err := JRedis.Redis_hget(Hash_Order, st.OrderID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}

	if data.Current.Status == Status_Cancle || data.Current.Status == Status_Success {
		str := fmt.Sprintf("ModOrderPrice faild,Current.Status=%s not support modify price \n", data.Current.Status)
		JLogger.Error(str)
		session.Forward("1", str, nil)
		return
	}

	data.Money = st.Money

	////////////////添加状态///////////////////////////////
	appendStatus(data, data.UserName, CurTime(), "修改订单价格", data.Current.Status)

	if err := JRedis.Redis_hset(Hash_Order, st.OrderID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	session.Forward("0", "success", data)
}

//取消订单
func CancelOrder(session *JHttp.Session) {
	type Para struct {
		OrderID string //订单id
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.OrderID == "" {
		session.Forward("1", "CancelOrder failed,OrderID is empty\n", nil)
		return
	}
	data := &Order{}
	if err := JRedis.Redis_hget(Hash_Order, st.OrderID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}

	if data.Current.Status == Status_Cancle || data.Current.Status == Status_Success {
		str := fmt.Sprintf("CancelOrder faild,Current.Status=%s not support cancle \n", data.Current.Status)
		JLogger.Error(str)
		session.Forward("1", str, nil)
		return
	}

	////////////////添加状态///////////////////////////////
	appendStatus(data, data.UserName, CurTime(), "订单取消", Status_Cancle)

	if err := JRedis.Redis_hset(Hash_Order, st.OrderID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}

	go cancleMaterial(data.MaterielID)
	if data.CustomID != "" && data.OrderID != "" {
		go removefromCustomerOrderID(data.CustomID, data.OrderID)
	}
	session.Forward("0", "success", data)
}

//删除订单
func DelOrder(session *JHttp.Session) {
	type Para struct {
		OrderID string //订单id
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.OrderID == "" {
		session.Forward("1", "DelOrder failed,OrderID is empty\n", nil)
		return
	}
	data := &Order{}
	if err := JRedis.Redis_hget(Hash_Order, st.OrderID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	if data.Current.Status == Status_Produce {
		str := fmt.Sprintf("DelOrder faild,order is on produce  \n", data.Current.Status)
		JLogger.Error(str)
		session.Forward("1", str, nil)
		return
	}
	if err := JRedis.Redis_hdel(Hash_Order, st.OrderID); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	if data.MaterielID != "" {
		go delMaterial(data.MaterielID)
	}
	if data.CustomID != "" && data.OrderID != "" {
		go removefromCustomerOrderID(data.CustomID, data.OrderID)
	}
	session.Forward("0", "success", data)
}

//订单进入生产
func PorduceOrder(session *JHttp.Session) {
	type Para struct {
		OrderID string //订单id
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.OrderID == "" {
		session.Forward("1", "PorduceOrder failed,OrderID is empty\n", nil)
		return
	}
	data := &Order{}
	if err := JRedis.Redis_hget(Hash_Order, st.OrderID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	if data.Current.Status == Status_Cancle || data.Current.Status == Status_Success {
		str := fmt.Sprintf("PorduceOrder faild,Current.Status=%s not support produce \n", data.Current.Status)
		JLogger.Error(str)
		session.Forward("1", str, nil)
		return
	}
	data.ProduceID = time.Unix(time.Now().Unix(), 0).Format("20060102150405")
	////////////////添加状态///////////////////////////////
	appendStatus(data, data.UserName, CurTime(), "订单进入生产", Status_Produce)

	if err := JRedis.Redis_hset(Hash_Order, st.OrderID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	session.Forward("0", "success", data)
}

//订单完成
func SuccessOrder(session *JHttp.Session) {
	type Para struct {
		OrderID string //订单id
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.OrderID == "" {
		session.Forward("1", "SuccessOrder failed,OrderID is empty\n", nil)
		return
	}
	data := &Order{}
	if err := JRedis.Redis_hget(Hash_Order, st.OrderID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	if data.Current.Status == Status_Cancle || data.Current.Status == Status_Success {
		str := fmt.Sprintf("SuccessOrder faild,Current.Status=%s not support finish \n", data.Current.Status)
		JLogger.Error(str)
		session.Forward("1", str, nil)
		return
	}

	go successMaterial(data.MaterielID)
	////////////////添加状态///////////////////////////////
	appendStatus(data, data.UserName, CurTime(), "订单完成", Status_Success)

	if err := JRedis.Redis_hset(Hash_Order, st.OrderID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	session.Forward("0", "success", data)
}

//获取所有订单列表
func GetGlobalOrders(session *JHttp.Session) {
	list, err := JRedis.Redis_hkeys(Hash_Order)
	if err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	data := []*Order{}
	for _, v := range list {
		d := &Order{}
		if err := JRedis.Redis_hget(Hash_Order, v, d); err == nil {
			data = append(data, d)
		}
	}
	session.Forward("0", "success", data)
}

//添加状态
func appendStatus(order *Order, name, time, action, status string) {
	flow := OderFlow{
		UserName:   name,
		OpreatTime: time,
		Action:     action,
		Status:     status,
	}
	order.Current = flow
	order.Flow = append(order.Flow, flow)
}
