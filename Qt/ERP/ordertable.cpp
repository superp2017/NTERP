#include "ordertable.h"
#include "datacenter.h"
#include <QDateTime>

OrderTable::OrderTable(QString status, QWidget *w):
    M_TableWidget(w)
{
    this->setColumnCount(15);
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    cutStatus = status;

    //设置表头内容
    QStringList header;
    header<<tr("分厂名称")<<tr("生产批号")<<tr("订单类型")<<tr("客户名称")<<tr("物料描述")\
         <<tr("订单总量")<<tr("单位")<<tr("已成品")<<tr("已出库")<<tr("客户批次")<<tr("客户备注")<<tr("未税单价(元)")<<tr("未税总价(元)")<<tr("状态")<<tr("创建时间");

    timecol = 14;

    this->setHorizontalHeaderLabels(header);
    this->setSortingEnabled(true);//允许列排序

    order_detail = NULL;
    connect(this,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(doubleclickRow(int,int)));

    connect(this,SIGNAL(cellPressed(int,int)),this,SLOT(clickRow(int,int)));


    m_menu = new QMenu();

    m_new      = m_menu->addAction("新建");
    m_mod      = m_menu->addAction("修改");
    m_cancle   = m_menu->addAction("取消");
    m_produce  = m_menu->addAction("完成生产");
    m_out      = m_menu->addAction("出库");
    m_mod_price= m_menu->addAction("定价");
    m_menu->addAction("放弃");

    connect(m_new,SIGNAL(triggered(bool)),this,SIGNAL(newOrder()));
    connect(m_mod,SIGNAL(triggered(bool)),this,SIGNAL(modOrder()));
    connect(m_cancle,SIGNAL(triggered(bool)),this,SIGNAL(cancleOrder()));
    connect(m_produce,SIGNAL(triggered(bool)),this,SIGNAL(produceOrder()));
    connect(m_out,SIGNAL(triggered(bool)),this,SIGNAL(outOrder()));
    connect(m_mod_price,SIGNAL(triggered(bool)),this,SIGNAL(modPrice()));

    if(cutStatus==Status_New){
        this->hideColumn(7);
        this->hideColumn(8);
    }
    if(cutStatus==Status_Produce){
        this->hideColumn(7);
    }
    if(cutStatus==Status_Success){
        this->hideColumn(8);
    }
    if(cutStatus==Status_All){
        this->hideColumn(13);
    }
}



//初始化所有数据
void OrderTable::initOrder(QVector<Order> list)
{
    removeAllRow();
    for(Order o:list){
        appendOrder(o);
    }
}

//更新所有数据
void OrderTable::updateOrder(QVector<Order> list)
{
    this->setRowCount(list.size());
    for(int i=0;i<list.size();++i){
        setRowData(list.at(i),i);
    }
}



//新建
void OrderTable::appendOrder(Order para)
{
    this->setRowCount(this->rowCount()+1);
    int row=this->rowCount()-1;
    setRowData(para,row);
}

//更新一行
void OrderTable::modOrder(Order para)
{
    int count = this->rowCount();
    for(int i=0;i<count;++i){
        QTableWidgetItem *item0 =  this->item(i,1);
        if(item0!=NULL&&item0->text()==para.OrderID){
            setRowData(para,i);
            break;
        }
    }
}

//移除一行
void OrderTable::removeOrder(Order para)
{
    int count = this->rowCount();
    for(int i=0;i<count;++i){
        QTableWidgetItem *item0 =  this->item(i,1);
        if(item0!=NULL&&item0->text()==para.OrderID){
            this->removeRow(i);
            break;
        }
    }
}

void OrderTable::showAllRow()
{
    int count = this->rowCount();
    for(int i=0;i<count;++i){
        this->setRowHidden(i,false);
    }
}

int  OrderTable::getTimeColNum()
{
    return timecol;
}






