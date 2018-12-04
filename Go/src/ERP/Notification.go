///*
//@Time : 2018/11/28 15:52
//@Author : mapeng
//@File : notification
//@Software: GoLand
//@Function:
//*/
package main

//
//import (
//	"JGo/JHttp"
//	"JGo/JLogger"
//	"JGo/JUtil"
//	"encoding/json"
//	"net"
//	"strings"
//	"sync"
//	"time"
//)
//
//type ClientInfo struct {
//	Addr     string       //ip
//	Port     string       //port
//	LastTime int64        //
//	Timer    *JUtil.Timer //
//}
//
////检查是否超时
//func (client *ClientInfo) checkTime() bool {
//	diff := time.Now().Sub(time.Unix(client.LastTime, 0))
//	if diff > 3*time.Minute || diff < -3*time.Minute {
//		return false
//	}
//	return true
//}
//
//func (client *ClientInfo) notice(info *NoticeInfo) error {
//	serverAddr := client.Addr + ":" + client.Port
//	conn, err := net.Dial("udp", serverAddr)
//	if err != nil {
//		JLogger.Error("发送数据到%s:%s失败,err:=\n!", client.Addr, client.Port, err.Error())
//		return err
//	}
//	defer conn.Close()
//	b, err := json.Marshal(info)
//	if err == nil {
//		if _, e := conn.Write(b); e != nil {
//			JLogger.Error("发送数据到%s:%s失败,data=%v\n!", client.Addr, client.Port, info)
//			return err
//		}
//		JLogger.Info("发送数据到%s:%s 成功,data=%v\n!", client.Addr, client.Port, info)
//		return nil
//	} else {
//		JLogger.Error("发送数据到%s:%s失败,err:=\n!", client.Addr, client.Port, err.Error())
//	}
//	return nil
//}
//
////超时删除ip
//func timeout(d interface{}) {
//	ip := d.(string)
//	delIP(ip)
//}
//
//func delIP(ip string) {
//	JLogger.Error("delIP=%s, \n", ip)
//	clientsMutex.Lock()
//	defer clientsMutex.Unlock()
//	delete(clients, ip)
//}
//
//func addIP(ip string) {
//	JLogger.Error("addip=%s\n", ip)
//	clientsMutex.RLock()
//	c, ok := clients[ip]
//	clientsMutex.RUnlock()
//	if !ok {
//		JLogger.Error("addip=%s, not   exist\n", ip)
//		info := &ClientInfo{
//			Addr:     ip,
//			Port:     "6543",
//			LastTime: CurStamp(),
//		}
//		clientsMutex.Lock()
//		info.Timer = JUtil.NewTimer(time.Duration(3)*time.Minute, timeout, info.Addr)
//		clients[ip] = info
//		clientsMutex.Unlock()
//	} else {
//		JLogger.Error("addip=%s,exist\n", ip)
//		c.Timer.Reset(time.Duration(3) * time.Minute)
//	}
//}
//
//type NoticeInfo struct {
//	NoticeType int         //012增改删
//	DataType   int         //数据类型
//	Data       interface{} //更新的数据
//	Addr       string      //发送者的IP
//}
//
//var (
//	clients      = make(map[string]*ClientInfo)
//	clientsMutex sync.RWMutex
//	IsNotice     = false //是否开启通知
//)
//
//
//
//func startNotice(isStart bool) {
//	IsNotice = isStart
//}
//

//
/////通知
//func Notice(data *NoticeInfo) {
//	JLogger.Error("1111111111111111111111111")
//	if !IsNotice {
//		JLogger.Error("2222222222")
//		return
//	}
//	JLogger.Error("33333333333333")
//
//	if len(clients) == 0 {
//		JLogger.Error("44444444444444444")
//		return
//	}
//	JLogger.Error("client=%d", len(clients))
//	for _, v := range clients {
//		JLogger.Error("addr=%s\n", v)
//		//if v.Addr == data.Addr {
//		//	JLogger.Error("v.Addr=%s,data.Addr=%s\n", v.Addr, data.Addr)
//		//	continue
//		//}
//		if ok := v.checkTime(); ok {
//			JLogger.Error("55555555555")
//			go v.notice(data)
//		} else {
//			JLogger.Error("6666666666666666")
//			go delIP(v.Addr)
//		}
//	}
//}
