package main

import (
	"JsGo/JsHttp"
	"JsGo/JsLogger"
	"JsGo/JsStore/JsRedis"
	"fmt"
)

type Supplier struct {
	SID             string //客户编号
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
	Goods           string //商品列表
	Status          string //状态 0:正常 1:删除
	CreatTime       string //创建时间
}

//新建一个客户
func NewSupplier(session *JsHttp.Session) {
	st := &Supplier{}
	if err := session.GetPara(st); err != nil {
		JsLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.Name == "" || st.Tel == "" {
		str := fmt.Sprintf("NewSupplier faild,Name = %s,Tel = %s\n", st.Name, st.Tel)
		JsLogger.Error(str)
		session.Forward("1", str, nil)
		return
	}
	st.SID = getSupplierID()
	st.CreatTime = CurTime()
	st.Status = "0"
	if err := JsRedis.Redis_hset(Hash_Supplier, st.SID, st); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	session.Forward("0", "success", st)
}

//修改客户信息
func ModSupplier(session *JsHttp.Session) {
	type Para struct {
		SID             string //客户编号
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
		str := fmt.Sprintf("ModSupplier faild,Name = %s,Tel = %s\n", st.Name, st.Tel)
		JsLogger.Error(str)
		session.Forward("1", str, nil)
		return
	}
	data := &Supplier{}
	if err := JsRedis.Redis_hget(Hash_Supplier, st.SID, data); err != nil {
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
	if err := JsRedis.Redis_hset(Hash_Supplier, st.SID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	session.Forward("0", "success", data)
}

///客户的解约和合作
func UpDownSupplier(session *JsHttp.Session) {
	type Para struct {
		SID  string
		IsUp bool
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		JsLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	data := &Supplier{}
	if err := JsRedis.Redis_hget(Hash_Supplier, st.SID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.IsUp {
		data.Status = "0"
	} else {
		data.Status = "1"
	}
	if err := JsRedis.Redis_hset(Hash_Supplier, st.SID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	session.Forward("0", "success", data)
}

//删除一个客户
func DelSupplier(session *JsHttp.Session) {
	type Para struct {
		SID string
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		JsLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.SID == "" {
		str := fmt.Sprintf("DelSupplier SID is enpty\n")
		JsLogger.Error(str)
		session.Forward("1", str, nil)
		return
	}
	if err := JsRedis.Redis_hdel(Hash_Supplier, st.SID); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	session.Forward("0", "success", st.SID)
}
