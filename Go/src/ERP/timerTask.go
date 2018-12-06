package main

import "time"

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

func forreachOrder() {

}

func foreachUser() {

}

func foreachGoods() {

}

func foreachGoodsOut() {

}

func foreachCustomer() {

}

func foreachSupplier() {

}

func foreachMaterial() {

}
