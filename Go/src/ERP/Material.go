package main

import "JsGo/JsStore/JsRedis"

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
	err := JsRedis.Redis_hset(Hash_Material, id, st)
	return st, err
}

//修改物料
func modMaterial(id, des, cid, name string) error {
	st := &Material{}
	if err := JsRedis.Redis_hget(Hash_Material, id, st); err != nil {
		return err
	}
	st.MaterDes = des
	st.CID = cid
	st.CustomName = name
	if err := JsRedis.Redis_hset(Hash_Material, id, st); err != nil {
		return err
	}
	return nil
}

//删除物料
func successMaterial(id string) error {
	st := &Material{}
	if err := JsRedis.Redis_hget(Hash_Material, id, st); err != nil {
		return err
	}
	st.Status = "1"
	if err := JsRedis.Redis_hset(Hash_Material, id, st); err != nil {
		return err
	}
	return nil
}

//删除物料
func delMaterial(id string) error {
	return JsRedis.Redis_hdel(Hash_Material, id)
}
