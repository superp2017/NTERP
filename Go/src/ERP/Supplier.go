package main

import (
	"JGo/JHttp"
	"JGo/JLogger"
	"JGo/JStore/JRedis"
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
	CreatStamp      int64  //创建的时间戳
	LastTime        int64  //最后更新时间
}

//新建一个客户
func NewSupplier(session *JHttp.Session) {
	st := &Supplier{}
	if err := session.GetPara(st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.Name == "" {
		str := fmt.Sprintf("NewSupplier faild,Name = %s\n", st.Name)
		JLogger.Error(str)
		session.Forward("1", str, nil)
		return
	}
	st.SID = getSupplierID()
	st.CreatTime = CurTime()
	st.LastTime = CurStamp()
	st.CreatStamp = CurStamp()
	st.Status = "0"
	if err := JRedis.Redis_hset(Hash_Supplier, st.SID, st); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	//通知
	go Notice(&NoticeInfo{
		NoticeType: NoticeType_NEW,
		DataType:   STRUCT_SUPPLIER,
		Data:       st,
		Addr:       RequestAddr(session.Req),
	})
	session.Forward("0", "success", st)
}

//修改客户信息
func ModSupplier(session *JHttp.Session) {
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
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.Name == "" {
		str := fmt.Sprintf("ModSupplier faild,Name = %s\n", st.Name)
		JLogger.Error(str)
		session.Forward("1", str, nil)
		return
	}
	data := &Supplier{}
	if err := JRedis.Redis_hget(Hash_Supplier, st.SID, data); err != nil {
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
	data.LastTime = CurStamp()
	if err := JRedis.Redis_hset(Hash_Supplier, st.SID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	//通知
	go Notice(&NoticeInfo{
		NoticeType: NoticeType_Modify,
		DataType:   STRUCT_SUPPLIER,
		Data:       st,
		Addr:       RequestAddr(session.Req),
	})
	session.Forward("0", "success", data)
}

///客户的解约和合作
func UpDownSupplier(session *JHttp.Session) {
	type Para struct {
		SID  string
		IsUp bool
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	data := &Supplier{}
	if err := JRedis.Redis_hget(Hash_Supplier, st.SID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.IsUp {
		data.Status = "0"
	} else {
		data.Status = "1"
	}
	data.LastTime = CurStamp()
	if err := JRedis.Redis_hset(Hash_Supplier, st.SID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	//通知
	go Notice(&NoticeInfo{
		NoticeType: NoticeType_Modify,
		DataType:   STRUCT_SUPPLIER,
		Data:       st,
		Addr:       RequestAddr(session.Req),
	})
	session.Forward("0", "success", data)
}

//删除一个客户
func DelSupplier(session *JHttp.Session) {
	type Para struct {
		SID string
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.SID == "" {
		str := fmt.Sprintf("DelSupplier SID is enpty\n")
		JLogger.Error(str)
		session.Forward("1", str, nil)
		return
	}
	sup := &Supplier{}
	if err := JRedis.Redis_hget(Hash_Supplier, st.SID, sup); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}

	if err := JRedis.Redis_hdel(Hash_Supplier, st.SID); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	//通知
	go Notice(&NoticeInfo{
		NoticeType: NoticeType_Del,
		DataType:   STRUCT_SUPPLIER,
		Data:       sup,
		Addr:       RequestAddr(session.Req),
	})
	session.Forward("0", "success", sup)
}

//获取所有供应商列表
func GetAllSupplier(session *JHttp.Session) {
	list, err := JRedis.Redis_hkeys(Hash_Supplier)
	data := []*Supplier{}
	if err == nil {
		for _, v := range list {
			d := &Supplier{}
			if e := JRedis.Redis_hget(Hash_Supplier, v, d); e == nil {
				data = append(data, d)
			}
		}
	}
	session.Forward("0", "success", data)
}
