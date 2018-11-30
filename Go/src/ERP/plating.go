package main

import (
	"JGo/JHttp"
	"JGo/JLogger"
	"JGo/JStore/JRedis"
)

//新增一个镀种
func NewPlating(session *JHttp.Session) {
	type Para struct {
		Plating string
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.Plating == "" {
		session.Forward("1", "NewPlating:Plating is empty\n", nil)
		return
	}
	if err := JRedis.Redis_Sset(Key_Plating, st.Plating); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	////通知
	//go	Notice(&NoticeInfo{
	//	NoticeType:NoticeType_NEW,
	//	DataType:STRUCT_PLANTING,
	//	Data:st.Plating,
	//})
	session.Forward("0", "success", st.Plating)
}

//删除一个镀种
func DelPlating(session *JHttp.Session) {
	type Para struct {
		Plating string
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.Plating == "" {
		session.Forward("1", "DelPlating:Plating is empty\n", nil)
		return
	}
	if err := JRedis.Redis_Sdel(Key_Plating, st.Plating); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	////通知
	//go	Notice(&NoticeInfo{
	//	NoticeType:NoticeType_Del,
	//	DataType:STRUCT_PLANTING,
	//	Data:st.Plating,
	//})
	session.Forward("0", "success", st.Plating)
}

//获取所有镀种
func GetAllPlating(session *JHttp.Session) {
	data := []string{}
	list, err := JRedis.Redis_Sget(Key_Plating)
	if err != nil {
		session.Forward("1", err.Error(), data)
		return
	}
	for _, v := range list {
		data = append(data, string(v.([]byte)))
	}
	session.Forward("0", "success", data)
}
