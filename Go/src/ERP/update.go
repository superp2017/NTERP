package main

import (
	"JGo/JHttp"
	"JGo/JUtil"
	"sort"
	"sync"
	"time"
)

//更新类方式
const (
	NoticeType_NEW    = 0 //新建
	NoticeType_Modify = 1 //修改
	NoticeType_Del    = 2 //删除
)

//更新的数据类型
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

type CacheInfo struct {
	Data       interface{}
	TimeStamp  int64
	ID         string
	UpDateType int
}

type DataCache struct {
	Data      []*CacheInfo //具体数据
	LastStamp int64        //最后更新的时间
	Mutex     sync.Mutex   //数据锁
	Timer     *JUtil.Timer //定时删除
}

//超时检测
func (da *DataCache) timeout(d interface{}) {
	da.Mutex.Lock()
	defer da.Mutex.Unlock()
	stamp := CurStamp()
	defer func() {
		da.LastStamp = stamp
		da.Timer.Reset(time.Duration(30) * time.Minute)
	}()
	if len(da.Data) == 0 {
		return
	}

	if stamp-da.LastStamp >= 3600 {
		da.Data = []*CacheInfo{}
	} else {
		index := -1
		for i, info := range da.Data {
			if stamp-info.TimeStamp >= 3600 {
				index = i
			}
		}
		da.Data = da.Data[:index]
	}
}

//更新缓存
func (da *DataCache) updateCache(id string, ut int, data interface{}) {
	da.Mutex.Lock()
	defer da.Mutex.Unlock()
	da.LastStamp = CurStamp()
	exist := false
	for _, info := range da.Data {
		if info.ID == id {
			info.Data = data
			info.TimeStamp = da.LastStamp
			info.ID = id
			info.UpDateType = ut
			exist = true
			break
		}
	}
	if !exist {
		da.Data = append(da.Data, &CacheInfo{
			Data:       data,
			TimeStamp:  da.LastStamp,
			ID:         id,
			UpDateType: ut,
		})
	} else {
		sort.Slice(da.Data, func(i, j int) bool {
			return da.Data[i].TimeStamp >= da.Data[j].TimeStamp
		})
	}
}

//获取数据
func (da *DataCache) getData(stamp int64) []*CacheInfo {
	da.Mutex.Lock()
	defer da.Mutex.Unlock()

	if stamp > da.LastStamp || len(da.Data) == 0 {
		return []*CacheInfo{}
	}
	index := 0
	for i, info := range da.Data {
		if info.TimeStamp < stamp {
			index = i
		}
	}
	return da.Data[:index]
}

var mCache map[int]*DataCache //所有缓存数据
var mutex sync.RWMutex

func startCache() {
	mCache = make(map[int]*DataCache)
	for i := 0; i < 7; i++ {
		ca := &DataCache{}
		ca.Timer = JUtil.NewTimer(time.Duration(30)*time.Minute, ca.timeout, 0)
		mCache[i] = ca
	}
}

func HeartBeat(session *JHttp.Session) {
	type Param struct {
		DataType int
		TimStamp int64
	}
	st := &Param{}
	if err := session.GetPara(st); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.TimStamp < 0 || st.DataType < 0 {
		session.Forward("1", "param error!", nil)
		return
	}

	list := GetUpdate(st.DataType, st.TimStamp)
	if len(list) > 0 {
		session.Forward("0", "success", list)
		return
	}
	session.Forward("1", "empty list", nil)
}

func newUpdate(st int, id string, ut int, d interface{}) {
	if c, ok := mCache[st]; ok {
		go c.updateCache(id, ut, d)
	}
}

func GetUpdate(st int, stamp int64) []*CacheInfo {
	mutex.RLock()
	defer mutex.RUnlock()
	if ca, ok := mCache[st]; ok {
		return ca.getData(stamp)
	}
	return []*CacheInfo{}
}
