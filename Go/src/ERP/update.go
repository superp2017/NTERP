package main

import (
	"JGo/JUtil"
	"sync"
)

type Cache struct {
	Data      interface{}
	TimeStamp int64
	ID        string
}

type DataCache struct {
	Data      map[string]interface{} //具体数据
	StampID   map[int64]string       //时间戳和ID的映射
	IDStamp   map[string]int64       //ID和时间戳的映射
	Stamps    []int64                //时间戳的列表
	LastStamp int64                  //最后更新的时间
	Mutex     sync.Mutex             //数据锁
	Timer     *JUtil.Timer           //定时删除
}

func (da *DataCache) timeout() {
	da.Mutex.Lock()
	defer da.Mutex.Unlock()
	stamp := CurStamp()
	index := -1
	for i, t := range da.Stamps {
		if stamp-t >= 3600 {
			if id, ok := da.StampID[t]; ok {
				delete(da.IDStamp, id)
				delete(da.Data, id)
			}
			delete(da.StampID, t)
			if index == -1 {
				index = i
			}
		}
	}
	da.Stamps = da.Stamps[:index]
}

func (da *DataCache) updateCache(id string, data interface{}) {
	da.Mutex.Lock()
	defer da.Mutex.Unlock()
	da.LastStamp = CurStamp()
	da.Stamps = append(da.Stamps, da.LastStamp)
	if _, ok := da.Data[id]; ok {
		if t, o := da.IDStamp[id]; o {
			delete(da.StampID, t)
		}
		da.StampID[da.LastStamp] = id
	} else {
		da.Data[id] = data
		da.IDStamp[id] = da.LastStamp
		da.StampID[da.LastStamp] = id
	}
}

func (da *DataCache) getData(stamp int64) []interface{} {
	if stamp > da.LastStamp {
		return []interface{}{}
	}
	var res []interface{}
	da.Mutex.Lock()
	defer da.Mutex.Unlock()
	for _, t := range da.Stamps {
		if id, ok := da.StampID[t]; ok {
			if d, o := da.Data[id]; o {
				res = append(res, d)
			}
		}
	}
	return res
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
