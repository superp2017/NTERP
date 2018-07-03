package main

import (
	"JGo/JHttp"
	"JGo/JStore/JRedis"
)

type Material struct {
	MaterID         string  //物料编号
	MaterDes        string  //物料描述
	Plating         string  //镀种
	Friction        string  //摩擦系数
	Thickness       string  //厚度
	Salt            string  //盐度
	ComponentSolid  string  //零件固号
	ComponentFormat string  //零件规格
	Unit            string  //单位
	OrderNum        float64 //订单数量
	CID             string  //客户ID
	CustomName      string  //客户姓名
	Status          string  //状态
	CreatTime       string  //创建时间
}

func newMaterial(des, plating, friction, thickness, salt, solid, format, unit, cid, customer string, num float64) (*Material, error) {
	id := getMaterialID()
	st := &Material{
		MaterID:         id,
		MaterDes:        des,
		Plating:         plating,
		Friction:        friction,
		Thickness:       thickness,
		Salt:            salt,
		ComponentSolid:  solid,
		ComponentFormat: format,
		Unit:            unit,
		OrderNum:        num,
		CID:             cid,
		CustomName:      customer,
		Status:          "0",
		CreatTime:       CurTime(),
	}
	err := JRedis.Redis_hset(Hash_Material, id, st)
	return st, err
}

//修改物料
func modMaterial(id, des, plating, friction, thickness, salt, solid, format, cid, name string) error {
	st := &Material{}
	if err := JRedis.Redis_hget(Hash_Material, id, st); err != nil {
		return err
	}
	st.MaterDes = des
	st.Plating = plating
	st.Friction = friction
	st.Thickness = thickness
	st.Salt = salt
	st.ComponentSolid = solid
	st.ComponentFormat = format
	st.CID = cid
	st.CustomName = name
	if err := JRedis.Redis_hset(Hash_Material, id, st); err != nil {
		return err
	}
	return nil
}

//完成物料
func successMaterial(id string) error {
	st := &Material{}
	if err := JRedis.Redis_hget(Hash_Material, id, st); err != nil {
		return err
	}
	st.Status = "1"
	if err := JRedis.Redis_hset(Hash_Material, id, st); err != nil {
		return err
	}
	return nil
}

//取消物料
func cancleMaterial(id string) error {
	st := &Material{}
	if err := JRedis.Redis_hget(Hash_Material, id, st); err != nil {
		return err
	}
	st.Status = "-1"
	if err := JRedis.Redis_hset(Hash_Material, id, st); err != nil {
		return err
	}
	return nil
}

//删除物料
func delMaterial(id string) error {
	return JRedis.Redis_hdel(Hash_Material, id)
}

//获取所有供应商列表
func GetAllMaterial(session *JHttp.Session) {
	list, err := JRedis.Redis_hkeys(Hash_Material)
	data := []*Material{}
	if err == nil {
		for _, v := range list {
			d := &Material{}
			if e := JRedis.Redis_hget(Hash_Material, v, d); e == nil {
				data = append(data, d)
			}
		}
	}
	session.Forward("0", "success", data)
}
