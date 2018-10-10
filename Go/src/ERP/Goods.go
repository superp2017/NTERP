package main

import (
	"JGo/JHttp"
	"JGo/JLogger"
	"JGo/JStore/JRedis"
	"fmt"
)

type Goods struct {
	ID           string //商品ID
	Name         string //商品名称
	Type         string //商品类别
	Format       string //规格
	Unit         string //单位
	Price        int    //进价
	SID          string //供应商id
	SupplierName string //供应商名称
	CreatTime    string //创建时间
}

//新建商品
func newGoods(session *JHttp.Session) {
	st := &Goods{}
	if err := session.GetPara(st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	st.ID = getGoodsID()
	if st.Name == "" || st.Type == "" {
		str := fmt.Sprintf("NewGoods failed,Name=%s,Type=%s\n", st.Name, st.Type)
		JLogger.Error(str)
		session.Forward("1", str, nil)
		return
	}
	st.CreatTime = CurTime()
	if err := JRedis.Redis_hset(Hash_Goods, st.ID, st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.SID != "" && st.ID != "" {
		go appendSupplierGoods(st.SID, st.ID)
	}
	if st.Type != "" {
		go appendGoodsType(st.Type)
	}
	session.Forward("0", "NewGoods success", st)
}

//修改商品
func modGoods(session *JHttp.Session) {
	type Para struct {
		ID           string
		Name         string //商品名称
		Type         string //类别
		Price        int    //进价
		Unit         string //单位
		Format       string //规格
		SID          string //供应商id
		SupplierName string //供应商名称
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.ID == "" {
		JLogger.Error("ModifyGoods failed , GoodsID is empty \n")
		session.Forward("1", "ModifyGoods failed , GoodsID is empty \n", nil)
		return
	}
	data := &Goods{}
	if err := JRedis.Redis_hget(Hash_Goods, st.ID, data); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	remove := false
	sid := data.SID
	if data.SID != st.SID && st.SID != "" && data.SID != "" {
		remove = true
	}
	newType := false
	if st.Type != data.Type {
		newType = true
	}
	data.Name = st.Name
	data.Type = st.Type
	data.Price = st.Price
	data.Unit = st.Unit
	data.Format = st.Format
	data.SID = st.SID
	data.SupplierName = st.SupplierName
	if err := JRedis.Redis_hset(Hash_Goods, data.ID, data); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if remove {
		go removeSupplierGoods(sid, data.ID)
		if data.SID != "" && data.ID != "" {
			go appendSupplierGoods(st.SID, st.ID)
		}
	}
	if newType && data.Type != "" {
		go appendGoodsType(data.Type)
	}
	session.Forward("0", "modify success", data)
}

//删除商品
func delGoods(session *JHttp.Session) {
	type Para struct {
		ID string //商品id
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.ID == "" {
		JLogger.Error("DelGoods failed , GoodsID is empty \n")
		session.Forward("1", "DelGoods failed , GoodsID is empty \n", nil)
		return
	}

	data := &Goods{}
	if err := JRedis.Redis_hget(Hash_Goods, st.ID, data); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}

	if err := JRedis.Redis_hdel(Hash_Goods, st.ID); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if data.SID != "" && data.ID != "" {
		go removeSupplierGoods(data.SID, data.ID)
	}
	session.Forward("0", "del success\n", st.ID)
}

//获取供应商的商品列表
func getSupplierGoods(session *JHttp.Session) {
	type Para struct {
		SID string
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.SID == "" {
		JLogger.Error("GetSupplierGoods failed SID is empty\n")
		session.Forward("1", "GetSupplierGoods failed SID is empty\n", nil)
		return
	}
	list := []string{}
	if err := JRedis.Redis_hget(Hash_SupplierGoods, st.SID, &list); err != nil {
		JLogger.Info(err.Error())
	}
	data := []*Goods{}
	for _, v := range list {
		d := &Goods{}
		if err := JRedis.Redis_hget(Hash_Goods, v, d); err == nil {
			data = append(data, d)
		}
	}
	session.Forward("0", "success", data)
}

//获取全部商品
func getGlobalGoods(session *JHttp.Session) {
	keys, _ := JRedis.Redis_hkeys(Hash_Goods)
	data := []*Goods{}
	for _, v := range keys {
		d := &Goods{}
		if err := JRedis.Redis_hget(Hash_Goods, v, d); err == nil {
			data = append(data, d)
		}
	}
	session.Forward("0", "GetGlobalGoods success\n", data)
}
