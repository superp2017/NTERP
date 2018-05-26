package main

type Supplier struct {
	SID             string //客户编号
	Name            string //客户公司名称
	Icon            string //客户公司logo
	Addr            string //客户公司地址
	Tel             string //公司电话
	ContactName     string //联系人
	ContactCell     string //联系人电话
	BankName        string //开户行
	BankNumber      string //银行卡号
	Bankbranch      string //银行支行
	CertificatesNum string //税号
	Certificates    string //客户证件（营业执照）
	Note            string //备注
	Goods           string //商品列表
}
