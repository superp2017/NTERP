package JsSsdb

import (
	"encoding/json"
	"errors"
	"log"
	"net/rpc"

	"JsGo/JsConfig"
	. "JsGo/JsLogger"
	"sync"
)

type ST_RecordPara struct {
	Table string
	Id    string
	Data  []byte
	Begin string
	End   string
	Limit int64
}

type ST_SData struct {
	Table string
	Id    string
	C     int
	T     string
}

type ST_DbServer struct {
	source string
	client *rpc.Client
}

var db_server *ST_DbServer
var g_lock sync.Mutex

func init() {
	init_sync()
}

func init_sync() {

	db_server = &ST_DbServer{}

	cfg, e := JsConfig.GetConfigMap([]string{"SsdbSyncServer"})
	if e != nil {
		log.Fatalln(e.Error())
	}

	db_server.source = cfg["Ip"] + ":" + cfg["Port"]
	db_server.client = nil

}

func getHandle() *ST_DbServer {

	if db_server.client != nil {

		return db_server
	}

	var err error
	db_server.client, err = rpc.DialHTTP("tcp", db_server.source)
	if err != nil {
		Info("NewClientsCache error:%s", err.Error())
		db_server.client = nil
		return nil
	}
	return db_server
}

func get(key string) ([]byte, error) {
	var para ST_RecordPara
	para.Table = key

	ret := make([]byte, 0)

	server := getHandle()
	if server == nil {
		return nil, errors.New("client error")
	}

	if err := server.client.Call("SyncHandle.CallGet", &para, &ret); err != nil {
		Error("SyncHandle.CallGet error:%s", err.Error())

		return nil, err
	} else {

		return ret, nil
	}
}

func set(key string, data []byte) error {
	var para ST_RecordPara
	para.Table = key

	para.Data = data

	ret := make([]byte, 0)

	server := getHandle()
	if server == nil {
		return errors.New("client error")
	}

	if err := server.client.Call("SyncHandle.CallSet", &para, &ret); err != nil {
		Error("SyncHandle.CallSet error:%s", err.Error())

		return err
	} else {

		return nil
	}
}

func exist(key string) (bool, error) {
	var para ST_RecordPara
	para.Table = key

	var ret bool = false

	server := getHandle()
	if server == nil {
		return false, errors.New("client error")
	}

	if err := server.client.Call("SyncHandle.CallExist", &para, &ret); err != nil {
		Error("SyncHandle.CallExist error:%s", err.Error())

		return false, err
	} else {

		return ret, nil
	}
}

func shareLock(table, id string, v interface{}) ([]byte, error) {
	var para ST_RecordPara
	para.Table = table
	para.Id = id

	ret := make([]byte, 0)

	server := getHandle()
	if server == nil {
		return nil, errors.New("client error")
	}

	Info("SyncHandle.CallShareLock table[%s], Id[%s]\n", table, id)
	if err := server.client.Call("SyncHandle.CallShareLock", &para, &ret); err != nil {
		Error("SyncHandle.CallShareLock error:%s", err.Error())

		return nil, err
	} else {

		json.Unmarshal(ret, v)

		return ret, nil
	}

}

func unsafeWrite(table, id string, b []byte) error {
	var para ST_RecordPara
	para.Table = table
	para.Id = id
	para.Data = b

	tmp := make([]byte, 0)

	server := getHandle()
	if server == nil {
		return errors.New("client error")
	}

	if err := server.client.Call("SyncHandle.CallUnsafeWrite", &para, &tmp); err != nil {
		Error("SyncHandle.CallUnsafeWrite error:%s", err.Error())

		return err
	} else {
		return nil
	}

}

func writeLock(table, id string, v interface{}) error {
	var para ST_RecordPara
	para.Table = table
	para.Id = id

	ret := make([]byte, 0)
	server := getHandle()
	if server == nil {
		return errors.New("client error")
	}

	Info("SyncHandle.CallWriteLock table[%s], Id[%s]\n", table, id)
	if err := server.client.Call("SyncHandle.CallWriteLock", &para, &ret); err != nil {
		Error("SyncHandle.CallWriteLock error:%s", err.Error())

		return err
	} else {
		if err := json.Unmarshal(ret, v); err != nil {
			Error(err.Error())
		}
		Info("leave WriteLock")
		return nil
	}

}

