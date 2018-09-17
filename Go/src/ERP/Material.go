package main

//
//import (
//	"JGo/JHttp"
//	"JGo/JLogger"
//	"JGo/JStore/JRedis"
//)
//
//type Material struct {
//	MaterID         string //物料编号
//	MaterDes        string //物料描述
//	Plating         string //镀种
//	Friction        string //摩擦系数
//	Thickness       string //厚度
//	Salt            string //盐度
//	ComponentSolid  string //零件固号
//	ComponentFormat string //零件规格
//	Factory         string //分厂名称
//	FactoryNumber   string //分厂号
//	ProductionLine  string //产线名称
//	OrderNum        int    //订单数量
//	Unit            string //单位
//	Money           int    //价格
//	TotleMoney      int    //总价
//	CID             string //客户ID
//	CustomName      string //客户姓名
//	Status          string //状态
//	CreatTime       string //创建时间
//}
//
//func newMaterial(des, plating, friction, thickness, salt, solid, format,
//	cid, customer, factory, factorynumber, productionline, unit string, ordernum, money int) (*Material, error) {
//	id := getMaterialID()
//	st := &Material{
//		MaterID:         id,
//		MaterDes:        des,
//		Plating:         plating,
//		Friction:        friction,
//		Thickness:       thickness,
//		Salt:            salt,
//		ComponentSolid:  solid,
//		ComponentFormat: format,
//		Factory:         factory,
//		FactoryNumber:   factorynumber,
//		ProductionLine:  productionline,
//		Unit:            unit,
//		OrderNum:        ordernum,
//		Money:           money,
//		CID:             cid,
//		CustomName:      customer,
//		Status:          "0",
//		CreatTime:       CurTime(),
//	}
//	st.TotleMoney = st.OrderNum * st.Money / 100
//	err := JRedis.Redis_hset(Hash_Material, id, st)
//	return st, err
//}
//
////修改物料
//func modMaterial(id, des, plating, friction, thickness, salt, solid, format, cid, name, productionline, unit string, ordernum, money int) error {
//	st := &Material{}
//	if err := JRedis.Redis_hget(Hash_Material, id, st); err != nil {
//		return err
//	}
//	st.MaterDes = des
//	st.Plating = plating
//	st.Friction = friction
//	st.Thickness = thickness
//	st.Salt = salt
//	st.ComponentSolid = solid
//	st.ComponentFormat = format
//	st.CID = cid
//	st.CustomName = name
//	st.ProductionLine = productionline
//	st.Unit = unit
//	st.OrderNum = ordernum
//	st.Money = money
//	st.TotleMoney = st.OrderNum * st.Money / 100
//	if err := JRedis.Redis_hset(Hash_Material, id, st); err != nil {
//		return err
//	}
//	return nil
//}
//
////完成物料
//func successMaterial(id string) error {
//	st := &Material{}
//	if err := JRedis.Redis_hget(Hash_Material, id, st); err != nil {
//		return err
//	}
//	st.Status = "1"
//	if err := JRedis.Redis_hset(Hash_Material, id, st); err != nil {
//		return err
//	}
//	return nil
//}
//
////取消物料
//func cancleMaterial(id string) error {
//	st := &Material{}
//	if err := JRedis.Redis_hget(Hash_Material, id, st); err != nil {
//		return err
//	}
//	st.Status = "-1"
//	if err := JRedis.Redis_hset(Hash_Material, id, st); err != nil {
//		return err
//	}
//	return nil
//}
//
////删除物料
//func delMaterial(id string) error {
//	return JRedis.Redis_hdel(Hash_Material, id)
//}
//
////获取所有供应商列表
//func GetAllMaterial(session *JHttp.Session) {
//	list, err := JRedis.Redis_hkeys(Hash_Material)
//	data := []*Material{}
//	if err == nil {
//		for _, v := range list {
//			d := &Material{}
//			if e := JRedis.Redis_hget(Hash_Material, v, d); e == nil {
//				data = append(data, d)
//			}
//		}
//	}
//	session.Forward("0", "success", data)
//}
//
//func updateAllMaterial(session *JHttp.Session) {
//	if err := updateallAaterial(); err != nil {
//		session.Forward("1", err.Error(), nil)
//		return
//	}
//	session.Forward("0", "success!", nil)
//}
//
//func updateallAaterial() error {
//	list, err := JRedis.Redis_hkeys(Hash_Order)
//	if err != nil {
//		return err
//	}
//	for _, v := range list {
//		if v == Key_LastOrderDate {
//			continue
//		}
//		d := &Order{}
//		if err := JRedis.Redis_hget(Hash_Order, v, d); err == nil {
//			if d.Current.Status != Status_Del {
//				go modMa(d.MaterielID, d.MaterielDes, d.Plating, d.Friction,
//					d.Thickness, d.Salt, d.ComponentSolid, d.ComponentFormat,
//					d.Factory, d.FactoryNumber, d.ProductionLine, d.CustomID, d.CustomName,
//					d.Unit, d.OrderNum, d.Money, d.TotleMoney)
//			}
//		}
//	}
//	return nil
//}
//
//func modMa(mid, des, plat, fric, thick, salt, solid, format, factory, factorynumber, productionline, cid, custommer, unit string, ordernum, money, totlemoney int) error {
//	st := &Material{}
//	if err := JRedis.Redis_hget(Hash_Material, mid, st); err != nil {
//		return err
//	}
//	JLogger.Error(mid)
//	st.MaterDes = des
//	st.Plating = plat
//	st.Friction = fric
//	st.Thickness = thick
//	st.Salt = salt
//	st.ComponentSolid = solid
//	st.ComponentFormat = format
//	st.Factory = factory
//	st.FactoryNumber = factorynumber
//	st.ProductionLine = productionline
//	st.CID = cid
//	st.CustomName = custommer
//	st.Unit = unit
//	st.OrderNum = ordernum
//	st.Money = money
//	st.TotleMoney = totlemoney
//	if err := JRedis.Redis_hset(Hash_Material, mid, st); err != nil {
//		return err
//	}
//	return nil
//}
