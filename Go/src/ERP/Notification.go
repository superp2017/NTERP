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
	"JGo/JLogger"
	"JGo/JUtil"
	"encoding/json"
	"net"
	"strings"
	"sync"
	"time"
)

type ClientInfo struct {
	Addr string//ip
	Port string //port
	LastTime int64//
	Timer *JUtil.Timer//
}

//检查是否超时
func (client*ClientInfo) checkTime()bool{
	diff := time.Now().Sub(time.Unix(client.LastTime, 0))
	if diff > time.Hour || diff < -time.Hour {
		return false
	}
	return true
}

func (client*ClientInfo) notice(info *NoticeInfo)error{
	serverAddr := client.Addr + ":" +  client.Port
	conn, err := net.Dial("udp", serverAddr)
	if err!=nil{
		JLogger.Error("发送数据到%s:%s失败,err:=\n!",client.Addr,client.Port,err.Error())
		return err
	}
	defer conn.Close()
	b,err:=json.Marshal(info)
	if err==nil{
		if _, e := conn.Write(b);e!=nil{
			JLogger.Error("发送数据到%s:%s失败,data=%v\n!",client.Addr,client.Port,info)
			return err
		}
		JLogger.Info("发送数据到%s:%s 成功,data=%v\n!",client.Addr,client.Port,info)
		return nil
	}else{
		JLogger.Error("发送数据到%s:%s失败,err:=\n!",client.Addr,client.Port,err.Error())
	}
	return nil
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
	clientsMutex.RLock()
	c,ok:=clients[ip]
	if!ok{
		clientsMutex.RUnlock()
		info:=&ClientInfo{
			Addr:ip,
			Port:"6543",
			LastTime:CurStamp(),
		}
		clientsMutex.Lock()
		info.Timer = JUtil.NewTimer(time.Duration(1)*time.Hour,timeout,info.Addr)
		clients[ip] =info
		clientsMutex.Unlock()
	}else{
		clientsMutex.RUnlock()
		c.Timer.Reset(time.Duration(1)*time.Hour)
	}
}

type NoticeInfo struct{
	NoticeType int//012增改删
	DataType int//数据类型
	Data interface{}//更新的数据
}

var (
	clients = make(map[string]*ClientInfo)
	clientsMutex sync.RWMutex
)

//通知类型
const (
	NoticeType_NEW 		= 0//新建
	NoticeType_Modify 	= 1//修改
	NoticeType_Del 		= 2//删除
)
//更新的数据类型
const(
	STRTUCT_ORDER 		= 0//订单
	STRUCT_MATERIAL		= 1//物料
	STRUCT_GOODS 		= 2///商品
	STRUCT_OUTRECORD	= 3///出库记录
	STRUCT_USER 		= 4//员工
	STRUCT_CUSTOMER		= 5//客户
	STRUCT_SUPPLIER 	= 6//供应商
	STRUCT_UNIT 		= 7//单位
	STRUCT_DEPARTMENT 	= 8//部门
	STRUCT_GOODS_TYPE 	= 9//商品的分类
	STRUCT_PLANTING 	= 10//镀种
)


func HeartBeat(session *JHttp.Session)  {
	ip:=strings.Split(session.Req.RemoteAddr,":")[0]
	addIP(ip)
	session.Forward("0","success",nil)
}


func Notice(data *NoticeInfo)  {
	clientsMutex.RLock()
	for _,v:= range clients{
		clientsMutex.RUnlock()
		if ok:=v.checkTime();ok{
			go v.notice(data)
		}else{
			go delIP(v.Addr)
		}
	}
}