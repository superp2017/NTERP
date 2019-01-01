package main

import (
	"JGo/JHttp"
	"JGo/JStore/JRedis"
	"strconv"
	"strings"
)

type Version struct {
	Version    string
	VersionNum int
	Date       string
	Des        string
	Url        string
}

//获取版本信息
func GetVertion(session *JHttp.Session) {
	st := &Version{}
	if err := JRedis.Redis_get("Version", st); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	session.Forward("0", "sucess", st)
}

//设置版本
func SetVersion(session *JHttp.Session) {
	st := &Version{}
	if err := session.GetPara(st); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	data := &Version{}
	if err := JRedis.Redis_get("Version", data); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	n := version2Num(st.Version)
	if n == -1 || n <= data.VersionNum {
		session.Forward("1", "sucess", nil)
		return
	}
	st.VersionNum = n
	st.Date = CurDate()
	if err := JRedis.Redis_set("Version", st); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	session.Forward("0", "sucess", st)
}

func version2Num(ver string) int {
	list := strings.Split(ver, ".")
	if len(list) == 1 {
		return -1
	}
	i, e := strconv.Atoi(list[0])
	if e != nil {
		return -1
	}
	j, e1 := strconv.Atoi(list[1])
	if e1 != nil {
		return -1
	}
	return i*1000 + j
}
