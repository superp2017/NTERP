package main

import (
	"JGo/JHttp"
	"JGo/JLogger"
	"JGo/JStore/JRedis"
	"JsGo/JsStore/JsRedis"
	"time"
)

type StorageOutRecord struct {
	OutID          string //进出凭证id
	Factory        string //分厂名称
	FactoryNumber  string //分厂号
	ProductionLine string //产线名称
	CreatDate      string //领用时间
	ID             string //商品ID
	Name           string //商品名称
	Type           string //类别
	StrorageName   string //仓库名称
	Nums           int    //数量
	Unit           string //单位
	Note           string //备注
	UserName       string //领用人姓名
	UserID         string //领用人工号
}

//新的领用记录
func NewOutRecord(session *JHttp.Session) {
	st := &StorageOutRecord{}
	if err := session.GetPara(st); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.ID == "" || st.Name == "" || st.UserName == "" || st.UserID == "" || st.Nums <= 0 {
		JLogger.Error("NewOutRecord  Parame failed!\n")
		session.Forward("1", "NewOutRecord  Parame failed!\n", nil)
		return
	}
	st.ID = time.Unix(time.Now().Unix(), 0).Format("20060102150405")
	if err := JsRedis.Redis_hset(Hash_StorageOutRecord, st.ID, st); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	session.Forward("0", "success!\n", st)
}

func GetAllOutRecord(session *JHttp.Session) {
	list, err := JRedis.Redis_hkeys(Hash_StorageOutRecord)
	if err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	data := []*StorageOutRecord{}

	for _, v := range list {
		d := &StorageOutRecord{}
		if err := JRedis.Redis_hget(Hash_StorageOutRecord, v, d); err == nil {
			data = append(data, d)
		}
	}
	session.Forward("0", "success!\n", data)
}
