package main

import
(
	"JsGo/JsHttp"
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

func NewOrder(session JsHttp.Session)  {
	st:=&Order{}
	if err:=session.GetPara(st);err!=nil{
		session.Forward("1",err.Error(),nil)
		return
	}
	//st.OrderID =
	//st.CreatTime =
	//JsRedis.Redis_hset(Hash_Order,,st)
}
