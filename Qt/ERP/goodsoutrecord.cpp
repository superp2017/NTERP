#include "goodsoutrecord.h"
#include "datacenter.h"

Tab_GoodsOutRecord::Tab_GoodsOutRecord(QWidget *w):M_TableWidget(w)
{
    this->setColumnCount(12);

    //设置表头内容
    QStringList header;
    header<<"凭证号"<<"分厂"<<"领用部门"<<"凭证日期"\
         <<"物料号"<<"物料描述"<<"数量"<<"单位"\
        <<"物料组"<<"库存地点"<<"领用人工号"<<"领用人姓名";
    this->setHorizontalHeaderLabels(header);
    initRecords(dataCenter::instance()->pub_AllOutRecord());
}

void Tab_GoodsOutRecord::initRecords(QVector<GoodsOutRecord> list)
{
    removeAllRow();
    for(GoodsOutRecord o:list){
        appendRecord(o);
    }
}

void Tab_GoodsOutRecord::appendRecord(GoodsOutRecord g)
{
    this->setRowCount(this->rowCount()+1);
    int row = this->rowCount()-1;
    setRowData(g,row);
}

void Tab_GoodsOutRecord::removeGoodsRecord(QString g)
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

void Tab_GoodsOutRecord::setRowData(GoodsOutRecord para, int row)
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

    item0->setText(para.OutID);
    item1->setText(para.Factory);
    item2->setText(para.Department);
    item3->setText(para.CreatDate);
    item4->setText(para.GoodsID);
    item5->setText(para.GoodsName);
    item6->setText(QString("%1").arg(para.Nums));
    item7->setText(para.Unit);
    item8->setText(para.Type);
    item9->setText(para.StrorageName);
    item10->setText(para.UserID);
    item11->setText(para.UserName);

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
