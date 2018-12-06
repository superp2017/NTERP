package main

import (
	"JGo/JHttp"
	"JGo/JLogger"
	"JGo/JStore/JRedis"
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
	CreatStamp      int64  //创建的时间戳
	LastTime        int64  //最后更新时间
	IsDel           bool   //标记删除
}

//新建一个客户
func NewCustomer(session *JHttp.Session) {
	st := &Customer{}
	if err := session.GetPara(st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.Name == "" {
		str := fmt.Sprintf("NewCustomer faild,Name = %s\n", st.Name)
		JLogger.Error(str)
		session.Forward("1", str, nil)
		return
	}
	st.CID = getCustomerID()
	st.CreatTime = CurTime()
	st.CreatStamp = CurStamp()
	st.LastTime = CurStamp()
	st.Status = "0"

	if err := JRedis.Redis_hset(Hash_Customer, st.CID, st); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}

	//更新
	increaseUpdate(STRUCT_CUSTOMER)

	session.Forward("0", "success", st)
}

//修改客户信息
func ModCustomer(session *JHttp.Session) {
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
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.Name == "" {
		str := fmt.Sprintf("ModCustomer faild,Name = %s\n", st.Name)
		JLogger.Error(str)
		session.Forward("1", str, nil)
		return
	}
	data := &Customer{}
	if err := JRedis.Redis_hget(Hash_Customer, st.CID, data); err != nil {
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
	if err := JRedis.Redis_hset(Hash_Customer, st.CID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}

	//更新
	increaseUpdate(STRUCT_CUSTOMER)

	session.Forward("0", "success", data)
}

///客户的解约和合作
func UpDownCustomer(session *JHttp.Session) {
	type Para struct {
		CID  string
		IsUp bool
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	data := &Customer{}
	if err := JRedis.Redis_hget(Hash_Customer, st.CID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.IsUp {
		data.Status = "0"
	} else {
		data.Status = "1"
	}
	data.LastTime = CurStamp()
	if err := JRedis.Redis_hset(Hash_Customer, st.CID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}

	//更新
	increaseUpdate(STRUCT_CUSTOMER)

	session.Forward("0", "success", data)
}

//删除一个客户
func DelCustomer(session *JHttp.Session) {
	type Para struct {
		CID string
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.CID == "" {
		str := fmt.Sprintf("DelCustomer CID is enpty\n")
		JLogger.Error(str)
		session.Forward("1", str, nil)
		return
	}
	data := &Customer{}
	if err := JRedis.Redis_hget(Hash_Customer, st.CID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	data.IsDel = true
	if err := JRedis.Redis_hset(Hash_Customer, st.CID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}

	//if err := JRedis.Redis_hdel(Hash_Customer, st.CID); err != nil {
	//	session.Forward("1", err.Error(), nil)
	//	return
	//}

	//更新
	increaseUpdate(STRUCT_CUSTOMER)

	session.Forward("0", "success", data)
}

//获取所有客户列表
func GetAllCustomer(session *JHttp.Session) {
	st := &AllPara{}
	if err := session.GetPara(st); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	data := []*Customer{}
	if st.Type == 1 {
		if st.Stamp > getUpdateStamp(STRUCT_CUSTOMER) {
			session.Forward("0", "success", data)
			return
		}
	}

	list, err := JRedis.Redis_hkeys(Hash_Customer)
	if err == nil {
		for _, v := range list {
			d := &Customer{}
			if e := JRedis.Redis_hget(Hash_Customer, v, d); e == nil {
				if !d.IsDel {
					if st.Type == 0 {
						data = append(data, d)
					} else {
						if d.LastTime > st.Stamp {
							data = append(data, d)
						}
					}
				}
			}
		}
	}
	JLogger.Error("GetAllCustomer:type=%d,num=%d,start=%d,stamp=%d,data=%v\n", st.Type, st.Num, st.Start, st.Stamp, data)
	session.Forward("0", "success", data)
}
