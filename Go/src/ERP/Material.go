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
}

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
	go appendCustomerMaterial(st.CID, st.MaterID)

	//通知
	go Notice(&NoticeInfo{
		NoticeType: NoticeType_NEW,
		DataType:   STRUCT_MATERIAL,
		Data:       st,
	})

	session.Forward("0", "success", st)
}

//查询单个物料
func QuertyMaterial(session *JHttp.Session) {
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
		JLogger.Error("QuertyMaterial,MaterID is empty!\n")
		session.Forward("1", "QuertyMaterial,MaterID is empty!\n", nil)
		return
	}
	data, err := getMaterial(st.MaterID)
	if err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	session.Forward("0", "success!\n", data)
}
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
	//通知
	go Notice(&NoticeInfo{
		NoticeType: NoticeType_Modify,
		DataType:   STRUCT_MATERIAL,
		Data:       st,
	})

	session.Forward("0", "success", st)
}
func modMaterialPrice(MaterID string, Money float64) error {
	if MaterID == "" || Money < 0 {
		str := fmt.Sprintf("modMaterialPrice ,MaterID=%s, Money=%d\n", MaterID, Money)
		return JLogger.ErrorLog(str)
	}
	st := &MaterialInfo{}
	if err := JRedis.Redis_hget(Hash_Material, MaterID, st); err != nil {
		JLogger.Error(err.Error())
		return err
	}
	st.Money = Money
	st.LastTime = CurStamp()
	if err := JRedis.Redis_hset(Hash_Material, st.MaterID, st); err != nil {
		JLogger.Error(err.Error())
		return err
	}
	return nil
}
func GetCustomerMaterial(session *JHttp.Session) {
	type Para struct {
		CID string //客户ID
	}
	st := &Para{}
	if err := session.GetPara(st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.CID == "" {
		JLogger.Error("CID is empty!\n")
		session.Forward("1", "CID is empty!\n", nil)
		return
	}
	list := []string{}
	JRedis.Redis_hget(Hash_CustomerMaterial, st.CID, &list)
	session.Forward("0", "success\n", list)
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

	if err := JRedis.Redis_hdel(Hash_Material, st.MaterID); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), st.MaterID)
		return
	}
	go delFromCustomerMaterial(st.CID, st.MaterID)
	//通知
	go Notice(&NoticeInfo{
		NoticeType: NoticeType_Del,
		DataType:   STRUCT_MATERIAL,
		Data:       mater,
	})
	session.Forward("0", "success\n", mater)
}

//添加一个物料标号
func appendCustomerMaterial(CID, MaterID string) error {
	list := []string{}
	JRedis.Redis_hget(Hash_CustomerMaterial, CID, &list)
	list = append(list, MaterID)
	err := JRedis.Redis_hset(Hash_CustomerMaterial, CID, &list)
	return err
}
func delFromCustomerMaterial(CID, MaterID string) error {
	list := []string{}
	JRedis.Redis_hget(Hash_CustomerMaterial, CID, &list)
	index := -1
	for i, v := range list {
		if v == MaterID {
			index = i
			break
		}
	}
	if index != -1 {
		list = append(list[:index], list[index+1:]...)
	}
	return nil
}

//查询单个物料
func getMaterial(MID string) (*MaterialInfo, error) {
	data := &MaterialInfo{}
	err := JRedis.Redis_hget(Hash_Material, MID, data)
	return data, err
}

//获取所有材料
func GetAllMaterial(session *JHttp.Session) {
	list, err := JRedis.Redis_hkeys(Hash_Material)
	data := []*MaterialInfo{}
	if err == nil {
		for _, v := range list {
			d := &MaterialInfo{}
			if e := JRedis.Redis_hget(Hash_Material, v, d); e == nil {
				data = append(data, d)
			}
		}
	}
	session.Forward("0", "success", data)
}
