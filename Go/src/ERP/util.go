package main

import (
	"JsGo/JsStore/JsRedis"
	"JsGo/JsUuid"
	"time"
)

//返回当前时间：例如 2017-02-17 16:33
func CurTime() string {
	return time.Unix(time.Now().Unix(), 0).Format("2006-01-02 15:04:05")
}

//返回当前日期：例如：2017-02-17
func CurDate() string {
	return time.Unix(time.Now().Unix(), 0).Format("2006-01-02")
}

///当前时间的时间戳
func CurStamp() int64 {
	return time.Now().Unix()
}

func IDer(db string) string {
	id := JsUuid.NewV4().String()
	for b, e := JsRedis.Redis_hexists(db, id); b; {
		id = JsUuid.NewV4().String()
		if e != nil {
			return id
		}
	}
	return id
}
