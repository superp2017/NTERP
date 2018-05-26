// Copyright 20 The Go Authors. All rights reserved.
// Author : mengzhaofeng
// Create Date : 2015/11/05
//
// 功能：　vgssdb 是对ssdb的一层封装，用于业务、配置数据的缓存，提供kv,hashmap,zset等功能
package main

import (
	"errors"
	"fmt"
	"log"
	"strconv"
	"sync"

	"JsGo/JsConfig"
	. "JsGo/JsLogger"

	"github.com/seefan/gossdb"
)

//st_SsdbConn 内部维护ssdb主机的name, ip, port, 链接句柄conn等信息
type st_SsdbConn struct {
	conn *gossdb.Connectors
	name string
	ip   string
	port int
}

// ssdb 主机列表缓存
// var g_DbCache map[string]*st_SsdbSlice
var g_conn *st_SsdbConn
var g_lock sync.Mutex

//　关闭ssdb 缓存
func Close() {
	g_conn.conn.Close()
}

func init() {

	ssdb, e := JsConfig.GetConfigMap([]string{"Ssdb"})
	if e != nil {
		log.Fatalln(e.Error())
	}
	// for _, v := range JsConfig.CFG.Ssdb {
	g_conn = &st_SsdbConn{}
	port, e := strconv.Atoi(ssdb["Port"])
	if e != nil {
		log.Fatalln(e.Error())
	}
	pool, err := gossdb.NewPool(&gossdb.Config{
		Host:             ssdb["Ip"],
		Port:             port,
		MinPoolSize:      5,
		MaxPoolSize:      50,
		AcquireIncrement: 5,
	})
	if err != nil {

		g_conn.conn = nil

		Error("Ip = %s, Port = %d, conn.conn is nil!\n", ssdb["Ip"], port)
		log.Fatalf("Ip = %s, Port = %d, conn.conn is nil!\n", ssdb["Ip"], port)
	}

	g_conn.conn = pool
	g_conn.ip = ssdb["Ip"]
	g_conn.port = port
	g_conn.name = ssdb["Name"]

}

// 获取　key 对应的　value
func get(key string) (gossdb.Value, error) {
	var e error = nil
	var c *gossdb.Client
	var value gossdb.Value

	c, e = g_conn.conn.NewClient()

	if e != nil {
		Error("ssdb[%s%] error: %s\n", g_conn.ip, e.Error())
		return value, e
	}
	value, e = c.Get(key)
	c.Close()
	if e != nil {
		Error("ssdb[%s%] Get [%s] error: %s\n", g_conn.ip, key, e.Error())
		return value, e
	}
	if value.IsEmpty() {
		Error("key[%s] is not exits", key)
		return value, errors.New(fmt.Sprintf("key[%s] is not exits", key))
	}
	return value, e
}

// 设置 key 对应的　value
func set(key string, val interface{}, ttl ...int64) (err error) {
	var e error = nil
	var c *gossdb.Client

	if e != nil {
		Error("ssdb[%s%] error: %s\n", g_conn.ip, e.Error())
		return e
	}

	if len(ttl) > 0 {
		ivl := ttl[0]
		e = c.Set(key, val, ivl)
	} else {
		e = c.Set(key, val)
	}
	c.Close()

	if e != nil {

		Error("ssdb[%s%] Set[%s:%v] error: %s\n", g_conn.ip, key, val, e.Error())
		return e
	}

	return e
}

// 获取哈希setName ，key对应的value
func hget(setName, key string) (value gossdb.Value, err error) {
	var e error = nil
	var c *gossdb.Client
	var ret gossdb.Value

	c, e = g_conn.conn.NewClient()
	//			Info("found the connection")
	//			fmt.Printf("found the connection")
	if e != nil {
		Error("ssdb[%s] error: %s\n", g_conn.ip, e.Error())
		return value, e
	}

	ret, e = c.Hget(setName, key)
	c.Close()

	if e != nil {

		Error("ssdb[%s] Hget [%s, %s] error: %s\n", g_conn.ip, setName, key, e.Error())
		return value, e
	}

	if ret.IsEmpty() {
		Error("set[%s], key[%s] is not exits", setName, key)
		return ret, errors.New(fmt.Sprintf("set[%s], key[%s] is not exits", setName, key))
	}

	return ret, e

}

