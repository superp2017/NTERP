package main

import (
	"JGo/JHttp"
	"JGo/JLogger"
	"JGo/JStore/JRedis"
	"fmt"
	"sort"
	"strconv"
	"time"
)

type OderFlow struct {
	UserName   string //用户姓名
	OpreatTime string //操作时间
	Action     string //动作
	Status     string //状态
}
type PrintDetail struct {
	UserID    string //用户ID
	UserName  string //用户姓名
	PrintDate string //打印时间
}

type Order struct {
	UID             string        //用户id
	UserName        string        //用户姓名
	OrderID         string        //订单id
	OrderType       string        //订单类型（普通订单、试样订单、返工订单）
	Factory         string        //分厂名称
	FactoryNumber   string        //分厂号
	ProductionLine  string        //产线名称
	MaterielID      string        //材料id
	MaterielDes     string        //材料描述
	Plating         string        //镀种
	Friction        string        //摩擦系数
	Thickness       string        //厚度
	Salt            string        //盐度
	ComponentSolid  string        //零件固号
	ComponentFormat string        //零件规格
	Unit            string        //单位
	CustomID        string        //客户ID
	CustomName      string        //客户姓名
	CustomBatch     string        //客户批次
	CustomNote      string        //客户备注
	ProduceID       string        //生产编号
	CreatTime       string        //创建时间
	CreatStamp      int64         //创建的时间戳
	LastTime        int64         //最后更新时间
	IsDel           bool          //是否删除
	ProduceTime     string        //出货时间
	SuccessTime     string        //完成时间
	Current         OderFlow      //当前状态
	Flow            []OderFlow    //订单流程
	OrderNum        float64       //订单数量
	ProduceNum      float64       //生产完成数量
	SuccessNum      float64       //出库数量
	TotleMoney      float64       //总价
	Money           float64       //单价
	PrintNum        int           //打印次数
	Print           []PrintDetail //打印记录
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
	st.CreatStamp = CurStamp()
	st.LastTime = CurStamp()
	st.TotleMoney = Decimal(st.OrderNum * st.Money)
	st.IsDel = false
	st.PrintNum = 0
	st.ProduceNum = 0
	st.SuccessNum = 0

	go setLastOrderDate(curMon)
	////////////////添加状态///////////////////////////////
	appendStatus(st, st.UserName, st.CreatTime, "创建订单", Status_New)
	if err := JRedis.Redis_hset(Hash_Order, st.OrderID, st); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}

	increaseUpdate(STRTUCT_ORDER)
	session.Forward("0", "success", st)
}

