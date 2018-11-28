/*
@Time : 2018/11/28 15:52 
@Author : mapeng
@File : notification
@Software: GoLand
@Function:
*/
package main

import (
	"JGo/JHttp"
	"JGo/JUtil"
	"strings"
	"sync"
	"time"
)

type ClentInfo struct {
	Addr string//ip
	Port int //port
	LastTime int64//
	Timer *JUtil.Timer//
}

//检查是否超时
func (client*ClentInfo) checkTime()bool{
	diff := time.Now().Sub(client.LastTime)
	if diff > time.Hour || diff < -time.Hour {
		return false
	}
	return true
}

//超时删除ip
func timeout(d interface{})  {
	ip:=d.(string)
	delIP(ip)
}

func delIP(ip string)  {
	clientsMutex.Lock()
	defer clientsMutex.Unlock()
	delete(clients,ip)
}

func addIP(ip string)  {
	clientsMutex.Lock()
	defer clientsMutex.Unlock()
	c,ok:=clients[ip]
	if!ok{
		info:=&ClentInfo{
			Addr:ip,
			Port:6666,
			LastTime:CurStamp(),
		}
		clients[ip] =info
		info.Timer = JUtil.NewTimer(time.Duration(1)*time.Hour,timeout,info.Addr)
	}else{
		c.Timer.Reset(time.Duration(1)*time.Hour)
	}
}

type NoticeInfo struct{
	NoticeType int//012增改删
	DataType int//数据类型
	Data interface{}//更新的数据
}


var (
	clients = make(map[string]*ClentInfo)
	clientsMutex sync.Mutex
)

//通知类型
const (
	NoticeType_NEW = 0
	NoticeType_Modify = 1
	NoticeType_Del = 2
)
const(
	STRTUCT_ORDER = 0
	STRUCT_MATERIAL=1
	STRUCT_GOODS =2
	STRUCT_OUTRECORD=3
	STRUCT_USER =4
	STRUCT_CUSTOMER=5
	STRUCT_SUPPLIER =6
)


func HeartBeat(session *JHttp.Session)  {
	ip:=strings.Split(session.Req.RemoteAddr,":")[0]
	addIP(ip)
	session.Forward("0","success",nil)
}


func notice(data NoticeInfo)  {
	
}