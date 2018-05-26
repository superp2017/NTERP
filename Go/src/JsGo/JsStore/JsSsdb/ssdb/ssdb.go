// Copyright 20 The Go Authors. All rights reserved.
// Author : mengzhaofeng
// Create Date : 2015/11/05
//
// 功能：　vgssdb 是对ssdb的一层封装，用于业务、配置数据的缓存，提供kv,hashmap,zset等功能
package ssdb

import (
	"errors"
	"fmt"
	_ "log"
	"sync"

	"JsLib/JsConfig"
	. "JsLib/JsLogger"

	"github.com/seefan/gossdb"
)

//st_SsdbConn 内部维护ssdb主机的name, ip, port, 链接句柄conn等信息
type st_SsdbConn struct {
	conn *gossdb.Connectors
	name string
	ip   string
	port int
}

type st_SsdbSlice struct {
	ssdb  []*st_SsdbConn
	index int
	n     int
}

// ssdb 主机列表缓存
var g_DbCache map[string]*st_SsdbSlice
var g_lock sync.Mutex

//　关闭ssdb 缓存
func Close() {
	for _, v := range g_DbCache {
		if v.ssdb != nil {
			for _, x := range v.ssdb {
				x.conn.Close()
			}
		}
	}
}

func init() {

	g_DbCache = make(map[string]*st_SsdbSlice)

	for _, v := range JsConfig.CFG.Ssdb {
		conn := &st_SsdbConn{}
		pool, err := gossdb.NewPool(&gossdb.Config{
			Host:             v.Ip,
			Port:             v.Port,
			MinPoolSize:      5,
			MaxPoolSize:      50,
			AcquireIncrement: 5,
		})
		if err != nil {

			conn.conn = nil
		}

		conn.conn = pool
		conn.ip = v.Ip
		conn.port = v.Port
		conn.name = v.Name
		// var ok bool
		// if v.Logger == "" {
		// 	conn.logger, ok = JsLogger.GetComLogger()
		// 	if !ok {
		// 		log.Fatalf("init logger error\n")
		// 	}
		// } else {
		// 	conn.logger, ok = JsLogger.GetLogger("./log/" + v.Logger)
		// 	if !ok {
		// 		log.Fatalf("init logger[%s] error\n", v.Logger)
		// 	}
		// }

		ary, ok := g_DbCache[v.Name]

		if ok {
			ary.ssdb = append(ary.ssdb, conn)
			ary.n = ary.n + 1
			g_DbCache[v.Name] = ary

		} else {
			slice := &st_SsdbSlice{make([]*st_SsdbConn, 1), 0, 1}
			slice.ssdb[0] = conn
			g_DbCache[v.Name] = slice

		}

	}

}

// 获取　key 对应的　value
func get(db, key string) (gossdb.Value, error) {
	var e error = nil
	var c *gossdb.Client
	var value gossdb.Value

	g_lock.Lock()
	defer g_lock.Unlock()
	v, ok := g_DbCache[db]

	if ok && v.ssdb != nil {

		conn := v.ssdb[v.index]
		x := v.index + 1
		v.index = x % v.n
		g_DbCache[db] = v

		c, e = conn.conn.NewClient()

		if e != nil {
			Warn("ssdb[%s%] error: %s\n", conn.ip, e.Error())
			return value, e
		}
		value, e = c.Get(key)
		c.Close()
		if e != nil {
			Warn("ssdb[%s%] Get [%s] error: %s\n", conn.ip, key, e.Error())
			return value, e
		}
		if value.IsEmpty() {
			Warn("key[%s] is not exits", key)
			return value, errors.New(fmt.Sprintf("key[%s] is not exits", key))
		}
		return value, e
	}

	Warn("key[%s] is not exits", key)
	return value, errors.New(fmt.Sprintf("key[%s] is not exits", key))
}

