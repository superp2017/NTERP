package main

import (
	"JGo/JHttp"
	"JGo/JLogger"
	"JGo/JStore/JRedis"
	"fmt"
)

type Employee struct {
	UID          string //用户id
	Name         string //用户姓名
	Sex          string //性别
	Cell         string //联系方式
	CID          string //公司ID
	CommpanyName string //公司名称
	Department   string //部门
	Author       int    //用户权限
	InTime       string //入职时间
	OutTime      string //离职时间
	CreatTime    string //创建时间
	CreatStamp   int64  //创建的时间戳
	LastTime     int64  //最后更新时间
	Status       string //状态
	Account      string //账号
	Code         string //密码
	Age          int    //年龄
	Salary       int    //薪水
	IsDel        bool   //标记删除
}

//新增一个员工
func NewEmployee(session *JHttp.Session) {
	st := &Employee{}
	if err := session.GetPara(st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.Name == "" {
		session.Forward("1", "NewEmployee failed,Name is empty\n", nil)
		return
	}
	st.UID = getEmployeeID()
	st.CreatTime = CurTime()
	st.CreatStamp = CurStamp()
	st.LastTime = CurStamp()
	st.Status = "0"
	if st.Account != "" && st.Code != "" {
		if exist, e := JRedis.Redis_hexists(Hash_Account, st.Account); e == nil && exist {
			str := fmt.Sprintf("NewEmployee faild,Account is exist\n")
			JLogger.Error(str)
			session.Forward("2", str, nil)
			return
		}
		if err := newAccount(st.UID, st.Name, st.Account, st.Code); err != nil {
			session.Forward("1", err.Error(), nil)
			return
		}
	}
	if err := JRedis.Redis_hset(Hash_Employee, st.UID, st); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}

	//更新
	increaseUpdate(STRUCT_USER)

	session.Forward("0", "success", st)
}

//修改员工信息
func ModEmployee(session *JHttp.Session) {
	type Para struct {
		UID        string //用户id
		Name       string //用户姓名
		Sex        string //性别
		Cell       string //联系方式
		Department string //部门
		Author     int    //用户权限
		Account    string //账号
		Code       string //密码
		Age        int    //年龄
		Salary     int    //薪水
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.UID == "" || st.Account == "" || st.Code == "" || st.Name == "" {
		str := fmt.Sprintf("ModEmployee faild,UID = %s,Account = %s,Code = %s,Name = %s\n",
			st.UID, st.Account, st.Code, st.Name)
		JLogger.Error(str)
		session.Forward("1", str, nil)
		return
	}
	data := &Employee{}
	if err := JRedis.Redis_hget(Hash_Employee, st.UID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	if data.Account == st.Account {
		go modAccount(st.Account, st.Code, st.UID, st.Name)
	} else {
		go delAccount(data.Account)

		go newAccount(st.UID, st.Name, st.Account, st.Code)
	}
	data.Name = st.Name
	data.Department = st.Department
	data.Sex = st.Sex
	data.Cell = st.Cell
	data.Age = st.Age
	data.Salary = st.Salary
	data.Account = st.Account
	data.Code = st.Code
	data.Author = st.Author
	data.LastTime = CurStamp()
	if err := JRedis.Redis_hset(Hash_Employee, st.UID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}

	//更新
	increaseUpdate(STRUCT_USER)

	session.Forward("0", "success", data)
}

//员工离职
func OutEmployee(session *JHttp.Session) {
	type Para struct {
		UID string //用户id
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.UID == "" {
		session.Forward("1", "OutEmployee failed,UID is empty\n", nil)
		return
	}
	data := &Employee{}
	if err := JRedis.Redis_hget(Hash_Employee, st.UID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	data.OutTime = CurTime()
	data.Status = "1"
	data.LastTime = CurStamp()
	if err := JRedis.Redis_hset(Hash_Employee, st.UID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	go delAccount(data.Account)

	//更新
	increaseUpdate(STRUCT_USER)

	session.Forward("0", "success", data)
}

//删除某一个员工
func DelEmployee(session *JHttp.Session) {
	type Para struct {
		UID string //用户id
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.UID == "" {
		session.Forward("1", "DelEmployee failed,UID is empty\n", nil)
		return
	}
	data := &Employee{}
	if err := JRedis.Redis_hget(Hash_Employee, st.UID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}

	data.IsDel = true

	if err := JRedis.Redis_hset(Hash_Employee, st.UID, data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	//if err := JRedis.Redis_hdel(Hash_Employee, st.UID); err != nil {
	//	session.Forward("1", err.Error(), nil)
	//	return
	//}

	go delAccount(data.Account)

	//更新
	increaseUpdate(STRUCT_USER)

	session.Forward("0", "success", data)
}

// 获取所有的员工信息
func GetAllEmployee(session *JHttp.Session) {
	st := &AllPara{}
	if err := session.GetPara(st); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	ids, err := JRedis.Redis_hkeys(Hash_Employee)
	if err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	data := []*Employee{}
	if st.Type == 1 {
		if st.Stamp > getUpdateStamp(STRUCT_USER) {
			session.Forward("0", "success", data)
			return
		}
	}
	for _, v := range ids {
		d := &Employee{}
		if err := JRedis.Redis_hget(Hash_Employee, v, d); err == nil {
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
	JLogger.Error("GetAllEmployeess:type=%d,num=%d,start=%d,stamp=%d,data=%v\n", st.Type, st.Num, st.Start, st.Stamp, data)
	session.Forward("0", "success", data)
}

//添加一个部门
func NewDepartment(session *JHttp.Session) {
	type Para struct {
		Department string
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.Department == "" {
		JLogger.Error("Department is empty\n")
		session.Forward("1", "Department is empty\n", nil)
		return
	}
	if err := appendDepartment(st.Department); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}

	session.Forward("0", "NewDepartment success\n", st.Department)
}

//删除一个部门
func RemoveDepartment(session *JHttp.Session) {
	type Para struct {
		Department string
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.Department == "" {
		JLogger.Error("Department is empty\n")
		session.Forward("1", "Department is empty\n", nil)
		return
	}
	if err := delDepartment(st.Department); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}

	session.Forward("0", "RemoveDepartment success\n", st.Department)
}

//获取所有的部门
func GetAllDepartment(session *JHttp.Session) {
	list, err := getDepartment()
	if err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	session.Forward("0", "GetAllDepartment success", list)
}

//追加一个部门
func appendDepartment(depart string) error {
	return JRedis.Redis_Sset(Key_Department, depart)
}

//删除一个部门
func delDepartment(depart string) error {
	return JRedis.Redis_Sdel(Key_Department, depart)
}

//获取部门
func getDepartment() ([]string, error) {
	data := []string{}
	d, err := JRedis.Redis_Sget(Key_Department)
	for _, v := range d {
		data = append(data, string(v.([]byte)))
	}
	return data, err
}
