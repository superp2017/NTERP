package main

//import "sync"
//
//var LastStamp = map[int]int64{
//	STRTUCT_ORDER:    0,
//	STRUCT_MATERIAL:  0,
//	STRUCT_GOODS:     0,
//	STRUCT_OUTRECORD: 0,
//	STRUCT_USER:      0,
//	STRUCT_CUSTOMER:  0,
//	STRUCT_SUPPLIER:  0,
//}
//var updateMutex sync.RWMutex
//
////添加最后更新时间
//func increaseUpdate(datatype int) {
//	updateMutex.Lock()
//	defer updateMutex.Unlock()
//	if _, ok := LastStamp[datatype]; ok {
//		LastStamp[datatype] = CurStamp()
//	}
//}
//
////获取最后最后更新时间
//func getUpdateStamp(datatype int) int64 {
//	updateMutex.RLock()
//	defer updateMutex.RUnlock()
//	if t, ok := LastStamp[datatype]; ok {
//		return t
//	}
//	return 0
//}
