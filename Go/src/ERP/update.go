package main

import (
	"sync"
)

type Cache struct {
	Data      interface{}
	TimeStamp int64
	ID        string
}

type DataCache struct {
	Data      map[string]*Cache //具体数据
	StampID   map[int64]string  //时间戳和ID的映射
	IDStamp   map[string]int64  //ID和时间戳的映射
	Stamps    []int64           //时间戳的列表
	LastStamp int64             //最后更新的时间
	Mutex     sync.Mutex        //数据锁

}

func (da *DataCache) updateCache(id string, data interface{}) {
	da.Mutex.Lock()
	defer da.Mutex.Unlock()
	da.LastStamp = CurStamp()
	if _, ok := da.Data[id]; ok {
		delete(da.StampID, da.Data[id].TimeStamp)
		da.Data[id].TimeStamp = da.LastStamp
		da.Stamps = append(da.Stamps, da.LastStamp)
		da.StampID[da.LastStamp] = id
	}
}

func (da *DataCache) modCache(data interface{}) {
	da.Mutex.Lock()
	defer da.Mutex.Unlock()
	da.LastStamp = CurStamp()
	//if _,ok:=[]
}

func init() {

}

//
/////
/////
///////
////
/////
///////
/////////
////
/////
///////
/////////
////
/////
//V/////
////V/////
/////////
/////////
/////////
/////////
////

//

var m_update = map[int]*DataCache{
	//STRTUCT_ORDER:     &DataCache{}, //订单
	//STRUCT_MATERIAL:   &DataCache{}, //物料
	//STRUCT_GOODS:      &DataCache{}, ///商品
	//STRUCT_OUTRECORD:  &DataCache{}, ///出库记录
	//STRUCT_USER:       &DataCache{}, //员工
	//STRUCT_CUSTOMER:   &DataCache{}, //客户
	//STRUCT_SUPPLIER:   &DataCache{}, //供应商
	//STRUCT_UNIT:       &DataCache{}, //单位
	//STRUCT_DEPARTMENT: &DataCache{}, //部门
	//STRUCT_GOODS_TYPE: &DataCache{}, //商品的分类
	//STRUCT_PLANTING:   &DataCache{}, //镀种
}
