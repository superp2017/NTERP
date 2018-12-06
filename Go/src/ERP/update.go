package main

import "sync"

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
