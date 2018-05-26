package main

import (
	"errors"
	"fmt"
	"log"
	"sync"
	"time"

	// "JsLib/JsDBCache/ssdb"
	"JsGo/JsLogger"
)

type ST_Record struct {
	Name string
	Id   string
	V    []byte
	C    int
	T    time.Time
	Mu   sync.Mutex
}

var g_log *JsLogger.ST_Logger
var g_mutex sync.Mutex
var g_count int

var g_records map[string]map[string]*ST_Record

func init() {
	var ok bool
	g_log, ok = JsLogger.GetComLogger()
	if !ok {
		log.Fatalln("GetComLogger error")
	}

	g_records = make(map[string]map[string]*ST_Record)
	g_count = 0
	go sync_Scavenger()
}

func sync_Get(key string) []byte {
	b, e := get(key)
	if e != nil {
		return nil
	}

	return b.Bytes()
}

func sync_Set(key string, val interface{}) error {
	return set(key, val)
}

func sync_Exist(key string) bool {
	return exists(key)
}

func sync_ShareLock(table, id string) []byte {

	b, e := hget(table, id)
	if e != nil {
		return nil
	}

	return b.Bytes()
}

func sync_UnsafeWrite(table, id string, b []byte) error {

	e := hset(table, id, string(b))
	if e != nil {
		return e
	}
	return nil
}

func sync_Scavenger() {
	for {
		time.Sleep(time.Minute * 30)
		n := time.Now().Hour()
		if g_count > 500000 || n == 2 {
			g_mutex.Lock()
			for j, rec := range g_records {
				del := make([]string, 0)
				for _, v := range rec {
					if v.C <= 0 {
						del = append(del, v.Id)
					}
				}

				for _, d := range del {
					delete(rec, d)
				}

				g_records[j] = rec
			}
			g_mutex.Unlock()
		}
	}
}

func sync_WriteLock(table, id string) []byte {
	var wr *ST_Record
	g_mutex.Lock()
	g_count++
	dbs := g_records

	rd, ok := dbs[table]
	if ok {

		wr, ok = rd[id]
		if ok {

			wr.C++
			g_mutex.Unlock()
			wr.Mu.Lock()
			wr.T = time.Now()
			return wr.V
		}
	} else {
		g_records[table] = make(map[string]*ST_Record)
	}

	var rec ST_Record

	rec.V = sync_ShareLock(table, id)
	if rec.V == nil {
		g_mutex.Unlock()
		return nil
	}

	rec.Name = table
	rec.Id = id
	rec.C = 1

	g_records[table][id] = &rec
	g_mutex.Unlock()

	rec.Mu.Lock()

	rec.T = time.Now()

	return rec.V
}

func sync_HExist(table, id string) bool {
	return hexists(table, id)
}

func sync_Hsize(table string) int64 {
	return hsize(table)
}

func sync_Hkeys(table, begin, end string, limit int64) []string {
	return hkeys(table, begin, end, limit)
}

func sync_HDel(table, id string) error {

	e := hdel(table, id)

	if e != nil {
		return e
	}
	return nil
}

func sync_Del(id string) error {
	e := del(id)
	if e != nil {
		return e
	}
	return nil
}

func sync_WriteBack(table, id string, v []byte) error {
	g_mutex.Lock()
	g_count--
	dbs := g_records

	rd, ok := dbs[table]
	if ok {
		wr, ok := rd[id]
		g_mutex.Unlock()
		if ok {
			var e error
			wr.V = v
			wr.C--
			wr.Mu.Unlock()

			if e != nil {
				return e
			}

			go sync_UnsafeWrite(table, id, wr.V)

			return nil
		}
		return errors.New(fmt.Sprintf("error: table %s id %s error", table, id))

	} else {

		g_mutex.Unlock()
		return errors.New(fmt.Sprintf("error: table %s error", table))
	}

}

func sync_ManualUnlock(table, id string) error {
	g_mutex.Lock()

	dbs := g_records

	rd, ok := dbs[table]
	if ok {
		wr, ok := rd[id]
		g_mutex.Unlock()
		if ok {
			var e error
			g_count -= wr.C
			wr.C = 0
			wr.Mu.Unlock()

			if e != nil {
				return e
			}

			return nil
		}
		return errors.New(fmt.Sprintf("error: table %s id %s error", table, id))

	} else {

		g_mutex.Unlock()
		return errors.New(fmt.Sprintf("error: table %s error", table))
	}

}

func sync_CurrentUnLock() []*ST_SData {
	g_mutex.Lock()
	defer g_mutex.Unlock()

	var list []*ST_SData = make([]*ST_SData, 0)

	dbs := g_records

	for tbl, rec := range dbs {
		del := make([]string, 0)
		for I, J := range rec {
			if J.C > 0 {
				var da ST_SData
				da.Table = tbl
				da.Id = I
				da.C = J.C
				da.T = J.T.Format("2006-01-02 15:04:05")
				list = append(list, &da)
			} else {
				del = append(del, J.Id)
			}
		}

		for _, d := range del {
			delete(rec, d)
		}

		dbs[tbl] = rec

		g_records = dbs

	}

	return list
}
