#include "ordertable.h"
#include "datacenter.h"

OrderTable::OrderTable(QTableWidget *w):
    QTableWidget(w)
{
    this->setColumnCount(9);
    this->setEditTriggers(QTableWidget::NoEditTriggers);
    this->setSelectionBehavior ( QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    this->setSelectionMode ( QAbstractItemView::SingleSelection); //设置选择模式，选择单行
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //    this->horizontalHeader()->setFixedHeight(25); //设置表头的高度
    this->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); //设置表头背景色
    QFont font = this->horizontalHeader()->font();
    font.setBold(true);
    this->horizontalHeader()->setFont(font);

    //设置表头内容
    QStringList header;
    header<<tr("生产订单")<<tr("物料编码")<<tr("物料描述")\
         <<tr("订单数量")<<tr("单位")<<tr("客户批次")<<tr("客户备注")<<tr("生产批号")<<tr("价格");
    this->setHorizontalHeaderLabels(header);

    this->setSortingEnabled(true);//允许列排序

    order_detail = NULL;
    connect(this,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(clickRow(int,int)));

}


//新建
void OrderTable::appendOrder(Order para)
{
    this->setRowCount(this->rowCount()+1);
    int row=this->rowCount()-1;
    setRowData(para,row);
}



//设置表头模式
void OrderTable::setHeaderColModel(QHeaderView::ResizeMode mode)
{
    this->horizontalHeader()->setSectionResizeMode(mode);
}

//设置表格一行
void OrderTable::setRowData(Order para,int row)
{

    QTableWidgetItem *item1 = this->item(row,0);
    QTableWidgetItem *item2 = this->item(row,1);
    QTableWidgetItem *item3 = this->item(row,2);
    QTableWidgetItem *item4 = this->item(row,3);
    QTableWidgetItem *item5 = this->item(row,4);
    QTableWidgetItem *item6 = this->item(row,5);
    QTableWidgetItem *item7 = this->item(row,6);
    QTableWidgetItem *item8 = this->item(row,7);
    QTableWidgetItem *item9 = this->item(row,8);
    if(item1==NULL){
        item1 = new QTableWidgetItem();
        this->setItem(row,0,item1);
    }
    if(item2==NULL){
        item2 = new QTableWidgetItem();
        this->setItem(row,1,item2);
    }
    if(item3==NULL){
        item3 = new QTableWidgetItem();
        this->setItem(row,2,item3);
    }
    if(item4==NULL){
        item4 = new QTableWidgetItem();
        this->setItem(row,3,item4);
    }
    if(item5==NULL){
        item5 = new QTableWidgetItem();
        this->setItem(row,4,item5);
    }
    if(item6==NULL){
        item6 = new QTableWidgetItem();
        this->setItem(row,5,item6);
    }
    if(item7==NULL){
        item7 = new QTableWidgetItem();
        this->setItem(row,6,item7);
    }
    if(item8==NULL){
        item8 = new QTableWidgetItem();
        this->setItem(row,7,item8);
    }
    if(item9==NULL){
        item9 = new QTableWidgetItem();
        this->setItem(row,8,item9);
    }


    item1->setText(para.OrderID);
    item2->setText(para.MaterielID);
    item3->setText(para.MaterielDes);
    item4->setText(QString("%1").arg(para.OrderNum));
    item5->setText(para.Unit);
    item6->setText(para.CustomBatch);
    item7->setText(para.CustomNote);
    item8->setText(para.ProduceID);
    item9->setText(QString("%1").arg(para.Money));


    item1->setTextAlignment(Qt::AlignCenter);
    item2->setTextAlignment(Qt::AlignCenter);
    item3->setTextAlignment(Qt::AlignCenter);
    item4->setTextAlignment(Qt::AlignCenter);
    item5->setTextAlignment(Qt::AlignCenter);
    item6->setTextAlignment(Qt::AlignCenter);
    item7->setTextAlignment(Qt::AlignCenter);
    item8->setTextAlignment(Qt::AlignCenter);
    item9->setTextAlignment(Qt::AlignCenter);
}


void OrderTable::removeAllRow()
{
    int count = this->rowCount();
    for(int i=0;i<count;++i){
        this->removeRow(0);
    }
}

void OrderTable::clickRow(int row, int ool)
{
    if(row<0){
        return;
    }
    ool = 0;
    QTableWidgetItem* item = this->item(row,0);
    cur_order = dataCenter::instance()->getOrder(item->text());
    if(order_detail ==NULL){
        order_detail = new DialogOrderDetail();
    }
    order_detail->init(cur_order);

    if (order_detail->exec()==123){

    }
}








