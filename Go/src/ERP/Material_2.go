package main

import (
	"JGo/JHttp"
	"JGo/JLogger"
	"JGo/JStore/JRedis"
	"fmt"
)

type MaterialInfo struct {
	MaterID         string //物料编号
	MaterDes        string //物料描述
	CID             string //客户ID
	CustomName      string //客户姓名
	Plating         string //镀种
	Friction        string //摩擦系数
	Thickness       string //厚度
	Salt            string //盐度
	ComponentSolid  string //零件固号
	ComponentFormat string //零件规格
	Factory         string //分厂名称
	FactoryNumber   string //分厂号
	ProductionLine  string //产线名称
	Unit            string //单位
	Money           int    //未税单价
	CreatTime       string //创建时间
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
	if err := JRedis.Redis_hset(Hash_Material, st.MaterID, st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	go appendCustomerMaterial(st.CID, st.MaterID)
	session.Forward("0", "success", st)
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
	if err := JRedis.Redis_hset(Hash_Material, st.MaterID, st); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	session.Forward("0", "success", st)
}

func GtCustomerMaterial(session *JHttp.Session) {
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
	if err := JRedis.Redis_hdel(Hash_Material, st.MaterID); err != nil {
		JLogger.Error(err.Error())
		session.Forward("1", err.Error(), nil)
		return
	}
	go delFromCustomerMaterial(st.CID, st.MaterID)
	session.Forward("0", "success\n", nil)
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
