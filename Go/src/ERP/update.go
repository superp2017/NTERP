package main

import (
	"JGo/JHttp"
	"JGo/JStore/JRedis"
	"sync"
	"time"
)

////更新的数据类型
const (
	STRTUCT_ORDER     = 0  //订单
	STRUCT_MATERIAL   = 1  //物料
	STRUCT_GOODS      = 2  ///商品
	STRUCT_OUTRECORD  = 3  ///出库记录
	STRUCT_USER       = 4  //员工
	STRUCT_CUSTOMER   = 5  //客户
	STRUCT_SUPPLIER   = 6  //供应商
	STRUCT_UNIT       = 7  //单位
	STRUCT_DEPARTMENT = 8  //部门
	STRUCT_GOODS_TYPE = 9  //商品的分类
	STRUCT_PLANTING   = 10 //镀种
)

var LastStamp = map[int]int64{
	STRTUCT_ORDER:    0,
	STRUCT_MATERIAL:  0,
	STRUCT_GOODS:     0,
	STRUCT_OUTRECORD: 0,
	STRUCT_USER:      0,
	STRUCT_CUSTOMER:  0,
	STRUCT_SUPPLIER:  0,
}
var updateMutex sync.RWMutex

//添加最后更新时间
func increaseUpdate(datatype int) {
	updateMutex.Lock()
	defer updateMutex.Unlock()
	if _, ok := LastStamp[datatype]; ok {
		LastStamp[datatype] = CurStamp()
	}
}

//获取最后最后更新时间
func getUpdateStamp(datatype int) int64 {
	updateMutex.RLock()
	defer updateMutex.RUnlock()
	if t, ok := LastStamp[datatype]; ok {
		return t
	}
	return 0
}

func updateAll(session *JHttp.Session) {
	go updateOrder()
	go updateUser()
	go updateGoods()
	go updateGoodsOut()
	go updateCustomer()
	go updateSupplier()
	go updateMaterial()
	session.Forward("0", "success", nil)
}

//默认删除7天内的标记的订单
func updateOrder() {
	list, err := JRedis.Redis_hkeys(Hash_Order)
	if err != nil {
		return
	}
	for i := len(list) - 1; i >= 0; i-- {
		v := list[i]
		if v == Key_LastOrderDate {
			continue
		}
		d := &Order{}
		if err := JRedis.Redis_hget(Hash_Order, v, d); err == nil {
			if t, e := time.Parse("2006-01-02 15:04:05", d.Current.OpreatTime); e == nil {
				d.LastTime = t.Unix()
			}
			if t, e := time.Parse("2006-01-02 15:04:05", d.CreatTime); e == nil {
				d.CreatStamp = t.Unix()
			}
			d.IsDel = false
			if d.Current.Status == "Status_Del" {
				d.IsDel = true
			}
			JRedis.Redis_hset(Hash_Order, v, d)
		}
	}
}

func updateUser() {
	list, err := JRedis.Redis_hkeys(Hash_Employee)
	if err != nil {
		return
	}
	for _, v := range list {
		d := &Employee{}
		if err := JRedis.Redis_hget(Hash_Employee, v, d); err == nil {
			d.LastTime = time.Now().Unix()
			if t, e := time.Parse("2006-01-02 15:04:05", d.CreatTime); e == nil {
				d.CreatStamp = t.Unix()
			}
			JRedis.Redis_hset(Hash_Employee, v, d)
		}
	}
}

func updateGoods() {
	list, err := JRedis.Redis_hkeys(Hash_Goods)
	if err != nil {
		return
	}
	for _, v := range list {
		d := &Goods{}
		if err := JRedis.Redis_hget(Hash_Goods, v, d); err == nil {
			d.LastTime = time.Now().Unix()
			if t, e := time.Parse("2006-01-02 15:04:05", d.CreatTime); e == nil {
				d.CreatStamp = t.Unix()
			}
			JRedis.Redis_hset(Hash_Goods, v, d)
		}
	}
}

func updateGoodsOut() {
	list, err := JRedis.Redis_hkeys(Hash_StorageOutRecord)
	if err != nil {
		return
	}
	for _, v := range list {
		d := &StorageOutRecord{}
		if err := JRedis.Redis_hget(Hash_StorageOutRecord, v, d); err == nil {
			d.LastTime = time.Now().Unix()
			if t, e := time.Parse("2006-01-02", d.CreatDate); e == nil {
				d.CreatStamp = t.Unix()
			}
			JRedis.Redis_hset(Hash_StorageOutRecord, v, d)
		}
	}
}

func updateCustomer() {
	list, err := JRedis.Redis_hkeys(Hash_Customer)
	if err != nil {
		return
	}
	for _, v := range list {
		d := &Customer{}
		if err := JRedis.Redis_hget(Hash_Customer, v, d); err == nil {
			d.LastTime = time.Now().Unix()
			if t, e := time.Parse("2006-01-02 15:04:05", d.CreatTime); e == nil {
				d.CreatStamp = t.Unix()
			}
			JRedis.Redis_hset(Hash_Customer, v, d)
		}
	}
}

func updateSupplier() {
	list, err := JRedis.Redis_hkeys(Hash_Supplier)
	if err != nil {
		return
	}
	for _, v := range list {
		d := &Supplier{}
		if err := JRedis.Redis_hget(Hash_Supplier, v, d); err == nil {
			d.LastTime = time.Now().Unix()
			if t, e := time.Parse("2006-01-02 15:04:05", d.CreatTime); e == nil {
				d.CreatStamp = t.Unix()
			}
			JRedis.Redis_hset(Hash_Supplier, v, d)
		}
	}
}

func updateMaterial() {
	list, err := JRedis.Redis_hkeys(Hash_Material)
	if err != nil {
		return
	}
	for _, v := range list {
		d := &MaterialInfo{}
		if err := JRedis.Redis_hget(Hash_Material, v, d); err == nil {
			d.LastTime = time.Now().Unix()
			if t, e := time.Parse("2006-01-02 15:04:05", d.CreatTime); e == nil {
				d.CreatStamp = t.Unix()
			}
			JRedis.Redis_hset(Hash_Material, v, d)
		}
	}
}
