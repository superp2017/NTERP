package main

import (
	"JGo/JHttp"
	"JGo/JLogger"
	"JGo/JStore/JRedis"
)

type Unit struct {
	Unit string
}

//新增一个单位
func NewUnit(session *JHttp.Session) {
	type Para struct {
		Unit string
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.Unit == "" {
		session.Forward("1", "NewUnit:Unit is empty\n", nil)
		return
	}
	if err := JRedis.Redis_Sset(Key_Unit, st.Unit); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}

	//更新
	//go newUpdate(STRUCT_UNIT, st.Unit, NoticeType_NEW, st.Unit)
	session.Forward("0", "success", st.Unit)
}

//删除一个单位
func DelUnit(session *JHttp.Session) {
	type Para struct {
		Unit string
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.Unit == "" {
		session.Forward("1", "DelUnit:Unit is empty\n", nil)
		return
	}
	if err := JRedis.Redis_Sdel(Key_Unit, st.Unit); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}

	//更新
	//go newUpdate(STRUCT_UNIT, st.Unit, NoticeType_Del, st.Unit)
	session.Forward("0", "success", st.Unit)
}

//获取所有单位
func GetAllUnit(session *JHttp.Session) {
	data := []string{}
	list, err := JRedis.Redis_Sget(Key_Unit)
	if err != nil {
		session.Forward("1", err.Error(), data)
		return
	}
	for _, v := range list {
		data = append(data, string(v.([]byte)))
	}
	session.Forward("0", "success", data)
}
