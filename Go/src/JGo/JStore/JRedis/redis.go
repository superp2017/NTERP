package JRedis

import (
	"JGo/JConfig"
	. "JGo/JLogger"
	"encoding/json"
	"errors"
	"log"
	"time"

	"github.com/gomodule/redigo/redis"
)

var g_pool *redis.Pool

func init() {

	redisCfg, e := JConfig.GetConfigMap([]string{"Redis"})
	if e != nil {
		log.Fatalln(e.Error())
	}

	g_pool = &redis.Pool{
		MaxIdle:     3,
		IdleTimeout: 240 * time.Second,
		Dial: func() (redis.Conn, error) {
			c, err := redis.Dial("tcp", redisCfg["Ip"]+":"+redisCfg["Port"])
			if err != nil {
				return nil, err
			}
			if _, err := c.Do("AUTH", redisCfg["Password"]); err != nil {
				c.Close()
				return nil, err
			}
			return c, nil
		},
		Wait: true}
}

func Redis_expire(k string, v interface{}, t int) error {
	c := g_pool.Get()
	if c == nil {
		Error("connect is nil")
		return errors.New("connect is nil")
	}

	b, e := json.Marshal(v)
	if e != nil {
		Error(e.Error())
		return e
	}

	c.Send("SELECT", 0)
	c.Send("SET", k, b)
	c.Send("EXPIRE", k, t)
	c.Flush()
	c.Receive()
	c.Receive()

	_, e = redis.Bytes(c.Receive())
	if e != nil {
		Error(e.Error())
		return e
	}

	return nil
}

func Redis_get(k string, v interface{}) error {

	c := g_pool.Get()
	if c == nil {
		Error("connect is nil")
		return errors.New("connect is nil")
	}

	c.Send("SELECT", 0)
	c.Send("GET", k)
	c.Flush()
	c.Receive()

	b, e := redis.Bytes(c.Receive())
	if e != nil {
		Error(e.Error())
		return e
	}

	e = json.Unmarshal(b, v)
	if e != nil {
		Error(e.Error())
		return e
	}

	return nil
}

func Redis_set(k string, v interface{}) error {
	c := g_pool.Get()
	if c == nil {
		Error("connect is nil")
		return errors.New("connect is nil")
	}

	b, e := json.Marshal(v)
	if e != nil {
		Error(e.Error())
		return e
	}

	c.Send("SELECT", 0)
	c.Send("SET", k, b)
	c.Flush()
	c.Receive()
	_, e = c.Receive()
	if e != nil {
		Error(e.Error())
		return e
	}
	return nil
}

func Redis_exists(k string) (bool, error) {
	c := g_pool.Get()
	if c == nil {
		Error("connect is nil")
		return false, errors.New("connect is nil")
	}

	c.Send("SELECT", 0)
	c.Send("EXISTS", k)
	c.Flush()
	c.Receive()
	b, e := redis.Bool(c.Receive())
	if e != nil {
		Error(e.Error())
		return false, e
	}
	return b, nil
}

func Redis_del(k string) error {
	c := g_pool.Get()
	if c == nil {
		Error("connect is nil")
		return errors.New("connect is nil")
	}

	c.Send("SELECT", 0)
	c.Send("DEL", k)
	c.Flush()
	c.Receive()
	_, e := c.Receive()
	if e != nil {
		Error(e.Error())
		return e
	}
	return nil
}

//读取数据库
func Redis_hget(t, k string, v interface{}) error {
	c := g_pool.Get()
	if c == nil {
		Error("connect is nil")
		return errors.New("connect is nil")
	}
	c.Send("SELECT", 1)
	c.Send("HGET", t, k)
	c.Flush()
	c.Receive()
	b, e := redis.Bytes(c.Receive())
	if e != nil {
		Error(e.Error())
		return e
	}
	e = json.Unmarshal(b, v)
	if e != nil {
		Error(e.Error())
		return e
	}
	return nil
}

//写入数据库
func Redis_hset(t, k string, v interface{}) error {
	c := g_pool.Get()
	if c == nil {
		Error("connect is nil")
		return errors.New("connect is nil")
	}

	b, e := json.Marshal(v)
	if e != nil {
		Error(e.Error())
		return e
	}

	c.Send("SELECT", 1)
	c.Send("HSET", t, k, b)
	c.Flush()
	c.Receive()
	_, e = c.Receive()
	if e != nil {
		Error(e.Error())
		return e
	}
	return nil
}

//从数据库查找是否存在
func Redis_hexists(t, k string) (bool, error) {
	c := g_pool.Get()
	if c == nil {
		Error("connect is nil")
		return false, errors.New("connect is nil")
	}

	c.Send("SELECT", 1)
	c.Send("HEXISTS", t, k)
	c.Flush()
	c.Receive()
	b, e := redis.Bool(c.Receive())
	if e != nil {
		Error(e.Error())
		return false, e
	}

	return b, nil
}

