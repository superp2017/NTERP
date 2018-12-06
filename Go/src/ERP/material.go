package main

import (
	"JGo/JHttp"
	"JGo/JLogger"
	"JGo/JStore/JRedis"
	"fmt"
)

type MaterialInfo struct {
	MaterID         string  //物料编号
	MaterDes        string  //物料描述
	CID             string  //客户ID
	CustomName      string  //客户姓名
	Plating         string  //镀种
	Friction        string  //摩擦系数
	Thickness       string  //厚度
	Salt            string  //盐度
	ComponentSolid  string  //零件固号
	ComponentFormat string  //零件规格
	Factory         string  //分厂名称
	FactoryNumber   string  //分厂号
	ProductionLine  string  //产线名称
	Unit            string  //单位
	Money           float64 //未税单价
	CreatTime       string  //创建时间
	CreatStamp      int64   //创建的时间戳
	LastTime        int64   //最后更新时间
	IsDel           bool    //标记删除
}

//新建物料
func NewMaterial(session *JHttp.Session) {
	st := &MaterialInfo{}
	if err := session.GetPara(st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.CID == "" || st.CustomName == "" || st.MaterDes == "" {
		str := fmt.Sprintf("NewMaterial MaterialInfo 部分参数为空：%v", st)
		JLogger.Error(str)
		session.Forward("1", str, nil)
		return
	}
	st.MaterID = getMaterialID()
	st.CreatTime = CurTime()
	st.CreatStamp = CurStamp()
	st.LastTime = CurStamp()

	if err := JRedis.Redis_hset(Hash_Material, st.MaterID, st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}

	//更新
	increaseUpdate(STRUCT_MATERIAL)

	session.Forward("0", "success", st)
}

//查询单个物料
func QueryMaterial(session *JHttp.Session) {
	type Para struct {
		MaterID string
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.MaterID == "" {
		JLogger.Error("QueryMaterial,MaterID is empty!\n")
		session.Forward("1", "QueryMaterial,MaterID is empty!\n", nil)
		return
	}
	data := &MaterialInfo{}
	if err := JRedis.Redis_hget(Hash_Material, st.MaterID, data); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	session.Forward("0", "success!\n", data)
}

//修改物料
func ModMaterial(session *JHttp.Session) {
	st := &MaterialInfo{}
	if err := session.GetPara(st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.CID == "" || st.CustomName == "" || st.MaterDes == "" {
		str := fmt.Sprintf("ModMaterial ,MaterialInfo 部分参数为空：%v", st)
		JLogger.Error(str)
		session.Forward("1", str, nil)
		return
	}
	st.LastTime = CurStamp()

	if err := JRedis.Redis_hset(Hash_Material, st.MaterID, st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	//更新
	increaseUpdate(STRUCT_MATERIAL)

	session.Forward("0", "success", st)
}

//删除物料
func DelMaterial(session *JHttp.Session) {
	type Para struct {
		CID     string
		MaterID string
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	mater := &MaterialInfo{}
	if err := JRedis.Redis_hget(Hash_Material, st.MaterID, mater); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), st.MaterID)
		return
	}

	mater.IsDel = true

	mater.LastTime = CurStamp()

	if err := JRedis.Redis_hset(Hash_Material, st.MaterID, mater); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), st.MaterID)
		return
	}

	//if err := JRedis.Redis_hdel(Hash_Material, st.MaterID); err != nil {
	//	JLogger.Error(err.Error())
	//	session.Forward("1", err.Error(), st.MaterID)
	//	return
	//}

	//更新
	go increaseUpdate(STRUCT_MATERIAL)
	session.Forward("0", "success\n", mater)
}

//获取所有材料
func GetAllMaterial(session *JHttp.Session) {
	st := &AllPara{}
	if err := session.GetPara(st); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	list, err := JRedis.Redis_hkeys(Hash_Material)
	data := []*MaterialInfo{}
	if st.Type == 1 {
		if st.Stamp > getUpdateStamp(STRUCT_MATERIAL) {
			session.Forward("0", "success", data)
			return
		}
	}

	if err == nil {
		for _, v := range list {
			d := &MaterialInfo{}
			if e := JRedis.Redis_hget(Hash_Material, v, d); e == nil {
				if !d.IsDel {
					if st.Type == 0 {
						data = append(data, d)
					} else {
						if d.LastTime > st.Stamp {
							data = append(data, d)
						}
					}
				}
			}
		}
	}
	session.Forward("0", "success", data)
}
