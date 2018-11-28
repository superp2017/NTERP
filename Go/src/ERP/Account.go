package main

import (
	"JGo/JHttp"
	"JGo/JLogger"
	"JGo/JStore/JRedis"
)

type Account struct {
	UID     string //用户id
	Name    string //用户姓名
	Account string //账号
	Code    string //密码
	LastTime int64//最后更新时间
}

func Login(session *JHttp.Session) {
	type Para struct {
		Account string
		Code    string
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	JLogger.Error("Account=%s,code=%s", st.Account, st.Code)
	if st.Account == "" || st.Code == "" {
		session.Forward("1", "login filed,Account or Code is empty\n", nil)
		return
	}
	UID, ok := checkAccount(st.Account, st.Code)
	if !ok {
		session.Forward("1", "checkAccount failed\n", st)
		return
	}
	data := &Employee{}
	if err := JRedis.Redis_hget(Hash_Employee, UID, data); err != nil {
		JLogger.ErrorLog("get Employee filed,UID=%s\n", UID)
		session.Forward("1", err.Error(), nil)
		return
	}
	session.Forward("0", "login success\n", data)
}

//新建账号
func newAccount(uid, name, account, code string) error {
	st := &Account{
		UID:     uid,
		Name:    name,
		Account: account,
		Code:    code,
		LastTime:CurStamp(),
	}
	return JRedis.Redis_hset(Hash_Account, account, st)
}

//删除一个账号
func delAccount(account string) error {
	return JRedis.Redis_hdel(Hash_Account, account)
}

//修改账号
func modAccount(account, code, uid, name string) error {
	st := &Account{
		UID:     uid,
		Name:    name,
		Account: account,
		Code:    code,
		LastTime:CurStamp(),
	}
	return JRedis.Redis_hset(Hash_Account, account, st)
}

///校验账号
func checkAccount(account, code string) (string, bool) {
	if account == "" {
		return "", false
	}
	c := &Account{}
	if err := JRedis.Redis_hget(Hash_Account, account, c); err != nil {
		JLogger.Error(err.Error())
		return "", false
	}
	if code == c.Code {
		return c.UID, true
	}
	return "", false
}
