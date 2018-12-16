package main

import (
	"JGo/JStore/JRedis"
	"time"
)

////启动定时任务
func StartTimer() {
	go timeTask(delReal, 2)
}

////定时任务
func timeTask(task func(), hour int) {
	for {
		//////////定时任务//////////////////
		now := time.Now()
		// 计算下一个零点
		next := now.Add(time.Hour * 24)
		next = time.Date(next.Year(), next.Month(), next.Day(), hour, 0, 0, 0, next.Location())
		t := time.NewTimer(next.Sub(now))
		<-t.C
		task()
	}
}

//删除那边标记删除的数据
func delReal() {
	go forreachOrder()
	go foreachUser()
	go foreachGoods()
	go foreachGoodsOut()
	go foreachCustomer()
	go foreachSupplier()
	go foreachMaterial()
}

//默认删除7天内的标记的订单
func forreachOrder() {
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
			if d.IsDel {
				if d.LastTime-CurStamp() >= 3600*24*7 {
					go JRedis.Redis_hdel(Hash_Order, v)
				}
			}
		}
	}
}

func foreachUser() {
	list, err := JRedis.Redis_hkeys(Hash_Employee)
	if err != nil {
		return
	}
	for _, v := range list {
		d := &Employee{}
		if err := JRedis.Redis_hget(Hash_Employee, v, d); err == nil {
			if d.IsDel {
				if d.LastTime-CurStamp() >= 3600*24 {
					go JRedis.Redis_hdel(Hash_Employee, v)
				}
			}
		}
	}
}

func foreachGoods() {
	list, err := JRedis.Redis_hkeys(Hash_Goods)
	if err != nil {
		return
	}
	for _, v := range list {
		d := &Goods{}
		if err := JRedis.Redis_hget(Hash_Goods, v, d); err == nil {
			if d.IsDel {
				if d.LastTime-CurStamp() >= 3600*24 {
					go JRedis.Redis_hdel(Hash_Goods, v)
				}
			}
		}
	}
}

func foreachGoodsOut() {
	list, err := JRedis.Redis_hkeys(Hash_StorageOutRecord)
	if err != nil {
		return
	}
	for _, v := range list {
		d := &StorageOutRecord{}
		if err := JRedis.Redis_hget(Hash_StorageOutRecord, v, d); err == nil {
			if d.IsDel {
				if d.LastTime-CurStamp() >= 3600*24 {
					go JRedis.Redis_hdel(Hash_StorageOutRecord, v)
				}
			}
		}
	}
}

func foreachCustomer() {
	list, err := JRedis.Redis_hkeys(Hash_Customer)
	if err != nil {
		return
	}
	for _, v := range list {
		d := &Customer{}
		if err := JRedis.Redis_hget(Hash_Customer, v, d); err == nil {
			if d.IsDel {
				if d.LastTime-CurStamp() >= 3600*24 {
					go JRedis.Redis_hdel(Hash_Customer, v)
				}
			}
		}
	}
}

func foreachSupplier() {
	list, err := JRedis.Redis_hkeys(Hash_Supplier)
	if err != nil {
		return
	}
	for _, v := range list {
		d := &Supplier{}
		if err := JRedis.Redis_hget(Hash_Supplier, v, d); err == nil {
			if d.IsDel {
				if d.LastTime-CurStamp() >= 3600*24 {
					go JRedis.Redis_hdel(Hash_Supplier, v)
				}
			}
		}
	}
}

func foreachMaterial() {
	list, err := JRedis.Redis_hkeys(Hash_Material)
	if err != nil {
		return
	}
	for _, v := range list {
		d := &MaterialInfo{}
		if err := JRedis.Redis_hget(Hash_Material, v, d); err == nil {
			if d.IsDel {
				if d.LastTime-CurStamp() >= 3600*48 {
					go JRedis.Redis_hdel(Hash_Material, v)
				}
			}
		}
	}
}
