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
	//通知
	go	Notice(&NoticeInfo{
		NoticeType:NoticeType_NEW,
		DataType:STRUCT_OUTRECORD,
		Data:st,
	})
	session.Forward("0", "success!\n", st)
}

//获取所有出库记录
func GetAllOutRecord(session *JHttp.Session) {
	list, err := JRedis.Redis_hkeys(Hash_StorageOutRecord)
	if err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	data := []*StorageOutRecord{}
	for _, v := range list {
		d := &StorageOutRecord{}
		if err := JRedis.Redis_hget(Hash_StorageOutRecord, v, d); err == nil {
			data = append(data, d)
		}
	}
	session.Forward("0", "success!\n", data)
}
