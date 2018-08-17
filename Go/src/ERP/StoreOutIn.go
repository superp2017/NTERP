package main

import "JGo/JHttp"

type StorageOutIn struct {
	OutID          string //进出凭证id
	Factory        string //分厂名称
	FactoryNumber  string //分厂号
	ProductionLine string //产线名称
	CreatDate      string //领用时间
	MaterielID     string //材料id
	MaterielDes    string //材料描述
	MaterielGroup  string //物料组（分类）
	StrorageName   string //仓库名称
	Nums           int    //数量
	Unit           string //单位
	Note           string //备注
	UserName       string //领用人姓名
	UserID         string //领用人工号
}

func NewOutRecord(session *JHttp.Session) {
	st := &StorageOutIn{}
	if err := session.GetPara(st); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}

	//Hash_StorageOutIn
}
