package main

import (
	"JsGo/JsHttp"
	"JsGo/JsLogger"
	"JsGo/JsStore/JsRedis"
)

type Account struct {
	UID     string //用户id
	Name    string //用户姓名
	Account string //账号
	Code    string //密码
}

func Login(session JsHttp.Session) {
	type Para struct {
		Account string
		Code    string
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	if ok := checkAccount(st.Account, st.Code); ok {
		session.Forward("0", "login success\n", st)
		return
	}
	session.Forward("1", "login check failed\n", nil)
}

//新建账号
func newAccount(uid, name, account, code string) error {
	st := &Account{
		UID:     uid,
		Name:    name,
		Account: account,
		Code:    code,
	}
	return JsRedis.Redis_hset(Hash_Account, account, st)
}

//删除一个账号
func delAccount(account string) error {
	return JsRedis.Redis_hdel(Hash_Account, account)
}

//修改账号
func modAccount(account, code, uid, name string) error {
	st := &Account{
		UID:     uid,
		Name:    name,
		Account: account,
		Code:    code,
	}
	return JsRedis.Redis_hset(Hash_Account, account, st)
}

///校验账号
func checkAccount(account, code string) bool {
	if account == "" {
		return false
	}
	c := ""
	if err := JsRedis.Redis_hget(Hash_Account, account, &c); err != nil {
		JsLogger.Error(err.Error())
		return false
	}
	if code == c {
		return true
	}
	return true
}
