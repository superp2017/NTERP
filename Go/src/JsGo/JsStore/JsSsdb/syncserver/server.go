package main

import (
	"errors"
	"fmt"
	"log"
	"net"
	"net/http"
	"net/rpc"

	"JsGo/JsConfig"
)

type SyncHandle int

var handle net.Listener

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

func (sh *SyncHandle) CallGet(para *ST_RecordPara, ret *[]byte) error {

	*ret = sync_Get(para.Table)
	if *ret == nil {
		return errors.New(fmt.Sprintf(" Get table[%s] error", para.Table))
	}
	return nil
}

func (sh *SyncHandle) CallSet(para *ST_RecordPara, ret *[]byte) error {
	return sync_Set(para.Table, para.Data)
}

func (sh *SyncHandle) CallExist(para *ST_RecordPara, ret *bool) error {
	*ret = sync_Exist(para.Table)
	return nil
}

func (sh *SyncHandle) CallUnsafeWrite(para *ST_RecordPara, ret *[]byte) error {
	return sync_UnsafeWrite(para.Table, para.Id, para.Data)
}

func (sh *SyncHandle) CallShareLock(para *ST_RecordPara, ret *[]byte) error {
	g_log.Info("-----ShareLock:table[%s],id[%s]\n", para.Table, para.Id)
	*ret = sync_ShareLock(para.Table, para.Id)
	if *ret == nil {
		g_log.Info("ShareLock error: table[%s],id[%s]\n", para.Table, para.Id)
		return errors.New(fmt.Sprintf("share lock failed table[%s], id[%s]", para.Table, para.Id))
	}
	return nil
}

func (sh *SyncHandle) CallWriteLock(para *ST_RecordPara, ret *[]byte) error {

	g_log.Info("<<<<<WriteLock:db[%d], table[%s],id[%s]\n", para.Table, para.Id)
	*ret = sync_WriteLock(para.Table, para.Id)
	if *ret == nil {
		g_log.Error("WriteLock error:db[%d], table[%s],id[%s]\n", para.Table, para.Id)
		return errors.New(fmt.Sprintf("lock failed  table[%s], id[%s]", para.Table, para.Id))
	}
	return nil
}

func (sh *SyncHandle) CallHDel(para *ST_RecordPara, ret *[]byte) error {
	g_log.Info("<<<<<CallHDel: table[%s],id[%s]\n", para.Table, para.Id)
	e := sync_HDel(para.Table, para.Id)
	if e != nil {
		g_log.Error("CallHDel error: table[%s],id[%s]\n", para.Table, para.Id)
		return errors.New(fmt.Sprintf("CallHDel failed  table[%s], id[%s]", para.Table, para.Id))
	}
	return nil
}

func (sh *SyncHandle) CallHExist(para *ST_RecordPara, ret *bool) error {
	*ret = sync_HExist(para.Table, para.Id)
	return nil
}

func (sh *SyncHandle) CallHsize(para *ST_RecordPara, ret *int64) error {
	*ret = sync_Hsize(para.Table)
	return nil
}

func (sh *SyncHandle) CallHKeys(para *ST_RecordPara, ret *[]string) error {
	*ret = sync_Hkeys(para.Table, para.Begin, para.End, para.Limit)
	return nil
}

func (sh *SyncHandle) CallDel(para *ST_RecordPara, ret *[]byte) error {
	g_log.Info("<<<<<CallHDel: key[%s]\n", para.Table, para.Id)
	e := sync_Del(para.Table)
	if e != nil {
		g_log.Error("CallHDel error: key[%s]\n", para.Table)
		return errors.New(fmt.Sprintf("CallHDel failed  key[%s]", para.Table))
	}
	return nil
}

func (sh *SyncHandle) CallWriteBack(para *ST_RecordPara, ret *[]byte) error {
	g_log.Info(">>>>>WriteBack:table[%s],id[%s]\n", para.Table, para.Id)
	return sync_WriteBack(para.Table, para.Id, para.Data)
}

func (sh *SyncHandle) CallManualUnlock(para *ST_RecordPara, ret *[]byte) error {
	g_log.Info(">>>>>Manual unlock:table[%s],id[%s]\n", para.Table, para.Id)
	return sync_ManualUnlock(para.Table, para.Id)
}

func (sh *SyncHandle) CallCurrentUnLock(para *ST_RecordPara, ret *[]*ST_SData) error {

	*ret = sync_CurrentUnLock()
	g_log.Info(">>>>>CallCurrentUnLock, n = %d, g_count = %d\n", len(*ret), g_count)
	return nil
}

func StartServer() {

	hb := new(SyncHandle)
	rpc.Register(hb)
	rpc.HandleHTTP()

	//handle, e := net.Listen("tcp", fmt.Sprintf(":%s",vgconst.CT_ClientCachePort))

	port, e := JsConfig.GetConfigString([]string{"Dtio", "Port"})
	if e != nil {
		log.Fatal("port error:", e)
	}
	handle, e = net.Listen("tcp", ":"+port)

	if e != nil {
		log.Fatal("listen error:", e)
	}
	http.Serve(handle, nil)

}

func StopServer() {
	if nil != handle {
		handle.Close()
	}
}