//修改订单
func ModOrder(session *JHttp.Session) {
	type Para struct {
		OrderID     string  //订单id
		MaterielID  string  //材料id
		OrderNum    float64 //订单数量
		CustomBatch string  //客户批次
		CustomNote  string  //客户备注
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
	if st.MaterielID == "" || st.OrderNum <= 0 {
		str := fmt.Sprintf("ModOrder faild,MaterielID =%s,CustomBatch=%s,OrderNum=%g\n", st.MaterielID, st.CustomBatch, st.OrderNum)
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
	if data.MaterielID != st.MaterielID {
		if mater, err := getMaterial(st.MaterielID); err == nil {
			copyMaterial2Order(data, mater)
		} else {
			JLogger.Error("MaterielID=%s获取材料失败\n", st.MaterielID)
		}
	}
	data.CustomBatch = st.CustomBatch
	data.CustomNote = st.CustomNote
	data.OrderNum = st.OrderNum
	data.TotleMoney = Decimal(data.OrderNum * data.Money)
	data.LastTime = CurStamp()

	////////////////添加状态///////////////////////////////
	appendStatus(data, data.UserName, CurTime(), "修改订单", getStatus(data.OrderNum, data.ProduceNum, data.SuccessNum))

	if err := JRedis.Redis_hset(Hash_Order, st.OrderID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	//更新
	increaseUpdate(STRTUCT_ORDER)

	session.Forward("0", "success", data)
}

//修改订单价格
func ModOrderPrice(session *JHttp.Session) {
	type Para struct {
		OrderID string  //订单id
		Money   float64 //价格
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

	if data.IsDel || data.Current.Status == Status_Cancle || data.Current.Status == Status_All || data.Current.Status == "Status_Del" {
		str := fmt.Sprintf("ModOrderPrice faild,Current.Status=%s not support modify price \n", data.Current.Status)
		JLogger.Error(str)
		session.Forward("1", str, nil)
		return
	}

	data.Money = st.Money
	data.TotleMoney = Decimal(data.OrderNum * st.Money)
	data.LastTime = CurStamp()
	////////////////添加状态///////////////////////////////
	appendStatus(data, data.UserName, CurTime(), "修改订单价格", getStatus(data.OrderNum, data.ProduceNum, data.SuccessNum))

	if err := JRedis.Redis_hset(Hash_Order, st.OrderID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}

	//更新
	increaseUpdate(STRTUCT_ORDER)

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
	data.LastTime = CurStamp()
	////////////////添加状态///////////////////////////////
	appendStatus(data, data.UserName, CurTime(), "订单取消", Status_Cancle)

	if err := JRedis.Redis_hset(Hash_Order, st.OrderID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}

	//更新
	increaseUpdate(STRTUCT_ORDER)

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
	////appendStatus(data, data.UserName, CurTime(), "订单标记删除", Status_Del)
	data.LastTime = CurStamp()
	data.IsDel = true
	if err := JRedis.Redis_hset(Hash_Order, st.OrderID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	//更新
	increaseUpdate(STRTUCT_ORDER)

	session.Forward("0", "success", data)
}

//订单生产完成
func PorduceOrder(session *JHttp.Session) {
	type Para struct {
		OrderID string  //订单id
		Num     float64 //数量
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

	data.ProduceNum += st.Num
	if data.ProduceNum >= data.OrderNum {
		data.ProduceNum = data.OrderNum
	}

	if data.ProduceNum == data.OrderNum {
		////////////////添加状态///////////////////////////////
		appendStatus(data, data.UserName, CurTime(), "订单全部生产完成", getStatus(data.OrderNum, data.ProduceNum, data.SuccessNum))
	} else {
		////////////////添加状态///////////////////////////////
		appendStatus(data, data.UserName, CurTime(), "订单完成部分生产", getStatus(data.OrderNum, data.ProduceNum, data.SuccessNum))
	}
	data.ProduceTime = CurTime()
	data.LastTime = CurStamp()

	if err := JRedis.Redis_hset(Hash_Order, st.OrderID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}

	//更新
	increaseUpdate(STRTUCT_ORDER)

	session.Forward("0", "success", data)
}

//订单完成
func SuccessOrder(session *JHttp.Session) {
	type Para struct {
		OrderID string  //订单id
		Num     float64 //数量
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
	} else {
		////////////////添加状态///////////////////////////////
		appendStatus(data, data.UserName, CurTime(), "订单部分出库", getStatus(data.OrderNum, data.ProduceNum, data.SuccessNum))
	}
	data.SuccessTime = CurTime()
	data.LastTime = CurStamp()

	if err := JRedis.Redis_hset(Hash_Order, st.OrderID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	//更新
	increaseUpdate(STRTUCT_ORDER)

	session.Forward("0", "success", data)
}

//获取所有订单列表
func GetGlobalOrders(session *JHttp.Session) {
	st := &AllPara{}
	if err := session.GetPara(st); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.Type < 0 || st.Type > 3 || st.Type == 2 && st.Num <= 0 {
		session.Forward("1", "param error\n", nil)
		return
	}
	isOver := false
	data := []*Order{}
	type Res struct {
		IsOver bool
		Data   []*Order
	}
	res := &Res{}
	if st.Type == 1 {
		if st.Stamp > getUpdateStamp(STRTUCT_ORDER) {
			res.Data = data
			res.IsOver = isOver
			session.Forward("0", "success", res)
			return
		}
	}
	list, err := JRedis.Redis_hkeys(Hash_Order)
	if err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}

	for _, v := range list {
		if v == Key_LastOrderDate {
			continue
		}
		d := &Order{}
		if err := JRedis.Redis_hget(Hash_Order, v, d); err == nil {
			if !d.IsDel {
				if st.Type == 1 {
					if d.LastTime > st.Stamp {
						data = append(data, d)
					}
				} else {
					data = append(data, d)
				}
			}
		} else {
			JLogger.Error(" Redis_hget,OrderID=%s,err =%s\n", v, err)
		}
	}

	lenData := len(data)
	if lenData > 0 {
		sort.Slice(data, func(i, j int) bool {
			return data[i].CreatStamp > data[j].CreatStamp //最新创建的放前面
		})
	}

	if st.Type == 2 {
		startIndex := -1
		for i, v := range data {
			if v.OrderID == st.Start {
				startIndex = i
			}
		}
		if startIndex == -1 && st.Start != "" {
			res.Data = []*Order{}
			res.IsOver = isOver
			session.Forward("0", "success", res)
			return
		}
		startIndex += 1
		if (startIndex + st.Num) <= lenData {
			data = data[startIndex : startIndex+st.Num]
		} else {
			data = data[startIndex:lenData]
		}
		if startIndex+st.Num >= lenData {
			isOver = true
		}
	}
	res.Data = data
	res.IsOver = isOver
	session.Forward("0", "success", res)
}

func SearchOrder(session *JHttp.Session) {
	type Para struct {
		CusName    string
		Status     string
		OrderType  string
		Factory    string
		StartStamp int64
		EndStamp   int64
		IsCus      bool
		IsType     bool
		IsStatus   bool
		IsFac      bool
		IsTime     bool
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
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
			if !d.IsDel {
				if st.IsStatus && st.Status != d.Current.Status && st.Status != Status_All {
					continue
				}
				if st.IsCus && st.CusName != d.CustomName {
					continue
				}
				if st.IsFac && st.Factory != d.Factory && st.Factory != "全部分厂" {
					continue
				}
				if st.IsType && st.OrderType != d.OrderType {
					continue
				}
				if st.IsTime && (d.CreatStamp < st.StartStamp || d.CreatStamp > st.EndStamp) {
					continue
				}
				data = append(data, d)
			}
		}
	}
	lenData := len(data)
	if lenData > 0 {
		sort.Slice(data, func(i, j int) bool {
			return data[i].CreatStamp <= data[j].CreatStamp
		})
	}
	session.Forward("0", "success", data)
}

func SearchOutOrder(session *JHttp.Session) {
	type Para struct {
		CusName    string
		StartStamp int64
		EndStamp   int64
		IsCus      bool
		IsTime     bool
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
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
			if !d.IsDel {
				if d.Current.Status != Status_Success {
					continue
				}
				if st.IsCus && st.CusName != d.CustomName {
					continue
				}
				if st.IsTime && (d.LastTime < st.StartStamp || d.LastTime > st.EndStamp) {
					continue
				}
				data = append(data, d)
			}
		}
	}
	lenData := len(data)
	if lenData > 0 {
		sort.Slice(data, func(i, j int) bool {
			return data[i].CreatStamp <= data[j].CreatStamp
		})
	}
	session.Forward("0", "success", data)
}

//获取打印号
func GetPrintNum(session *JHttp.Session) {
	err, num := setPrintNumber(false)
	if err != nil {
		session.Forward("0", "failed", num)
		return
	}
	session.Forward("0", "success", num)
}

//更新打印号
func UpdatePrintNum(session *JHttp.Session) {
	type Para struct {
		Orders   []string
		UserID   string
		UserName string
	}
	st := &Para{}
	if err := session.GetPara(&st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	list := []*Order{}
	for _, v := range st.Orders {
		data := &Order{}
		if err := JRedis.Redis_hget(Hash_Order, v, data); err == nil {
			data.PrintNum++
			data.Print = append(data.Print, PrintDetail{
				UserID:    st.UserID,
				UserName:  st.UserName,
				PrintDate: CurTime(),
			})
			data.LastTime = CurStamp()
			if e := JRedis.Redis_hset(Hash_Order, v, data); e == nil {
				list = append(list, data)
			}
		}
	}
	go setPrintNumber(true)

	increaseUpdate(STRTUCT_ORDER)
	session.Forward("0", "success", list)
}

//设置打印号
func setPrintNumber(isSet bool) (error, string) {
	num := 0
	if err := JRedis.Redis_get("PrintNumber", &num); err != nil {
		return err, time.Unix(time.Now().Unix(), 0).Format("2006") + strconv.Itoa(8888)
	}
	if !isSet {
		return nil, time.Unix(time.Now().Unix(), 0).Format("2006") + strconv.Itoa(num)
	}
	num++
	if err := JRedis.Redis_set("PrintNumber", &num); err != nil {
		return err, time.Unix(time.Now().Unix(), 0).Format("2006") + strconv.Itoa(8888)
	}
	return nil, time.Unix(time.Now().Unix(), 0).Format("2006") + strconv.Itoa(num)
}

//复制mater信息到order
func copyMaterial2Order(data *Order, mater *MaterialInfo) {
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

func getStatus(num, produce, success float64) string {

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
