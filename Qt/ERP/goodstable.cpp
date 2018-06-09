#include "goodstable.h"
#include <QDebug>

goodsTable::goodsTable(QWidget *w):M_TableWidget(w)
{
    this->setColumnCount(12);

    //设置表头内容
    QStringList header;
    header<<tr("商品编号")<<tr("商品名称")<<tr("类别")\
         <<tr("进价(元)")<<tr("库存数量")<<tr("单位")\
        <<tr("库存总价(元)")<<tr("规格")<<tr("颜色")\
       <<tr("供应商")<<tr("状态")<<tr("备注");
    this->setHorizontalHeaderLabels(header);


    connect(this,SIGNAL(cellPressed(int,int)),this,SLOT(clickRow(int,int)));
}


void goodsTable::initGoods(QVector<Goods> list)
{
    removeAllRow();
    for(Goods o:list){
        appendGoods(o);
    }
}

void goodsTable::updataGoods(QVector<Goods> list)
{
    this->setRowCount(list.size());
    for(int i=0;i<list.size();++i){
        setRowData(list.at(i),i);
    }
}

void goodsTable::appendGoods(Goods g)
{
    this->setRowCount(this->rowCount()+1);
    int row = this->rowCount()-1;
    setRowData(g,row);
}

void goodsTable::modGoods(Goods g)
{
    int count = this->rowCount();
    for(int i=0;i<count;++i){
        QTableWidgetItem *item0 =  this->item(i,0);
        if(item0!=NULL&&item0->text()==g.ID){
            setRowData(g,i);
            break;
        }
    }
}

void goodsTable::removeGoods(QString g)
{
    int count = this->rowCount();
    for(int i=0;i<count;++i){
        QTableWidgetItem *item0 =  this->item(i,0);
        if(item0!=NULL&&item0->text()==g){
            this->removeRow(i);
            break;
        }
    }
}

void goodsTable::clickRow(int row, int col)
{
    if(row<0){
        return;
    }

    col =0;
    QTableWidgetItem* item = this->item(row,0);
    if (item!=NULL){
        emit GoodsClick(item->text());
    }
}

void goodsTable::setRowData(Goods para, int row)
{
    QTableWidgetItem *item0 = this->item(row,0);
    QTableWidgetItem *item1 = this->item(row,1);
    QTableWidgetItem *item2 = this->item(row,2);
    QTableWidgetItem *item3 = this->item(row,3);
    QTableWidgetItem *item4 = this->item(row,4);
    QTableWidgetItem *item5 = this->item(row,5);
    QTableWidgetItem *item6 = this->item(row,6);
    QTableWidgetItem *item7 = this->item(row,7);
    QTableWidgetItem *item8 = this->item(row,8);
    QTableWidgetItem *item9 = this->item(row,9);
    QTableWidgetItem *item10 = this->item(row,10);
    QTableWidgetItem *item11 = this->item(row,11);
    if(item0==NULL){
        item0 = new QTableWidgetItem();
        this->setItem(row,0,item0);
    }
    if(item1==NULL){
        item1 = new QTableWidgetItem();
        this->setItem(row,1,item1);
    }
    if(item2==NULL){
        item2 = new QTableWidgetItem();
        this->setItem(row,2,item2);
    }
    if(item3==NULL){
        item3 = new QTableWidgetItem();
        this->setItem(row,3,item3);
    }
    if(item4==NULL){
        item4 = new QTableWidgetItem();
        this->setItem(row,4,item4);
    }
    if(item5==NULL){
        item5 = new QTableWidgetItem();
        this->setItem(row,5,item5);
    }
    if(item6==NULL){
        item6 = new QTableWidgetItem();
        this->setItem(row,6,item6);
    }
    if(item7==NULL){
        item7 = new QTableWidgetItem();
        this->setItem(row,7,item7);
    }
    if(item8==NULL){
        item8 = new QTableWidgetItem();
        this->setItem(row,8,item8);
    }
    if(item9==NULL){
        item9 = new QTableWidgetItem();
        this->setItem(row,9,item9);
    }
    if(item10==NULL){
        item10 = new QTableWidgetItem();
        this->setItem(row,10,item10);
    }
    if(item11==NULL){
        item11 = new QTableWidgetItem();
        this->setItem(row,11,item11);
    }
    item0->setText(para.ID);
    item1->setText(para.Name);
    item2->setText(para.Type);
    item3->setText(QString("%1").arg(para.Price/100.0));
    item4->setText(QString("%1").arg(para.Num));
    item5->setText(para.Unit);
    item6->setText(QString("%1").arg(para.TotalPrice/100.0));
    item7->setText(para.Format);
    item8->setText(para.Color);
    item9->setText(para.SupplierName);
    QString status="已下架";
    if(para.Status=="0"){
        status = "正常";
    }
    item10->setText(status);
    item11->setText(para.Note);

    item0->setTextAlignment(Qt::AlignCenter);
    item1->setTextAlignment(Qt::AlignCenter);
    item2->setTextAlignment(Qt::AlignCenter);
    item3->setTextAlignment(Qt::AlignCenter);
    item4->setTextAlignment(Qt::AlignCenter);
    item5->setTextAlignment(Qt::AlignCenter);
    item6->setTextAlignment(Qt::AlignCenter);
    item7->setTextAlignment(Qt::AlignCenter);
    item8->setTextAlignment(Qt::AlignCenter);
    item9->setTextAlignment(Qt::AlignCenter);
    item10->setTextAlignment(Qt::AlignCenter);
    item11->setTextAlignment(Qt::AlignCenter);
}
