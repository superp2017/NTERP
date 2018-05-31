package main

import (
	"JsGo/JsHttp"
	"JsGo/JsLogger"
	"JsGo/JsStore/JsRedis"
	"fmt"
)

type Customer struct {
	CID             string //客户编号
	Name            string //客户公司名称
	Icon            string //客户公司logo
	Addr            string //客户公司地址
	Tel             string //公司电话
	ContactName     string //联系人
	ContactCell     string //联系人电话
	BankName        string //开户行
	BankNumber      string //银行卡号
	Bankbranch      string //银行支行
	CertificatesNum string //税号
	Certificates    string //客户证件（营业执照）
	Note            string //备注
	Status          string //状态 0:正常 1:删除
	CreatTime       string //创建时间
}

//新建一个客户
func NewCustomer(session *JsHttp.Session) {
	st := &Customer{}
	if err := session.GetPara(st); err != nil {
		JsLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.Name == "" || st.Tel == "" {
		str := fmt.Sprintf("NewCustomer faild,Name = %s,Tel = %s\n", st.Name, st.Tel)
		JsLogger.Error(str)
		session.Forward("1", str, nil)
		return
	}
	st.CID = getCustomerID()
	st.CreatTime = CurTime()
	st.Status = "0"
	if err := JsRedis.Redis_hset(Hash_Customer, st.CID, st); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	session.Forward("0", "success", st)
}

//修改客户信息
func ModCustomer(session *JsHttp.Session) {
	type Para struct {
		CID             string //客户编号
		Name            string //客户公司名称
		Icon            string //客户公司logo
		Addr            string //客户公司地址
		Tel             string //公司电话
		ContactName     string //联系人
		ContactCell     string //联系人电话
		BankName        string //开户行
		BankNumber      string //银行卡号
		Bankbranch      string //银行支行
		CertificatesNum string //税号
		Certificates    string //客户证件（营业执照）
		Note            string //备注
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		JsLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.Name == "" || st.Tel == "" {
		str := fmt.Sprintf("ModCustomer faild,Name = %s,Tel = %s\n", st.Name, st.Tel)
		JsLogger.Error(str)
		session.Forward("1", str, nil)
		return
	}
	data := &Customer{}
	if err := JsRedis.Redis_hget(Hash_Customer, st.CID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	data.Name = st.Name
	data.Icon = st.Icon
	data.Addr = st.Addr
	data.Tel = st.Tel
	data.ContactName = st.ContactName
	data.ContactCell = st.ContactCell
	data.BankName = st.BankName
	data.BankNumber = st.BankNumber
	data.Bankbranch = st.Bankbranch
	data.CertificatesNum = st.CertificatesNum
	data.Certificates = st.Certificates
	data.Note = st.Note
	if err := JsRedis.Redis_hset(Hash_Customer, st.CID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	session.Forward("0", "success", data)
}

///客户的解约和合作
func UpDownCustomer(session *JsHttp.Session) {
	type Para struct {
		CID  string
		IsUp bool
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		JsLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	data := &Customer{}
	if err := JsRedis.Redis_hget(Hash_Customer, st.CID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.IsUp {
		data.Status = "0"
	} else {
		data.Status = "1"
	}
	if err := JsRedis.Redis_hset(Hash_Customer, st.CID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	session.Forward("0", "success", data)
}

//删除一个客户
func DelCustomer(session *JsHttp.Session) {
	type Para struct {
		CID string
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		JsLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.CID == "" {
		str := fmt.Sprintf("DelCustomer CID is enpty\n")
		JsLogger.Error(str)
		session.Forward("1", str, nil)
		return
	}
	if err := JsRedis.Redis_hdel(Hash_Customer, st.CID); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	go delCustomerOrderID(st.CID)
	session.Forward("0", "success", st.CID)
}

//获取所有客户列表
func GetAllCustomer(session *JsHttp.Session)  {
	list,err:=JsRedis.Redis_hkeys(Hash_Customer)
	data := []*Customer{}
	if err==nil{
		for _,v:=range list{
			d := &Customer{}
			if e:=JsRedis.Redis_hget(Hash_Customer,v,d);e==nil{
				data = append(data,d)
			}
		}
	}
	session.Forward("0","success",data)
}

//添加一个订单到 客户订单列表中
func appendCustomerOrderID(CID, OrderID string) error {
	data:=[]string{}
	JsRedis.Redis_hget(Hash_CustomerOrder,CID,data)
	exist:=false
	for _,v:=range data{
		if v==OrderID{
			exist = true
			break
		}
	}
	if !exist{
		data = append(data,OrderID)
	}
	return JsRedis.Redis_hset(Hash_CustomerOrder, CID, OrderID)
}

//从客户订单中移除一个订单
func removefromCustomerOrderID(CID, OrderID string)error{
	data:=[]string{}
	JsRedis.Redis_hget(Hash_CustomerOrder,CID,data)
	index:=-1
	for i,v:=range data{
		if v==OrderID{
			index = i
			break
		}
	}
	if index!=-1{
		data = append(data[:index],data[index+1:]...)
	}
	return JsRedis.Redis_hset(Hash_CustomerOrder, CID, OrderID)
}

//删除客户订单
func delCustomerOrderID(CID string) error {
	return JsRedis.Redis_hdel(Hash_CustomerOrder, CID)
}