func hDel(table, id string) error {
	var para ST_RecordPara
	para.Table = table
	para.Id = id

	para.Data = nil

	server := getHandle()
	if server == nil {
		return errors.New("client error")
	}

	Info("SyncHandle.CallHDel table[%s], Id[%s]\n", table, id)
	if err := server.client.Call("SyncHandle.CallHDel", &para, nil); err != nil {
		Error("SyncHandle.CallHDel error:%s", err.Error())

		return err
	} else {
		return nil
	}
}

func hExist(table, id string) (bool, error) {
	var para ST_RecordPara
	para.Table = table
	para.Id = id

	server := getHandle()
	if server == nil {
		return false, errors.New("client error")
	}

	var ret bool = false

	if err := server.client.Call("SyncHandle.CallHExist", &para, &ret); err != nil {
		Error("SyncHandle.CallHExist error:%s", err.Error())

		return false, err
	} else {
		return ret, nil
	}
}

func hSize(table string) (int64, error) {
	var para ST_RecordPara
	para.Table = table

	server := getHandle()
	if server == nil {
		return -1, errors.New("client error")
	}

	var ret int64 = -1

	if err := server.client.Call("SyncHandle.CallHsize", &para, &ret); err != nil {
		Error("SyncHandle.CallHSize error:%s", err.Error())

		return -1, err
	} else {
		return ret, nil
	}
}

func hKeys(table, begin, end string, limit int64) ([]string, error) {
	var para ST_RecordPara
	para.Table = table
	para.Begin = begin
	para.End = end
	para.Limit = limit

	server := getHandle()
	if server == nil {
		return nil, errors.New("client error")
	}

	ret := make([]string, 0)

	if err := server.client.Call("SyncHandle.CallHKeys", &para, &ret); err != nil {
		Error("SyncHandle.CallHKeys error:%s", err.Error())

		return nil, err
	} else {
		return ret, nil
	}
}

func del(table string) error {
	var para ST_RecordPara
	para.Table = table
	para.Id = ""

	para.Data = nil

	server := getHandle()
	if server == nil {
		return errors.New("client error")
	}

	Info("SyncHandle.CallDel db[%s], table[%s],\n", table)
	if err := server.client.Call("SyncHandle.CallDel", &para, nil); err != nil {
		Error("SyncHandle.CallDel error:%s", err.Error())

		return err
	} else {
		return nil
	}
}

func writeBack(table, id string, v interface{}) error {
	var para ST_RecordPara
	para.Table = table
	para.Id = id

	b, e := json.Marshal(v)
	if e != nil {
		return e
	}

	para.Data = b

	//tmp := make([]byte, 0)

	//tmp := 0

	server := getHandle()
	if server == nil {
		return errors.New("client error")
	}

	Info("SyncHandle.CallWriteBack table[%s], Id[%s]\n", table, id)
	if err := server.client.Call("SyncHandle.CallWriteBack", &para, nil); err != nil {
		Error("SyncHandle.CallWriteBack error:%s", err.Error())

		return err
	} else {
		return nil
	}

}

func manualUnlock(table, id string) error {
	var para ST_RecordPara
	para.Table = table
	para.Id = id

	tmp := make([]byte, 0)

	server := getHandle()
	if server == nil {
		return errors.New("client error")
	}

	Info("SyncHandle.CallManualUnlock table[%s], Id[%s]\n", table, id)
	if err := server.client.Call("SyncHandle.CallManualUnlock", &para, &tmp); err != nil {
		Error("SyncHandle.CallManualUnlock error:%s", err.Error())

		return err
	} else {
		return nil
	}
}

func currentUnlock() []*ST_SData {
	var para ST_RecordPara
	ret := make([]*ST_SData, 0)

	server := getHandle()
	if server == nil {
		return nil
	}

	if err := server.client.Call("SyncHandle.CallCurrentUnLock", &para, &ret); err != nil {
		Error("SyncHandle.CallCurrentUnLock error:%s", err.Error())
		return nil
	} else {
		return ret
	}
}
