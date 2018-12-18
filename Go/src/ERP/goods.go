package main

import (
	"JGo/JHttp"
	"JGo/JLogger"
	"JGo/JStore/JRedis"
	"fmt"
)

type Goods struct {
	ID           string  //商品ID
	Name         string  //商品名称
	Type         string  //商品类别
	Format       string  //规格
	Num          float64 //库存
	Unit         string  //单位
	SID          string  //供应商id
	SupplierName string  //供应商名称
	CreatTime    string  //创建时间
	CreatStamp   int64   //创建的时间戳
	LastTime     int64   //最后更新时间
	IsDel        bool    //标记删除
}

//新建商品
func NewGoods(session *JHttp.Session) {
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
	st.CreatStamp = CurStamp()
	st.LastTime = CurStamp()
	st.IsDel = false
	if err := JRedis.Redis_hset(Hash_Goods, st.ID, st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}

	if st.Type != "" {
		go appendGoodsType(st.Type)
	}

	//更新
	increaseUpdate(STRUCT_GOODS)

	session.Forward("0", "NewGoods success", st)
}

//查询商品信息
func QueryGoods(session *JHttp.Session) {
	type Para struct {
		GoodsID string
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.GoodsID == "" {
		JLogger.Error("QueryGoods failed,GoodsID is empty\n")
		session.Forward("1", "QueryGoods failed,GoodsID is empty\n", nil)
		return
	}

	data := &Goods{}
	if err := JRedis.Redis_hget(Hash_Goods, st.GoodsID, data); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	session.Forward("0", "success\n", data)
}

//修改商品
func ModGoods(session *JHttp.Session) {
	type Para struct {
		ID           string
		Name         string //商品名称
		Type         string //类别
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

	newType := false
	if st.Type != data.Type {
		newType = true
	}
	data.Name = st.Name
	data.Type = st.Type
	data.Unit = st.Unit
	data.Format = st.Format
	data.SID = st.SID
	data.SupplierName = st.SupplierName
	data.LastTime = CurStamp()
	if err := JRedis.Redis_hset(Hash_Goods, data.ID, data); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}

	if newType && data.Type != "" {
		go appendGoodsType(data.Type)
	}

	//更新
	increaseUpdate(STRUCT_GOODS)

	session.Forward("0", "modify success", data)
}

//商品入库
func AddGoodsNum(session *JHttp.Session) {
	type Para struct {
		ID  string  //商品id
		Num float64 //数量
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.ID == "" || st.Num <= 0 {
		str := fmt.Sprintf("AddGoodsNums failed,ID=%s,Num=%f\n", st.ID, st.Num)
		JLogger.Error(str)
		session.Forward("1", str, nil)
		return
	}
	data, err := inoutGoodsNum(st.ID, st.Num, true)
	if err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}

	//更新
	increaseUpdate(STRUCT_GOODS)

	session.Forward("0", "mod success\n", data)
}

//商品入库、出库
func inoutGoodsNum(ID string, Num float64, isAdd bool) (*Goods, error) {
	data := &Goods{}
	if err := JRedis.Redis_hget(Hash_Goods, ID, data); err != nil {
		JLogger.Error(err.Error())
		return data, err
	}
	if isAdd {
		data.Num += Num
	} else {
		data.Num -= Num
	}
	data.LastTime = CurStamp()
	if err := JRedis.Redis_hset(Hash_Goods, ID, data); err != nil {
		JLogger.Error(err.Error())
		return data, err
	}
	return data, nil
}

//删除商品
func DelGoods(session *JHttp.Session) {
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
	data.LastTime = CurStamp()
	data.IsDel = true

	if err := JRedis.Redis_hset(Hash_Goods, st.ID, data); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	//更新
	increaseUpdate(STRUCT_GOODS)

	session.Forward("0", "del success\n", data)
}

//获取全部商品
func GetGlobalGoods(session *JHttp.Session) {
	st := &AllPara{}
	if err := session.GetPara(st); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	keys, _ := JRedis.Redis_hkeys(Hash_Goods)
	data := []*Goods{}
	if st.Type == 1 {
		if st.Stamp > getUpdateStamp(STRUCT_GOODS) {
			session.Forward("0", "success", data)
			return
		}
	}
	for _, v := range keys {
		d := &Goods{}
		if err := JRedis.Redis_hget(Hash_Goods, v, d); err == nil {
			if !d.IsDel {
				if st.Type == 0 {
					data = append(data, d)
				} else {
					if d.LastTime > st.Stamp {
						data = append(data, d)
					}
				}
			}
		}
	}
	session.Forward("0", "GetGlobalGoods success\n", data)
}

//删除一个商品类型
func RemoveGoodsType(session *JHttp.Session) {
	type Para struct {
		Type string
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if err := removeGoodsType(st.Type); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}

	session.Forward("0", "RemoveGoodsType success\n", st.Type)
}

//添加一个商品类型
func AddGoodsType(session *JHttp.Session) {
	type Para struct {
		Type string
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if err := appendGoodsType(st.Type); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}

	session.Forward("0", "AddGoodsType success\n", st.Type)
}

//获取所有商品类型
func GetAllGoodsType(session *JHttp.Session) {
	list := []string{}
	if err := JRedis.Redis_hget(Hash_SupplierGoods, Key_GoodType, &list); err != nil {
		JLogger.Error(err.Error())
	}
	session.Forward("0", "get GoodsType success\n", list)
}

func appendGoodsType(Type string) error {
	list := []string{}
	if err := JRedis.Redis_hget(Hash_SupplierGoods, Key_GoodType, &list); err != nil {
		JLogger.Info(err.Error())
	}
	ok := false
	for _, v := range list {
		if v == Type {
			ok = true
			break
		}
	}
	if !ok {
		list = append(list, Type)
	}

	return JRedis.Redis_hset(Hash_SupplierGoods, Key_GoodType, &list)
}

func removeGoodsType(Type string) error {
	list := []string{}
	if err := JRedis.Redis_hget(Hash_SupplierGoods, Key_GoodType, &list); err != nil {
		JLogger.Error(err.Error())
		return err
	}
	index := -1
	for i, v := range list {
		if v == Type {
			index = i
			break
		}
	}
	if index != -1 {
		list = append(list[:index], list[index+1:]...)
	}
	return JRedis.Redis_hset(Hash_SupplierGoods, Key_GoodType, &list)
}