// 设置 key 对应的　value
func set(db, key string, val interface{}, ttl ...int64) (err error) {
	var e error = nil
	var c *gossdb.Client
	g_lock.Lock()
	defer g_lock.Unlock()
	v, ok := g_DbCache[db]

	if ok && v.ssdb != nil {

		conn := v.ssdb[v.index]
		x := v.index + 1
		v.index = x % v.n

		g_DbCache[db] = v

		c, e = conn.conn.NewClient()

		if e != nil {
			Warn("ssdb[%s%] error: %s\n", conn.ip, e.Error())
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

			Warn("ssdb[%s%] Set[%s:%v] error: %s\n", conn.ip, key, val, e.Error())
			return e
		}

		return e
	}

	return e
}

// 获取哈希setName ，key对应的value
func hget(db, setName, key string) (value gossdb.Value, err error) {
	var e error = nil
	var c *gossdb.Client
	var ret gossdb.Value

	g_lock.Lock()
	defer g_lock.Unlock()
	v, ok := g_DbCache[db]

	if ok && v.ssdb != nil {

		conn := v.ssdb[v.index]
		x := v.index + 1
		v.index = x % v.n
		g_DbCache[db] = v

		c, e = conn.conn.NewClient()
		//			Info("found the connection")
		//			fmt.Printf("found the connection")
		if e != nil {
			Warn("ssdb[%s] error: %s\n", conn.ip, e.Error())
			return value, e
		}

		ret, e = c.Hget(setName, key)
		c.Close()

		if e != nil {

			Warn("ssdb[%s] Hget [%s, %s] error: %s\n", conn.ip, setName, key, e.Error())
			return value, e
		}

		if ret.IsEmpty() {
			Warn("set[%s], key[%s] is not exits", setName, key)
			return ret, errors.New(fmt.Sprintf("set[%s], key[%s] is not exits", setName, key))
		}

		return ret, e
	}

	Warn("set[%s], key[%s] is not exits", setName, key)
	return ret, errors.New(fmt.Sprintf("set[%s], key[%s] is not exits", setName, key))
}

// 设置哈希setName，key 对应的value
func hset(db, setName, key string, value interface{}) (err error) {
	var e error = nil
	var c *gossdb.Client

	g_lock.Lock()
	defer g_lock.Unlock()
	v, ok := g_DbCache[db]
	if ok && v.ssdb != nil {

		conn := v.ssdb[v.index]
		x := v.index + 1
		v.index = x % v.n
		g_DbCache[db] = v

		c, e = conn.conn.NewClient()
		if e != nil {
			Warn("ssdb[%s] error: %s\n", conn.ip, e.Error())
			return e
		}
		//			Info("found the connection and the dbname=%s and the key=%s \n",setName,key)
		e = c.Hset(setName, key, value)
		c.Close()
		if e != nil {
			//Warn("ssdb[%s] Hset[%s,%s:%v] error: %s\n", v.ip, setName, key, value, e.Error())
			Warn("ssdb[%s] Hset[%s,%s] error: %s\n", conn.ip, setName, key, e.Error())
			return e
		}
		return e
	}

	return e
}

// 判断缓存中是否存在key
func exists(db, key string) (re bool, err error) {
	var e error = nil
	var c *gossdb.Client

	g_lock.Lock()
	defer g_lock.Unlock()
	v, ok := g_DbCache[db]

	if ok && v.ssdb != nil {

		conn := v.ssdb[v.index]
		x := v.index + 1
		v.index = x % v.n
		g_DbCache[db] = v

		c, e = conn.conn.NewClient()
		if e != nil {
			Warn("ssdb[%s] error: %s\n", conn.ip, e.Error())
			return false, e
		}

		var ok bool
		ok, e = c.Exists(key)
		c.Close()

		if ok {
			return true, nil
		}
	}

	return false, e
}

