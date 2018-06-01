package main

import (
	"JGo/JStore/JRedis"
	"JGo/JHttp"
)

type Material struct {
	MaterID    string //物料编号
	MaterDes   string //物料描述
	Unit       string //单位
	OrderNum   int    //订单数量
	CID        string //客户ID
	CustomName string //客户姓名
	Status     string //状态
	CreatTime  string //创建时间
}

func newMaterial(des, unit, cid, customer string, num int) (*Material, error) {
	id := getMaterialID()
	st := &Material{
		MaterID:    id,
		MaterDes:   des,
		Unit:       unit,
		OrderNum:   num,
		CID:        cid,
		CustomName: customer,
		Status:     "0",
		CreatTime:  CurTime(),
	}
	err := JRedis.Redis_hset(Hash_Material, id, st)
	return st, err
}

//修改物料
func modMaterial(id, des, cid, name string) error {
	st := &Material{}
	if err := JRedis.Redis_hget(Hash_Material, id, st); err != nil {
		return err
	}
	st.MaterDes = des
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
func GetAllMaterial(session *JHttp.Session)  {
	list,err:=JRedis.Redis_hkeys(Hash_Material)
	data := []*Material{}
	if err==nil{
		for _,v:=range list{
			d := &Material{}
			if e:=JRedis.Redis_hget(Hash_Material,v,d);e==nil{
				data = append(data,d)
			}
		}
	}
	session.Forward("0","success",data)
}