func Redis_hsize(t string) (int64, error) {
	c := g_pool.Get()
	if c == nil {
		Error("connect is nil")
		return -1, errors.New("connect is nil")
	}

	c.Send("SELECT", 1)
	c.Send("HLEN", t)
	c.Flush()
	c.Receive()
	b, e := redis.Int64(c.Receive())
	if e != nil {
		Error(e.Error())
		return -1, e
	}

	return b, nil
}

func Redis_hkeys(t string) ([]string, error) {
	c := g_pool.Get()
	if c == nil {
		Error("connect is nil")
		return nil, errors.New("connect is nil")
	}

	c.Send("SELECT", 1)
	c.Send("HKEYS", t)
	c.Flush()
	c.Receive()
	b, e := redis.Strings(c.Receive())
	if e != nil {
		Error(e.Error())
		return nil, e
	}

	return b, nil
}

func Redis_hdel(t, k string) error {
	c := g_pool.Get()
	if c == nil {
		Error("connect is nil")
		return errors.New("connect is nil")
	}

	c.Send("SELECT", 1)
	c.Send("HDEL", t, k)
	c.Flush()
	c.Receive()
	_, e := c.Receive()
	if e != nil {
		Error(e.Error())
		return e
	}

	return nil
}

func Redis_hmset(t string, data map[string]interface{}) error {
	c := g_pool.Get()
	if c == nil {
		Error("connect is nil")
		return errors.New("connect is nil")
	}

	c.Send("SELECT", 1)
	for k, v := range data {
		b, e := json.Marshal(v)
		if e != nil {
			Error(e.Error())
			return e
		}
		// fmt.Printf("b = %v, %s\n", v, string(b))
		c.Send("HSET", t, k, b)
	}

	c.Flush()
	c.Receive()
	for k, _ := range data {
		_, e := c.Receive()
		if e != nil {
			Error("k[%s],%s", k, e.Error())
			return e
		}
	}

	return nil
}

func Redis_hmget(t string, ret *map[string]interface{}) error {
	c := g_pool.Get()
	if c == nil {
		Error("connect is nil")
		return errors.New("connect is nil")
	}

	c.Send("SELECT", 1)
	keys := make([]string, len(*ret))
	i := 0
	for k, _ := range *ret {
		c.Send("HGET", t, k)
		keys[i] = k
		i++
	}

	c.Flush()
	c.Receive()
	for _, k := range keys {

		b, e := redis.Bytes(c.Receive())
		if e != nil {
			Error(e.Error())
			return e
		}

		e = json.Unmarshal(b, (*ret)[k])
		if e != nil {
			Error(e.Error())
			return e
		}
	}

	return nil
}

//往t集合中添加v
func Redis_Sset(k string, v interface{}) error {
	c := g_pool.Get()
	if c == nil {
		Error("connect is nil")
		return errors.New("connect is nil")
	}
	c.Send("SELECT", 2)
	c.Send("SADD", k, v)
	c.Flush()
	c.Receive()
	_, e1 := c.Receive()
	if e1 != nil {
		Error(e1.Error())
		return e1
	}
	return nil
}

func Redis_Sget(k string) ([]interface{}, error) {
	c := g_pool.Get()
	if c == nil {
		Error("connect is nil")
		return nil, errors.New("connect is nil")
	}
	c.Send("SELECT", 2)
	c.Send("SMEMBERS", k)
	c.Flush()
	c.Receive()
	re, e := redis.Values(c.Receive())
	if e != nil {
		Error(e.Error())
	}
	return re, e
}

//删除t集合中的一个或者多个k
func Redis_Sdel(t string, k interface{}) error {
	c := g_pool.Get()
	if c == nil {
		Error("connect is nil")
		return errors.New("connect is nil")
	}
	c.Send("SELECT", 2)
	c.Send("SREM", t, k)
	c.Flush()
	c.Receive()
	_, e := c.Receive()
	if e != nil {
		Error(e.Error())
		return e
	}
	return nil
}

//返回多个集合k的交集v
func Redis_SINTER(v interface{}, k interface{}) error {
	c := g_pool.Get()
	if c == nil {
		Error("connect is nil")
		return errors.New("connect is nil")
	}

	b1, e1 := json.Marshal(k)
	if e1 != nil {
		Error(e1.Error())
		return e1
	}

	c.Send("SELECT", 2)
	c.Send("SINTER", b1)
	c.Flush()
	c.Receive()
	b, e := redis.Bytes(c.Receive())
	if e != nil {
		Error(e.Error())
		return e
	}
	e = json.Unmarshal(b, v)
	if e != nil {
		Error(e.Error())
		return e
	}
	return nil
}
