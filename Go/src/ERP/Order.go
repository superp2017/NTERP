package main

import (
	"JGo/JHttp"
	"JGo/JLogger"
	"JGo/JStore/JRedis"
	"fmt"
)

type OderFlow struct {
	UserName   string //用户姓名
	OpreatTime string //操作时间
	Action     string //动作
	Status     string //状态
}

type Order struct {
	UID             string     //用户id
	UserName        string     //用户姓名
	OrderID         string     //订单id
	OrderType       string     //订单类型（普通订单、试样订单、返工订单）
	Factory         string     //分厂名称
	FactoryNumber   string     //分厂号
	ProductionLine  string     //产线名称
	MaterielID      string     //材料id
	MaterielDes     string     //材料描述
	Plating         string     //镀种
	Friction        string     //摩擦系数
	Thickness       string     //厚度
	Salt            string     //盐度
	ComponentSolid  string     //零件固号
	ComponentFormat string     //零件规格
	Unit            string     //单位
	CustomID        string     //客户ID
	CustomName      string     //客户姓名
	CustomBatch     string     //客户批次
	CustomNote      string     //客户备注
	ProduceID       string     //生产编号
	CreatTime       string     //创建时间
	ProduceTime     string     //出货时间
	SuccessTime     string     //完成时间
	Current         OderFlow   //当前状态
	Flow            []OderFlow //订单流程
	OrderNum        int        //订单数量
	ProduceNum      int        //生产完成数量
	SuccessNum      int        //出库数量
	TotleMoney      int        //总价
	Money           int        //单价
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
	index := getOrderID()
	curMon := CurMonth()
	date := getLastOrderDate()
	if date != "" && curMon != date {
		index = resetOrderID()
	}
	id := CurDateEx() + index
	id = st.FactoryNumber + id
	if st.OrderType == "1" {
		id = "1" + id
	} else if st.OrderType == "2" {
		id = "2" + id
	} else if st.OrderType == "3" {
		id = "3" + id
	} else {
		id = "1" + id
	}
	st.OrderID = id
	st.CreatTime = CurTime()
	st.TotleMoney = st.OrderNum * st.Money
	go setLastOrderDate(curMon)
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

////修改订单
//func ModOrder(session *JHttp.Session) {
//	type Para struct {
//		OrderID         string //订单id
//		MaterielID      string //材料id
//		MaterielDes     string //材料描述
//		Plating         string //镀种
//		Friction        string //摩擦系数
//		Thickness       string //厚度
//		Salt            string //盐度
//		ComponentSolid  string //组件固号
//		ComponentFormat string //组件规格
//		OrderNum        int    //订单数量
//		Unit            string //单位
//		CustomID        string //客户ID
//		CustomName      string //客户姓名
//		CustomBatch     string //客户批次
//		CustomNote      string //客户备注
//		ProductionLine  string //产线
//	}
//	st := &Para{}
//	if err := session.GetPara(st); err != nil {
//		JLogger.Error(err.Error())
//		session.Forward("1", err.Error(), nil)
//		return
//	}
//	if st.OrderID == "" {
//		session.Forward("1", "ModOrder faild,OrderID is empty\n", nil)
//		return
//	}
//	if st.MaterielID == "" || st.MaterielDes == "" {
//		str := fmt.Sprintf("ModOrder faild,MaterielID =%s,MaterielDes=%s\n", st.MaterielID, st.MaterielDes)
//		JLogger.Error(str)
//		session.Forward("1", str, nil)
//		return
//	}
//
//	data := &Order{}
//	if err := JRedis.Redis_hget(Hash_Order, st.OrderID, data); err != nil {
//		session.Forward("1", err.Error(), nil)
//		return
//	}
//	if data.Current.Status == Status_Cancle || data.Current.Status == Status_Success {
//		str := fmt.Sprintf("ModOrder faild,Current.Status=%s not support modify\n", data.Current.Status)
//		JLogger.Error(str)
//		session.Forward("1", str, nil)
//		return
//	}
//	data.MaterielID = st.MaterielID
//	data.MaterielDes = st.MaterielDes
//	data.Plating = st.Plating
//	data.Friction = st.Friction
//	data.Thickness = st.Thickness
//	data.Salt = st.Salt
//	data.ComponentSolid = st.ComponentSolid
//	data.ComponentFormat = st.ComponentFormat
//	data.OrderNum = st.OrderNum
//	data.Unit = st.Unit
//	data.CustomID = st.CustomID
//	data.CustomName = st.CustomName
//	data.CustomBatch = st.CustomBatch
//	data.CustomNote = st.CustomNote
//
//	data.TotleMoney = data.OrderNum * data.Money / 100
//
//	////////////////添加状态///////////////////////////////
//	appendStatus(data, data.UserName, CurTime(), "修改订单", getStatus(data.OrderNum, data.ProduceNum, data.SuccessNum))
//
//	if err := JRedis.Redis_hset(Hash_Order, st.OrderID, data); err != nil {
//		session.Forward("1", err.Error(), nil)
//		return
//	}
//	//go modMaterial(st.MaterielID, st.MaterielDes, st.Plating, st.Friction,
//	//	st.Thickness, st.Salt, st.ComponentSolid, st.ComponentFormat,
//	//	st.CustomID, st.CustomName, st.ProductionLine, st.Unit, st.OrderNum, data.Money)
//	session.Forward("0", "success", data)
//}

func ModOrder(session *JHttp.Session) {
	type Para struct {
		OrderID     string //订单id
		MaterielID  string //材料id
		OrderNum    int    //订单数量
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
	if st.MaterielID == "" || st.CustomBatch == "" || st.OrderNum <= 0 {
		str := fmt.Sprintf("ModOrder faild,MaterielID =%s,CustomBatch=%s,OrderNum=%d\n", st.MaterielID, st.CustomBatch, st.OrderNum)
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

	if mater, err := getMaterial(st.MaterielID); err == nil {
		initOrderMaterial(data, mater)
	} else {
		JLogger.Error("MaterielID=%s获取材料失败\n", st.MaterielID)
	}
	data.CustomBatch = st.CustomBatch
	data.CustomNote = st.CustomNote
	data.OrderNum = st.OrderNum
	data.TotleMoney = data.OrderNum * data.Money

	////////////////添加状态///////////////////////////////
	appendStatus(data, data.UserName, CurTime(), "修改订单", getStatus(data.OrderNum, data.ProduceNum, data.SuccessNum))

	if err := JRedis.Redis_hset(Hash_Order, st.OrderID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
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
	data.TotleMoney = data.OrderNum * st.Money
	////////////////添加状态///////////////////////////////
	appendStatus(data, data.UserName, CurTime(), "修改订单价格", getStatus(data.OrderNum, data.ProduceNum, data.SuccessNum))

	if err := JRedis.Redis_hset(Hash_Order, st.OrderID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	////修改物料
	go modMaterialPrice(data.MaterielID, st.Money)
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
	if data.Current.Status != Status_Cancle {
		str := fmt.Sprintf("DelOrder faild,order status =%s ,is not cancel  \n", data.Current.Status)
		JLogger.Error(str)
		session.Forward("1", str, nil)
		return
	}
	appendStatus(data, data.UserName, CurTime(), "订单标记删除", Status_Del)

	if err := JRedis.Redis_hset(Hash_Order, st.OrderID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}

	//if err := JRedis.Redis_hdel(Hash_Order, st.OrderID); err != nil {
	//	session.Forward("1", err.Error(), nil)
	//	return
	//}

	if data.CustomID != "" && data.OrderID != "" {
		go removefromCustomerOrderID(data.CustomID, data.OrderID)
	}
	session.Forward("0", "success", data)
}

//订单生产完成
func PorduceOrder(session *JHttp.Session) {
	type Para struct {
		OrderID string //订单id
		Num     int    //数量
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.OrderID == "" || st.Num <= 0 {
		str := fmt.Sprintf("PorduceOrder failed,OrderID =%s,Num=%f\n", st.OrderID, st.Num)
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
		str := fmt.Sprintf("PorduceOrder faild,Current.Status=%s not support produce \n", data.Current.Status)
		JLogger.Error(str)
		session.Forward("1", str, nil)
		return
	}
	//data.ProduceID = time.Unix(time.Now().Unix(), 0).Format("20060102150405")

	data.ProduceNum += st.Num
	if data.ProduceNum >= data.OrderNum {
		data.ProduceNum = data.OrderNum
	}

	if data.ProduceNum == data.OrderNum {
		////////////////添加状态///////////////////////////////
		appendStatus(data, data.UserName, CurTime(), "订单全部生产完成", getStatus(data.OrderNum, data.ProduceNum, data.SuccessNum))
		data.ProduceTime = CurTime()
	} else {
		////////////////添加状态///////////////////////////////
		appendStatus(data, data.UserName, CurTime(), "订单完成部分生产", getStatus(data.OrderNum, data.ProduceNum, data.SuccessNum))
	}

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
		Num     int    //数量
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.OrderID == "" || st.Num <= 0 {
		str := fmt.Sprintf("SuccessOrder failed,OrderID =%s,Num=%f\n", st.OrderID, st.Num)
		JLogger.Error(str)
		session.Forward("1", str, nil)
		return
	}
	data := &Order{}
	if err := JRedis.Redis_hget(Hash_Order, st.OrderID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	if data.Current.Status == Status_Cancle || data.Current.Status == Status_Success || data.Current.Status == Status_New {
		str := fmt.Sprintf("SuccessOrder faild,Current.Status=%s not support finish \n", data.Current.Status)
		JLogger.Error(str)
		session.Forward("1", str, nil)
		return
	}

	////////////////添加状态///////////////////////////////
	data.SuccessNum += st.Num
	if data.SuccessNum >= data.OrderNum {
		data.SuccessNum = data.OrderNum
	}
	if data.SuccessNum == data.OrderNum {
		////////////////添加状态///////////////////////////////
		appendStatus(data, data.UserName, CurTime(), "订单全部出库", Status_Success)
		data.SuccessTime = CurTime()
	} else {
		////////////////添加状态///////////////////////////////
		appendStatus(data, data.UserName, CurTime(), "订单部分出库", getStatus(data.OrderNum, data.ProduceNum, data.SuccessNum))
	}

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
		if v == Key_LastOrderDate {
			continue
		}
		d := &Order{}
		if err := JRedis.Redis_hget(Hash_Order, v, d); err == nil {
			if d.Current.Status != Status_Del {
				if mater, e := getMaterial(d.MaterielID); e == nil {
					initOrderMaterial(d, mater)
				}
				d.TotleMoney = d.OrderNum * d.Money
				data = append(data, d)
			}
		}
	}
	session.Forward("0", "success", data)
}

func initOrderMaterial(data *Order, mater *MaterialInfo) {
	data.MaterielID = mater.MaterID
	data.MaterielDes = mater.MaterDes
	data.Plating = mater.Plating
	data.Friction = mater.Friction
	data.Thickness = mater.Thickness
	data.Salt = mater.Salt
	data.ComponentSolid = mater.ComponentSolid
	data.ComponentFormat = mater.ComponentFormat
	data.Unit = mater.Unit
	data.CustomID = mater.CID
	data.CustomName = mater.CustomName
	data.Factory = mater.Factory
	data.FactoryNumber = mater.FactoryNumber
	data.ProductionLine = mater.ProductionLine
	data.Money = mater.Money
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

//获取订单最后时间
func getLastOrderDate() string {
	var date string
	JRedis.Redis_hget(Hash_Order, Key_LastOrderDate, &date)
	return date
}

//设置订单最后时间
func setLastOrderDate(date string) error {
	return JRedis.Redis_hset(Hash_Order, Key_LastOrderDate, &date)
}

func getStatus(num, produce, success int) string {

	if produce == 0 && success == 0 {
		return Status_New
	}
	if produce == num && success == 0 {
		return Status_Produce
	}
	if produce == num && success == num {
		return Status_Success
	}
	if produce > 0 && produce < num && success == 0 {
		return Status_Part_Produce
	}
	if success > 0 && success < num && produce == num {
		return Status_Part_Success
	}
	if produce > 0 && produce < num && success > 0 && success < num {
		return Status_Part_Part
	}
	return Status_New
}
