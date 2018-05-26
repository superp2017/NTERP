package main

import "JsGo/JsStore/JsRedis"

type Account struct {
	UID     string //用户id
	Name    string //用户姓名
	Account string //账号
	Code    string //密码
}

//新建账号
func newAccount(uid,name,account,code string)error  {
	st:=&Account{
		UID:uid,
		Name:name,
		Account:account,
		Code:code,
	}
	return JsRedis.Redis_hset(Hash_Account,account,st)
}
//删除一个账号
func delAccount(account string)error  {
	return JsRedis.Redis_hdel(Hash_Account,account)
}

//修改账号
func modAccount(account,code,uid,name string)error  {
	st:=&Account{
		UID:uid,
		Name:name,
		Account:account,
		Code:code,
	}
	return JsRedis.Redis_hset(Hash_Account,account,st)
}