// 设置哈希setName，key 对应的value
func hset(setName, key string, value interface{}) (err error) {
	var e error = nil
	var c *gossdb.Client

	c, e = g_conn.conn.NewClient()
	if e != nil {
		Error("ssdb[%s] error: %s\n", g_conn.ip, e.Error())
		return e
	}
	//			Info("found the connection and the dbname=%s and the key=%s \n",setName,key)
	e = c.Hset(setName, key, value)
	c.Close()
	if e != nil {
		//Error("ssdb[%s] Hset[%s,%s:%v] error: %s\n", v.ip, setName, key, value, e.Error())
		Error("ssdb[%s] Hset[%s,%s] error: %s\n", g_conn.ip, setName, key, e.Error())
		return e
	}
	return e

}

// 获取setName的size
func hsize(setName string) int64 {
	var e error = nil
	var c *gossdb.Client

	c, e = g_conn.conn.NewClient()
	if e != nil {
		Error("ssdb[%s] error: %s\n", g_conn.ip, e.Error())
		return -1
	}
	//			Info("found the connection and the dbname=%s and the key=%s \n",setName,key)
	s, e := c.Hsize(setName)
	c.Close()
	if e != nil {
		//Error("ssdb[%s] Hset[%s,%s:%v] error: %s\n", v.ip, setName, key, value, e.Error())
		Error("ssdb[%s] Hsize[%s] error: %s\n", g_conn.ip, setName, e.Error())
		return -1
	}
	return s

}

// 获取setName的keys
func hkeys(setName, begin, end string, limit int64) []string {
	var e error = nil
	var c *gossdb.Client

	c, e = g_conn.conn.NewClient()
	if e != nil {
		Error("ssdb[%s] error: %s\n", g_conn.ip, e.Error())
		return nil
	}
	//			Info("found the connection and the dbname=%s and the key=%s \n",setName,key)
	s, e := c.Hkeys(setName, begin, end, limit)
	c.Close()
	if e != nil {
		//Error("ssdb[%s] Hset[%s,%s:%v] error: %s\n", v.ip, setName, key, value, e.Error())
		Error("ssdb[%s] Hsize[%s] error: %s\n", g_conn.ip, setName, e.Error())
		return nil
	}
	return s

}

// 判断缓存中是否存在key
func exists(key string) bool {
	var e error = nil
	var c *gossdb.Client

	c, e = g_conn.conn.NewClient()
	if e != nil {
		Error("ssdb[%s] error: %s\n", g_conn.ip, e.Error())
		return false
	}

	var ok bool
	ok, e = c.Exists(key)
	c.Close()

	if ok {
		return true
	}

	return false
}

// 设置缓存中的key超时时间ttl
func expire(key string, ttl int64) (re bool, err error) {
	var e error = nil
	var c *gossdb.Client

	c, e = g_conn.conn.NewClient()
	if e != nil {
		Error("ssdb[%s] error: %s\n", g_conn.ip, e.Error())
		return false, e
	}

	var ok bool
	ok, e = c.Expire(key, ttl)
	c.Close()

	if ok {
		return true, nil
	}

	return false, e
}

// 通用的ssdb 命令接口
// func Do(args ...interface{}) ([]string, error) {
// 	var e error = nil
// 	var c *gossdb.Client
// 	var ret []string

// 	v, ok := g_DbCache[db]
// 	if ok && v.conn != nil {
// 		c, e = v.conn.NewClient()
// 		if e != nil {
// 			Error("ssdb[%s] error: %s\n", v.ip, e.Error())
// 			return value, e
// 		}

// 		ret, e = c.Do(args)
// 		c.Close()

// 		if e != nil {
// 			return value, e
// 		}

// 		return ret, e
// 	}

// 	return ret, e
// }

// 删除ssdb中的key
func del(key string) error {
	var e error = nil
	var c *gossdb.Client

	c, e = g_conn.conn.NewClient()
	if e != nil {
		Error("ssdb[%s] error: %s\n", g_conn.ip, e.Error())
		return e
	}

	e = c.Del(key)
	c.Close()

	if e != nil {
		Error("ssdb[%s] Del[%s] error: %s\n", g_conn.ip, key, e.Error())
		return e
	}

	return e

}