//设置表格一行
void OrderTable::setRowData(Order para,int row)
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
    QTableWidgetItem *item12 = this->item(row,12);
    QTableWidgetItem *item13 = this->item(row,13);
    QTableWidgetItem *item14 = this->item(row,14);
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
    if(item11==NULL){
        item11 = new QTableWidgetItem();
        this->setItem(row,11,item11);
    }
    if(item12==NULL){
        item12 = new QTableWidgetItem();
        this->setItem(row,12,item12);
    }
    if(item13==NULL){
        item13 = new QTableWidgetItem();
        this->setItem(row,13,item13);
    }
    if(item14==NULL){
        item14 = new QTableWidgetItem();
        this->setItem(row,14,item14);
    }

    item0->setText(para.Factory);
    item1->setText(para.OrderID);
    QString type;
    if(para.OrderType=="1"){
        type="普通订单";
    }
    if(para.OrderType=="2"){
        type="试样订单";
    }
    if(para.OrderType=="3"){
        type="返工订单";
    }
    item2->setText(type);
    item3->setText(para.CustomName);
    item4->setText(para.MaterielDes);
    item5->setText(QString("%1").arg(para.OrderNum));
    item6->setText(para.Unit);
    item7->setText(QString("%1").arg(para.ProduceNum));
    item8->setText(QString("%1").arg(para.SuccessNum));
    item9->setText(para.CustomBatch);
    item10->setText(para.CustomNote);
    item11->setText(QString("%1").arg(para.Money));
    item12->setText(QString("%1").arg(para.TotleMoney));
    QString status;
    if(para.Current.Status==Status_New){
        status="新建";
    }
    if(para.Current.Status==Status_Produce){
        status="已成品";
    }
    if(para.Current.Status==Status_Success){
        status="已出库";
    }
    if(para.Current.Status==Status_Cancle){
        status="已取消";
    }
    if(para.Current.Status==Status_PartSuccess){
        status="出库中";
    }
    if(para.Current.Status==Status_PartProduce){
        status="生产中";
    }

    item13->setText(QString("%1").arg(status));
    item14->setText( QDateTime::fromString(para.CreatTime,"yyyy-MM-dd HH:mm:ss").toString("yyyy-MM-dd"));

    item0->setTextAlignment(Qt::AlignCenter);
    item1->setTextAlignment(Qt::AlignCenter);
    item2->setTextAlignment(Qt::AlignCenter);
    item3->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    item4->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    item5->setTextAlignment(Qt::AlignCenter);
    item6->setTextAlignment(Qt::AlignCenter);
    item7->setTextAlignment(Qt::AlignCenter);
    item8->setTextAlignment(Qt::AlignCenter);
    item9->setTextAlignment(Qt::AlignCenter);
    item10->setTextAlignment(Qt::AlignCenter);
    item11->setTextAlignment(Qt::AlignCenter);
    item12->setTextAlignment(Qt::AlignCenter);
    item13->setTextAlignment(Qt::AlignCenter);
    item14->setTextAlignment(Qt::AlignCenter);
}


void OrderTable::mousePressEvent(QMouseEvent *e)
{
    QTableWidget::mousePressEvent(e);
    e->accept();
    if(e->buttons()==Qt::RightButton){
        int count =this->rowCount();
        if(count>0){
            if(e->pos().y()<=this->rowHeight(0)*count)  {
                if(this->selectedRanges().size()>0){
                    int row = this->selectedRanges().at(0).topRow();
                    if(row<0){
                        return;
                    }
                    QTableWidgetItem* item = this->item(row,1);
                    if(item==NULL)
                        return;
                    bool exist =false;
                    Order cur_order = dataCenter::instance()->pub_getOrder(item->text(),exist);
                    if(!exist){
                        return;
                    }

                    if(cur_order.Current.Status==Status_New){
                        m_new->setEnabled(true);
                        m_mod->setEnabled(true);
                        m_cancle->setEnabled(true);
                        m_produce->setEnabled(true);
                        m_out->setEnabled(false);
                        m_mod_price->setEnabled(true);
                    }
                    if(cur_order.Current.Status==Status_Produce){
                        m_new->setEnabled(false);
                        m_mod->setEnabled(false);
                        m_cancle->setEnabled(false);
                        m_produce->setEnabled(false);
                        m_out->setEnabled(true);
                        m_mod_price->setEnabled(false);
                    }
                    if(cur_order.Current.Status==Status_Cancle){
                        m_new->setEnabled(true);
                        m_mod->setEnabled(false);
                        m_cancle->setEnabled(false);
                        m_produce->setEnabled(false);
                        m_out->setEnabled(false);
                        m_mod_price->setEnabled(false);
                    }
                    if(cur_order.Current.Status==Status_Success){
                        m_new->setEnabled(true);
                        m_mod->setEnabled(false);
                        m_cancle->setEnabled(false);
                        m_produce->setEnabled(false);
                        m_out->setEnabled(false);
                        m_mod_price->setEnabled(false);
                    }
                    m_menu->exec(e->globalPos());
                }
            }
        }
    }

}


void OrderTable::doubleclickRow(int row, int ool)
{
    if(row<0){
        return;
    }
    this->checkSelect();
    ool = 0;
    QTableWidgetItem* item = this->item(row,1);
    if(item==NULL)
        return;
    bool exist =false;
    Order cur_order = dataCenter::instance()->pub_getOrder(item->text(),exist);
    if(!exist){
        return;
    }
    if(order_detail ==NULL){
        order_detail = new DialogOrderDetail();
    }
    order_detail->init(cur_order);
    if (order_detail->exec()==123){

    }
}

void OrderTable::clickRow(int row, int col)
{
    if(row<0){
        return;
    }
    col =0;
    QTableWidgetItem* item = this->item(row,1);
    if (item!=NULL){
        emit orderClick(item->text());
    }
}