// 设置缓存中的key超时时间ttl
func expire(db, key string, ttl int64) (re bool, err error) {
	var e error = nil
	var c *gossdb.Client

	g_lock.Lock()
	defer g_lock.Unlock()
	v, ok := g_DbCache[db]

	if ok && v.ssdb != nil {

		conn := v.ssdb[v.index]
		x := v.index + 1
		v.index = x % v.n
		g_DbCache[db] = v

		c, e = conn.conn.NewClient()
		if e != nil {
			Warn("ssdb[%s] error: %s\n", conn.ip, e.Error())
			return false, e
		}

		var ok bool
		ok, e = c.Expire(key, ttl)
		c.Close()

		if ok {
			return true, nil
		}
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
// 			Warn("ssdb[%s] error: %s\n", v.ip, e.Error())
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
func del(db, key string) error {
	var e error = nil
	var c *gossdb.Client

	g_lock.Lock()
	defer g_lock.Unlock()
	v, ok := g_DbCache[db]

	if ok && v.ssdb != nil {

		conn := v.ssdb[v.index]
		x := v.index + 1
		v.index = x % v.n
		g_DbCache[db] = v

		c, e = conn.conn.NewClient()
		if e != nil {
			Warn("ssdb[%s] error: %s\n", conn.ip, e.Error())
			return e
		}

		e = c.Del(key)
		c.Close()

		if e != nil {
			Warn("ssdb[%s] Del[%s] error: %s\n", conn.ip, key, e.Error())
			return e
		}

		return e
	}

	return e
}

//　删除ssdb中的哈希setName
func hdel(db, setName, key string) (err error) {
	var e error = nil
	var c *gossdb.Client

	g_lock.Lock()
	defer g_lock.Unlock()
	v, ok := g_DbCache[db]

	if ok && v.ssdb != nil {

		conn := v.ssdb[v.index]
		x := v.index + 1
		v.index = x % v.n
		g_DbCache[db] = v

		c, e = conn.conn.NewClient()
		if e != nil {
			Warn("ssdb[%s] error: %s\n", conn.ip, e.Error())
			return e
		}

		e = c.Hdel(setName, key)
		c.Close()

		if e != nil {
			Warn("ssdb[%s] Del[%s] error: %s\n", conn.ip, key, e.Error())
			return e
		}

		return e
	}

	return e
}

// 判断哈希setName中是否存在 key
func hexists(db, setName, key string) (re bool, err error) {
	var e error = nil
	var c *gossdb.Client

	g_lock.Lock()
	defer g_lock.Unlock()
	v, ok := g_DbCache[db]

	if ok && v.ssdb != nil {

		conn := v.ssdb[v.index]
		x := v.index + 1
		v.index = x % v.n
		g_DbCache[db] = v

		c, e = conn.conn.NewClient()
		if e != nil {
			Warn("ssdb[%s] error: %s\n", conn.ip, e.Error())
			return false, e
		}

		var ok bool
		ok, e = c.Hexists(setName, key)
		c.Close()

		if ok {
			return true, nil
		}
	}

	return false, e
}

////获取一个所有表key
func keys(db, start, end string, limit int64) (s []string, ee error) {
	var e error = nil
	var c *gossdb.Client
	g_lock.Lock()
	defer g_lock.Unlock()
	v, ok := g_DbCache[db]

	if ok && v.ssdb != nil {

		conn := v.ssdb[v.index]
		x := v.index + 1
		v.index = x % v.n
		g_DbCache[db] = v

		c, e = conn.conn.NewClient()
		if e != nil {
			Warn("ssdb[%s]  Keys error: %s\n", conn.ip, e.Error())
			return nil, e
		}
		keys, err := c.Keys(start, end, limit)
		c.Close()
		if err != nil {
			Warn("ssdb[%s] start [%s] end[%s]limit[%d]error: %s\n", conn.ip, start, end, limit, err.Error())
			return nil, e
		}
		return keys, err
	}

	return s, errors.New(fmt.Sprintf("ssdb Keys ,start [%s] end[%s]limit[%d]is not exits\n", start, end, limit))
}

// 获取哈希setName所有的结果
func MultiHgetAll(db, setName string) (val map[string]gossdb.Value, err error) {

	g_lock.Lock()
	defer g_lock.Unlock()
	v, ok := g_DbCache[db]

	if ok && v.ssdb != nil {

		conn := v.ssdb[v.index]
		x := v.index + 1
		v.index = x % v.n
		g_DbCache[db] = v

		c, e := conn.conn.NewClient()
		if e != nil {
			Warn("ssdb[%s] error: %s\n", conn.ip, e.Error())
			return nil, e
		}

		val, e = c.MultiHgetAll(setName)
		c.Close()

		if e != nil {

			Warn("ssdb[%s] MultiHgetAll [%s] error: %s\n", conn.ip, setName, e.Error())
			return val, e
		}

		return val, e
	}

	Warn("set[%s] is not exits", setName)
	return nil, errors.New(fmt.Sprintf("set[%s] is not exits", setName))
}

// 删除setName中的所有key
func hclear(db, setName string) (err error) {
	var e error = nil

	g_lock.Lock()
	defer g_lock.Unlock()
	v, ok := g_DbCache[db]

	if ok && v.ssdb != nil {

		conn := v.ssdb[v.index]
		x := v.index + 1
		v.index = x % v.n
		g_DbCache[db] = v

		c, e := conn.conn.NewClient()
		if e != nil {
			Warn("ssdb[%s] error: %s\n", conn.ip, e.Error())
			return e
		}

		e = c.Hclear(setName)
		c.Close()

		if e != nil {
			Warn("ssdb[%s] Clear hashmap[%s] error: %s\n", conn.ip, setName, e.Error())
			return e
		}

		return e
	}

	return e
}

// 批量获取哈希setName的结果
func multiHget(db, setName string, key ...string) (val map[string]gossdb.Value, err error) {

	g_lock.Lock()
	defer g_lock.Unlock()
	v, ok := g_DbCache[db]

	if ok && v.ssdb != nil {

		conn := v.ssdb[v.index]
		x := v.index + 1
		v.index = x % v.n
		g_DbCache[db] = v

		c, e := conn.conn.NewClient()
		if nil != e {
			Warn("ssdb[%s] error: %s\n", conn.ip, err.Error())
			return nil, err
		}

		val, err = c.MultiHget(setName, key...)
		c.Close()

		if nil != err {
			Warn("ssdb[%s] MultiHget [%s] key=[%s]error: %s\n", conn.ip, setName, key, err.Error())
			return nil, err
		}

		return val, err
	}

	Warn("set[%s] is not exits", setName)
	return nil, errors.New(fmt.Sprintf("set[%s] is not exits", setName))
}

// 批量设置哈希setName的结果
func multiHset(db, setName string, kvs map[string]interface{}) (err error) {

	g_lock.Lock()
	defer g_lock.Unlock()
	v, ok := g_DbCache[db]

	if ok && v.ssdb != nil {

		conn := v.ssdb[v.index]
		x := v.index + 1
		v.index = x % v.n
		g_DbCache[db] = v

		c, e := conn.conn.NewClient()
		if nil != e {
			Warn("ssdb[%s] error: %s\n", conn.ip, err.Error())
			return err
		}

		err = c.MultiHset(setName, kvs)
		c.Close()

		if nil != err {
			Warn("ssdb[%s] MultiHset [%s] key=[%s]error: %s\n", conn.ip, setName, kvs, err.Error())
			return err
		}

		return nil
	}

	Warn("set[%s] is not exits", setName)
	return errors.New(fmt.Sprintf("set[%s] is not exits", setName))
}

func traversal(db, setName string) (val map[string]gossdb.Value, err error) {

	g_lock.Lock()
	defer g_lock.Unlock()
	v, ok := g_DbCache[db]

	if ok && v.ssdb != nil {

		conn := v.ssdb[v.index]
		x := v.index + 1
		v.index = x % v.n
		g_DbCache[db] = v

		c, e := conn.conn.NewClient()
		if nil != e {
			Warn("ssdb[%s] error: %s\n", conn.ip, err.Error())
			return nil, err
		}

		val, err = c.MultiHgetAll(setName)
		c.Close()

		if nil != err {
			Warn("ssdb[%s] set[%s] Traversal error: %s\n", conn.ip, setName, err.Error())
			return nil, err
		}

		return val, nil
	}

	Warn("set[%s] is not exits", setName)
	return nil, errors.New(fmt.Sprintf("set[%s] is not exits", setName))
}
