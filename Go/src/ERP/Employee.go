package main

import (
	"JsGo/JsHttp"
	"JsGo/JsStore/JsRedis"
)
type Employee struct {
	UID          string //用户id
	Name         string //用户姓名
	Sex          string //性别
	Cell         string //联系方式
	CID          string //公司ID
	CommpanyName string //公司名称
	Department   string //部门
	Author       string //用户权限
	InTime       string //入职时间
	OutTime      string //离职时间
	CreatTime    string //创建时间
	Status       string //状态
	Account      string //账号
	Code         string //密码
	Age          int    //年龄
	Salary       int    //薪水
}

func NewEmployee(session JsHttp.Session)  {
	st:=&Employee{}
	if err:=session.GetPara(st);err!=nil{
		session.Forward("1",err.Error(),nil)
		return
	}
	if st.Name==""{
		session.Forward("1","NewEmployee failed,Name is empty\n",nil)
		return
	}
	st.UID = getCustomerID()
	st.CreatTime = CurTime()
	if st.Author== ""{
		st.Author= "0"
	}
	st.Status = "0"
	if st.Account!=""&&st.Code!=""{
		if err:= newAccount(st.UID,st.Name,st.Account,st.Code);err!=nil{
			session.Forward("1",err.Error(),nil)
			return
		}
	}
	if err:=JsRedis.Redis_hset(Hash_Employee,st.UID,st);err!=nil{
		session.Forward("1",err.Error(),nil)
		return
	}
	session.Forward("0","success",st)
}

func ModEmployee(session JsHttp.Session)  {
	type Para struct{
		UID          string //用户id
		Name         string //用户姓名
		Sex          string //性别
		Cell         string //联系方式
		Department   string //部门
		Author       string //用户权限
		Account      string //账号
		Code         string //密码
		Age          int    //年龄
		Salary       int    //薪水
	}
	st:=&Para{}
	if err:=session.GetPara(st);err!=nil{
		session.Forward("1",err.Error(),nil)
		return
	}
}

func DelEmployee(session JsHttp.Session)  {

}

func GetAllEmployeess(session JsHttp.Session)  {

}
