package main

import (
	"JGo/JHttp"
	"JGo/JLogger"
	"JGo/JStore/JRedis"
	"JsGo/JsLogger"
	"JsGo/JsStore/JsRedis"
	"fmt"
)

type StoreGoods struct {
	ID           string //商品ID
	Name         string //商品名称
	Type         string //类别
	Price        int    //进价
	Num          int    //数量
	Unit         string //单位
	TotalPrice   int    //库存总价
	Format       string //规格
	Color        string //颜色
	SID          string //供应商id
	SupplierName string //供应商名称
	Status       string //状态
	Note         string //备注
	CreatTime    string //创建时间
}

//新建商品
func NewGoods(session *JHttp.Session) {
	st := &StoreGoods{}
	if err := session.GetPara(st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	st.ID =getGoodsID()
	if st.Name == "" || st.Num < 0 || st.Type == "" {
		str := fmt.Sprintf("NewGoods failed,Name=%s,Num=%s,Type=%s\n", st.Name, st.Num, st.Type)
		JLogger.Error(str)
		session.Forward("1", str, nil)
		return
	}
	st.TotalPrice = st.Price * st.Num
	st.CreatTime = CurTime()
	st.Status = "0"
	if err := JRedis.Redis_hset(Hash_Goods, st.ID, st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.SID != "" && st.ID != "" {
		go appendSupplierGoods(st.SID, st.ID)
	}
	session.Forward("0", "NewGoods success", st)
}

//修改商品
func ModifyGoods(session *JHttp.Session) {
	type Para struct {
		ID           string
		Name         string //商品名称
		Type         string //类别
		Price        int    //进价
		Num          int    //数量
		Unit         string //单位
		Format       string //规格
		Color        string //颜色
		SID          string //供应商id
		SupplierName string //供应商名称
		Status       string //状态
		Note         string //备注
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		JsLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.ID == "" {
		JsLogger.Error("ModifyGoods failed , GoodsID is empty \n")
		session.Forward("1", "ModifyGoods failed , GoodsID is empty \n", nil)
		return
	}
	data := &StoreGoods{}
	if err := JsRedis.Redis_hget(Hash_Goods, st.ID, data); err != nil {
		JsLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	data.Name = st.Name
	data.Type = st.Type
	data.Price = st.Price
	data.Num = st.Num
	data.Unit = st.Unit
	data.Format = st.Format
	data.Color = st.Color
	data.SID = st.SID
	data.SupplierName = st.SupplierName
	data.Status = st.Status
	data.Note = st.Note
	data.TotalPrice = data.Price * data.Num
	if err := JsRedis.Redis_hset(Hash_Goods, data.ID, data); err != nil {
		JsLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	session.Forward("0", "modify success", data)
}

//删除商品
func DelGoods(session *JHttp.Session) {
	type Para struct {
		ID string //商品id
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		JsLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.ID == "" {
		JsLogger.Error("DelGoods failed , GoodsID is empty \n")
		session.Forward("1", "DelGoods failed , GoodsID is empty \n", nil)
		return
	}

	data := &StoreGoods{}
	if err := JsRedis.Redis_hget(Hash_Goods, st.ID, data); err != nil {
		JsLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}

	if err := JsRedis.Redis_hdel(Hash_Goods, st.ID); err != nil {
		JsLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if data.SID != "" && data.ID != "" {
		go removeSupplierGoods(data.SID, data.ID)
	}
	session.Forward("0", "del success\n", st.ID)
}

//获取供应商的商品列表
func GetSupplierGoods(session *JHttp.Session) {
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
	data := []*StoreGoods{}
	for _, v := range list {
		d := &StoreGoods{}
		if err := JRedis.Redis_hget(Hash_Goods, v, d); err == nil {
			data = append(data, d)
		}
	}
	session.Forward("0", "success", data)
}

//获取全部商品
func GetGlobalGoods(session *JHttp.Session) {
	keys, _ := JRedis.Redis_hkeys(Hash_Goods)
	data := []*StoreGoods{}
	for _, v := range keys {
		d := &StoreGoods{}
		if err := JRedis.Redis_hget(Hash_Goods, v, d); err == nil {
			data = append(data, d)
		}
	}
	session.Forward("0", "GetGlobalGoods success\n", data)
}

func appendSupplierGoods(SID, GoodsID string) error {
	list := []string{}
	if err := JRedis.Redis_hget(Hash_SupplierGoods, SID, &list); err != nil {
		JLogger.Info(err.Error())
	}
	ok := false
	for _, v := range list {
		if v == GoodsID {
			ok = true
			break
		}
	}
	if !ok {
		list = append(list, GoodsID)
	}

	return JRedis.Redis_hset(Hash_SupplierGoods, SID, &list)
}

func removeSupplierGoods(SID, GoodsID string) error {
	list := []string{}
	if err := JRedis.Redis_hget(Hash_SupplierGoods, SID, &list); err != nil {
		JLogger.Info(err.Error())
	}
	index := -1
	for i, v := range list {
		if v == GoodsID {
			index = i
			break
		}
	}
	if index != -1 {
		list = append(list[:index], list[index+1:]...)
	}

	return JRedis.Redis_hset(Hash_SupplierGoods, SID, &list)
}
