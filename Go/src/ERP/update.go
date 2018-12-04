package main

import (
	"sync"
)

type DataCache struct {
	Data      map[int64]interface{} //具体数据
	Stamps    []int64               //时间戳的列表
	LastStamp int64                 //最后更新的时间
	Mutex     sync.Mutex            //数据锁
}

func (da *DataCache) newCache(data interface{}) {
	da.Mutex.Lock()
	defer da.Mutex.Unlock()
	da.LastStamp = CurStamp()
	da.Data[da.LastStamp] = data
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
