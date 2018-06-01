package JRedis

import (
	. "JGo/JLogger"
	"encoding/json"
	"errors"

	"github.com/gomodule/redigo/redis"
)

const (
	s_DB int = 15
)

func Redis_idbase() (uint64, error) {
	c := g_pool.Get()
	if c == nil {
		Error("connect is nil")
		return 0, errors.New("connect is nil")
	}

	c.Send("SELECT", s_DB)
	c.Send("GET", "IDBASE")
	c.Flush()
	c.Receive()
	u, e := redis.Uint64(c.Receive())
	if e != nil {
		return 0, e
	}
	return u, nil
}

func Redis_idbase_update(idbase uint64) error {
	c := g_pool.Get()
	if c == nil {
		Error("connect is nil")
		return errors.New("connect is nil")
	}

	c.Send("SELECT", s_DB)
	c.Send("SET", "IDBASE", idbase)
	c.Flush()
	c.Receive()
	_, e := c.Receive()
	if e != nil {
		return e
	}
	return nil
}

func Redis_index(id uint64, doc map[string]interface{}) error {
	c := g_pool.Get()
	if c == nil {
		Error("connect is nil")
		return errors.New("connect is nil")
	}

	b, e := json.Marshal(doc)
	if e != nil {
		Error(e.Error())
		return e
	}

	c.Send("SELECT", s_DB)
	c.Send("SET", id, b)
	c.Flush()
	c.Receive()

	_, e = redis.Bytes(c.Receive())
	if e != nil {
		Error(e.Error())
		return e
	}

	return nil
}

func Redis_query(ids []uint64) (map[uint64]interface{}, error) {
	c := g_pool.Get()
	if c == nil {
		Error("connect is nil")
		return nil, errors.New("connect is nil")
	}

	c.Send("SELECT", s_DB)
	for _, v := range ids {
		c.Send("GET", v)
	}
	c.Flush()
	c.Receive()
	ret := make(map[uint64]interface{})
	for _, v := range ids {
		b, e := redis.String(c.Receive())
		if e != nil {
			Error("query[%d]error:%s", v, e.Error())
			continue
		}
		ret[v] = b
	}

	return ret, nil
}
