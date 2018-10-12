#include "goodstable.h"
#include <QDebug>

goodsTable::goodsTable(QWidget *w):M_TableWidget(w)
{
    this->setColumnCount(7);

    //设置表头内容
    QStringList header;
    header<<tr("商品编号")<<tr("商品名称")<<tr("类别")<<tr("规格")\
         <<tr("库存数量")<<tr("单位")<<tr("供应商");
    this->setHorizontalHeaderLabels(header);

    connect(this,SIGNAL(cellPressed(int,int)),this,SLOT(clickRow(int,int)));

    m_menu = new QMenu();
    m_mod  = m_menu->addAction("修改");
    m_del  = m_menu->addAction("删除");
    connect(m_mod,SIGNAL(triggered(bool)),this,SIGNAL(modGoods()));
    connect(m_del,SIGNAL(triggered(bool)),this,SIGNAL(delGoods()));

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
    if (item!=NULL&&!item->text().isEmpty()){
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

    item0->setText(para.ID);
    item1->setText(para.Name);
    item2->setText(para.Type);
    item3->setText(para.Format);
    item4->setText(QString("%1").arg(para.Num));
    item5->setText(para.Unit);
    item6->setText(para.SupplierName);

    item0->setTextAlignment(Qt::AlignCenter);
    item1->setTextAlignment(Qt::AlignCenter);
    item2->setTextAlignment(Qt::AlignCenter);
    item3->setTextAlignment(Qt::AlignCenter);
    item4->setTextAlignment(Qt::AlignCenter);
    item5->setTextAlignment(Qt::AlignCenter);
    item6->setTextAlignment(Qt::AlignCenter);
}