//　删除ssdb中的哈希setName
func hdel(setName, key string) (err error) {
	var e error = nil
	var c *gossdb.Client

	c, e = g_conn.conn.NewClient()
	if e != nil {
		Error("ssdb[%s] error: %s\n", g_conn.ip, e.Error())
		return e
	}

	e = c.Hdel(setName, key)
	c.Close()

	if e != nil {
		Error("ssdb[%s] Del[%s] error: %s\n", g_conn.ip, key, e.Error())
		return e
	}

	return e

}

// 判断哈希setName中是否存在 key
func hexists(table, key string) bool {
	var e error = nil
	var c *gossdb.Client

	c, e = g_conn.conn.NewClient()
	if e != nil {
		Error("ssdb[%s] error: %s\n", g_conn.ip, e.Error())
		return false
	}

	var ok bool
	ok, e = c.Hexists(table, key)
	c.Close()

	if ok {
		return true
	}

	return false
}

////获取一个所有表key
func keys(start, end string, limit int64) (s []string, ee error) {
	var e error = nil
	var c *gossdb.Client

	c, e = g_conn.conn.NewClient()
	if e != nil {
		Error("ssdb[%s]  Keys error: %s\n", g_conn.ip, e.Error())
		return nil, e
	}
	keys, err := c.Keys(start, end, limit)
	c.Close()
	if err != nil {
		Error("ssdb[%s] start [%s] end[%s]limit[%d]error: %s\n", g_conn.ip, start, end, limit, err.Error())
		return nil, e
	}
	return keys, err

}

// 获取哈希setName所有的结果
func multiHgetAll(setName string) (val map[string]gossdb.Value, err error) {

	c, e := g_conn.conn.NewClient()
	if e != nil {
		Error("ssdb[%s] error: %s\n", g_conn.ip, e.Error())
		return nil, e
	}

	val, e = c.MultiHgetAll(setName)
	c.Close()

	if e != nil {

		Error("ssdb[%s] MultiHgetAll [%s] error: %s\n", g_conn.ip, setName, e.Error())
		return val, e
	}

	return val, e

}

// 删除setName中的所有key
func hclear(setName string) (err error) {
	var e error = nil

	c, e := g_conn.conn.NewClient()
	if e != nil {
		Error("ssdb[%s] error: %s\n", g_conn.ip, e.Error())
		return e
	}

	e = c.Hclear(setName)
	c.Close()

	if e != nil {
		Error("ssdb[%s] Clear hashmap[%s] error: %s\n", g_conn.ip, setName, e.Error())
		return e
	}

	return e

}

// 批量获取哈希setName的结果
func multiHget(setName string, key ...string) (val map[string]gossdb.Value, err error) {

	c, e := g_conn.conn.NewClient()
	if nil != e {
		Error("ssdb[%s] error: %s\n", g_conn.ip, err.Error())
		return nil, err
	}

	val, err = c.MultiHget(setName, key...)
	c.Close()

	if nil != err {
		Error("ssdb[%s] MultiHget [%s] key=[%s]error: %s\n", g_conn.ip, setName, key, err.Error())
		return nil, err
	}

	return val, err

}

// 批量设置哈希setName的结果
func multiHset(setName string, kvs map[string]interface{}) (err error) {

	c, e := g_conn.conn.NewClient()
	if nil != e {
		Error("ssdb[%s] error: %s\n", g_conn.ip, err.Error())
		return err
	}

	err = c.MultiHset(setName, kvs)
	c.Close()

	if nil != err {
		Error("ssdb[%s] MultiHset [%s] key=[%s]error: %s\n", g_conn.ip, setName, kvs, err.Error())
		return err
	}

	return nil

}

func traversal(setName string) (val map[string]gossdb.Value, err error) {

	c, e := g_conn.conn.NewClient()
	if nil != e {
		Error("ssdb[%s] error: %s\n", g_conn.ip, err.Error())
		return nil, err
	}

	val, err = c.MultiHgetAll(setName)
	c.Close()

	if nil != err {
		Error("ssdb[%s] set[%s] Traversal error: %s\n", g_conn.ip, setName, err.Error())
		return nil, err
	}

	return val, nil

}
