package main

import (
	"JGo/JHttp"
	"fmt"
	"JGo/JStore/JRedis"
	"JGo/JLogger"
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

func NewGoods(session *JHttp.Session) {
	st := &StoreGoods{}
	if err := session.GetPara(st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	st.ID = IDer(Hash_Goods)
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
	if st.SID != "" {
		go appendSupplierGoods(st.SID, st.ID)
	}
	session.Forward("0", "NewGoods success", st)
}

//获取供应商的商品列表
func GetSupplierGoods(session *JHttp.Session)  {
	type Para struct {
		SID string
	}
	st:=&Para{}
	if err:=session.GetPara(st);err!=nil{
		JLogger.Error(err.Error())
		session.Forward("1",err.Error(),nil)
		return
	}
	if st.SID==""{
		JLogger.Error("GetSupplierGoods failed SID is empty\n")
		session.Forward("1","GetSupplierGoods failed SID is empty\n",nil)
		return
	}
	list := []string{}
	if err := JRedis.Redis_hget(Hash_SupplierGoods, st.SID, &list); err != nil {
		JLogger.Info(err.Error())
	}
	data:=[]*StoreGoods{}
	for _,v:=range list {
		d:=&StoreGoods{}
		if err:=JRedis.Redis_hget(Hash_Goods,v,d);err==nil{
			data= append(data,d)
		}
	}
	session.Forward("0","success",data)
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
	session.Forward("0","GetGlobalGoods success\n",data)
}

func appendSupplierGoods(SID, GoodsID string) error {
	list := []string{}
	if err := JRedis.Redis_hget(Hash_SupplierGoods, SID, &list); err != nil {
		JLogger.Info(err.Error())
	}
	ok:=false
	for _,v:=range list{
		if v==GoodsID{
			ok = true
			break
		}
	}
	if !ok{
		list =append(list,GoodsID)
	}

	return JRedis.Redis_hset(Hash_SupplierGoods, SID, &list)
}
