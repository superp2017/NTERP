package main

import (
	"JGo/JHttp"
	"JGo/JLogger"
	"JGo/JStore/JRedis"
	"time"
)

type StorageOutRecord struct {
	OutID        string  //进出凭证id
	Factory      string  //分厂名称
	Department   string  //部门名称
	CreatDate    string  //领用时间
	CreatStamp   int64   //创建的时间戳
	LastTime     int64   //最后更新时间
	GoodsID      string  //商品ID
	GoodsName    string  //商品名称
	Type         string  //类别
	StrorageName string  //仓库名称
	Nums         float64 //数量
	Unit         string  //单位
	Note         string  //备注
	UserName     string  //领用人姓名
	UserID       string  //领用人工号
}

//新的领用记录
func NewOutRecord(session *JHttp.Session) {
	st := &StorageOutRecord{}
	if err := session.GetPara(st); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.GoodsID == "" || st.GoodsName == "" || st.UserName == "" || st.UserID == "" || st.Nums <= 0 {
		JLogger.Error("NewOutRecord  Parame failed!\n")
		session.Forward("1", "NewOutRecord  Parame failed!\n", nil)
		return
	}
	st.OutID = time.Unix(time.Now().Unix(), 0).Format("20060102150405")
	st.CreatDate = CurDate()
	st.LastTime = CurStamp()
	st.CreatStamp = CurStamp()
	if _, e := inoutGoodsNum(st.GoodsID, st.Nums, false); e != nil {
		session.Forward("1", "NewOutRecord failed:"+e.Error(), nil)
		return
	}

	if err := JRedis.Redis_hset(Hash_StorageOutRecord, st.OutID, st); err != nil {
		session.Forward("1", err.Error(), nil)
		go inoutGoodsNum(st.GoodsID, st.Nums, true)
		return
	}

	//更新
	/////go newUpdate(STRUCT_OUTRECORD, st.OutID, NoticeType_NEW, st)
	go increaseUpdate(STRUCT_OUTRECORD)
	session.Forward("0", "success!\n", st)
}

//获取所有出库记录
func GetAllOutRecord(session *JHttp.Session) {
	st := &AllPara{}
	if err := session.GetPara(st); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	data := []*StorageOutRecord{}
	if st.Type == 1 {
		if st.Stamp > getUpdateStamp(STRUCT_OUTRECORD) {
			session.Forward("0", "success", data)
			return
		}
	}
	list, err := JRedis.Redis_hkeys(Hash_StorageOutRecord)
	if err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}

	for _, v := range list {
		d := &StorageOutRecord{}
		if err := JRedis.Redis_hget(Hash_StorageOutRecord, v, d); err == nil {
			if st.Type == 0 {
				data = append(data, d)
			} else {
				if d.LastTime > st.Stamp {
					data = append(data, d)
				}
			}
		}
	}
	session.Forward("0", "success!\n", data)
}
