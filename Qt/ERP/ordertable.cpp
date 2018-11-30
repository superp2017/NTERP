#include "ordertable.h"
#include "datacenter.h"
#include <QDateTime>

#include "dialogorderdetail.h"

OrderTable::OrderTable(QString status, QWidget *w):
    M_TableWidget(w)
{
    this->setColumnCount(18);
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    cutStatus = status;

    //设置表头内容
    QStringList header;
    header<<tr("分厂名称")<<tr("产线名称")<<tr("生产批号")<<tr("订单类型")<<tr("客户名称")<<tr("物料描述")\
         <<tr("订单总量")<<tr("单位")<<tr("未成品")<<tr("已成品")<<tr("未出库")<<tr("已出库")<<tr("客户批次")<<tr("客户备注")<<tr("未税单价(元)")<<tr("未税总价(元)")<<tr("状态")<<tr("创建时间");

    timecol = 17;

    this->setHorizontalHeaderLabels(header);
    this->setSortingEnabled(true);//允许列排序

    connect(this,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(doubleclickRow(int,int)));

    connect(this,SIGNAL(cellPressed(int,int)),this,SLOT(clickRow(int,int)));


    m_menu      = new QMenu();
//    m_new       = new QAction("新建");
    m_mod       = new QAction("修改");
    m_cancle    = new QAction("取消");
    m_produce   = new QAction("生产");
    m_out       = new QAction("出库");
    m_mod_price = new QAction("定价");
    m_del       = new QAction("删除");
    m_giveup    = new QAction("放弃");


//    connect(m_new,SIGNAL(triggered(bool)),this,SIGNAL(newOrder()));
    connect(m_mod,SIGNAL(triggered(bool)),this,SIGNAL(modOrder()));
    connect(m_cancle,SIGNAL(triggered(bool)),this,SIGNAL(cancleOrder()));
    connect(m_produce,SIGNAL(triggered(bool)),this,SIGNAL(produceOrder()));
    connect(m_out,SIGNAL(triggered(bool)),this,SIGNAL(outOrder()));
    connect(m_mod_price,SIGNAL(triggered(bool)),this,SIGNAL(modPrice()));
    connect(m_del,SIGNAL(triggered(bool)),this,SIGNAL(delOrder()));

    if(cutStatus==Status_New){
        this->hideColumn(8);
        this->hideColumn(9);
        this->hideColumn(10);
    }
    if(cutStatus==Status_Produce){
        this->hideColumn(7);
        this->hideColumn(10);
    }
    if(cutStatus==Status_Success){
        this->hideColumn(7);
        this->hideColumn(8);
        this->hideColumn(9);
    }
    checkAuthor(dataCenter::instance()->pub_CurUser().Author);
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
    bool ok = false;
    int count = this->rowCount();
    for(int i=0;i<count;++i){
        QTableWidgetItem *item0 =  this->item(i,2);
        if(item0!=NULL&&item0->text()==para.OrderID){
            setRowData(para,i);
            ok = true;
            break;
        }
    }
    if(!ok){
        appendOrder(para);
    }
}

//移除一行
void OrderTable::removeOrder(Order para)
{
    int count = this->rowCount();
    for(int i=0;i<count;++i){
        QTableWidgetItem *item0 =  this->item(i,2);
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

void OrderTable::checkAuthor(int author)
{
    switch (author) {
    case 0:
        m_del->setEnabled(false);
        m_mod_price->setEnabled(false);
        this->hideColumn(14);
        this->hideColumn(15);
        break;
    case 1:
//        m_new->setEnabled(false);
        m_mod->setEnabled(false);
        m_cancle->setEnabled(false);
        m_produce->setEnabled(false);
        m_out->setEnabled(false);
        m_mod_price->setEnabled(false);
        m_del->setEnabled(false);
        m_giveup->setEnabled(false);
        this->hideColumn(14);
        this->hideColumn(15);
        break;
    case 2:
//        m_new->setEnabled(false);
        m_mod->setEnabled(false);
        m_cancle->setEnabled(false);
        m_produce->setEnabled(false);
        m_out->setEnabled(false);
        m_del->setEnabled(false);
        break;
    default:
        break;
    }
}

void OrderTable::checkOrder(Order order)
{
//    QString status = order.Current.Status;
//    int last = order.

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
    QTableWidgetItem *item15 = this->item(row,15);
    QTableWidgetItem *item16 = this->item(row,16);
    QTableWidgetItem *item17 = this->item(row,17);
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
    if(item15==NULL){
        item15 = new QTableWidgetItem();
        this->setItem(row,15,item15);
    }
    if(item16==NULL){
        item16 = new QTableWidgetItem();
        this->setItem(row,16,item16);
    }

    if(item17==NULL){
        item17 = new QTableWidgetItem();
        this->setItem(row,17,item17);
    }

    item0->setText(para.Factory);
    item1->setText(para.ProductionLine);
    item2->setText(para.OrderID);
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
    item3->setText(type);
    item4->setText(para.CustomName);
    item5->setText(para.MaterielDes);
    item6->setText(QString("%1").arg(para.OrderNum));
    item7->setText(para.Unit);
    item8->setText(QString("%1").arg(para.OrderNum-para.ProduceNum));
    item9->setText(QString("%1").arg(para.ProduceNum));
    item10->setText(QString("%1").arg(para.ProduceNum-para.SuccessNum));
    item11->setText(QString("%1").arg(para.SuccessNum));
    item12->setText(para.CustomBatch);
    item13->setText(para.CustomNote);
    item14->setText(QString("%1").arg(para.Money));
    item15->setText(QString("%1").arg(para.TotleMoney));
    QString status;
    //    if(cutStatus==Status_New){
    //        status="新建";
    //    }
    //    if(cutStatus==Status_Produce){
    //        status="已成品";
    //    }
    //    if(cutStatus==Status_Success){
    //        status="已出库";
    //    }
    //    if(cutStatus==Status_All)
    //    {
    if(para.Current.Status==Status_Cancle){
        status="已取消";
    }
    if(para.Current.Status==Status_PartSuccess){
        status="部分出库";
    }
    if(para.Current.Status==Status_PartProduce){
        status="部分成品";
    }
    if(para.Current.Status==Status_New){
        status="新建";
    }
    if(para.Current.Status==Status_Produce){
        status="全部成品";
    }
    if(para.Current.Status==Status_Success){
        status="全部出库";
    }
    if(para.Current.Status==Status_Part_Part){
        status="部分成品部分出库";
    }
    //    }

    item16->setText(QString("%1").arg(status));
    item17->setText( QDateTime::fromString(para.CreatTime,"yyyy-MM-dd HH:mm:ss").toString("yyyy-MM-dd"));

    item0->setTextAlignment(Qt::AlignCenter);
    item1->setTextAlignment(Qt::AlignCenter);
    item2->setTextAlignment(Qt::AlignCenter);
    item3->setTextAlignment(Qt::AlignCenter);
    item4->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    item5->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    item6->setTextAlignment(Qt::AlignCenter);
    item7->setTextAlignment(Qt::AlignCenter);
    item8->setTextAlignment(Qt::AlignCenter);
    item9->setTextAlignment(Qt::AlignCenter);
    item10->setTextAlignment(Qt::AlignCenter);
    item11->setTextAlignment(Qt::AlignCenter);
    item12->setTextAlignment(Qt::AlignCenter);
    item13->setTextAlignment(Qt::AlignCenter);
    item14->setTextAlignment(Qt::AlignCenter);
    item15->setTextAlignment(Qt::AlignCenter);
    item16->setTextAlignment(Qt::AlignCenter);
    item17->setTextAlignment(Qt::AlignCenter);
}

void OrderTable::setEnable(bool mod, bool cancel, bool produce,  bool out, bool modPrice,bool del)
{

    m_menu->clear();
    int i=0;

//    if(cutStatus==Status_New){
//        if(m_new->isEnabled()){
//            m_menu->addAction(m_new);
//            i++;
//        }
//    }
    if(m_mod->isEnabled()&&mod){
        m_menu->addAction(m_mod);
        i++;
    }
    if(m_cancle->isEnabled()&&cancel) {
        m_menu->addAction(m_cancle);
        i++;
    }
    if(m_produce->isEnabled()&&produce){
        m_menu->addAction(m_produce);
        i++;
    }
    if(m_out->isEnabled()&&out){
        m_menu->addAction(m_out);
        i++;
    }
    if(m_mod_price->isEnabled()&&modPrice){
        m_menu->addAction(m_mod_price);
        i++;
    }
    if(m_del->isEnabled()&&del) {
        m_menu->addAction(m_del);
        i++;
    }
    if(m_giveup->isEnabled()&&i>0){
        m_menu->addAction(m_giveup);
    }
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
                    QTableWidgetItem* item = this->item(row,2);
                    if(item==NULL)
                        return;
                    bool exist =false;
                    Order cur_order = dataCenter::instance()->pub_getOrder(item->text(),exist);
                    if(!exist){
                        return;
                    }
                    //                    if(cutStatus==Status_New){
                    //                        setEnable(true,true,true,false,true,false);
                    //                    }
                    //                    if(cutStatus==Status_Produce){
                    //                        setEnable(false,false,false,true,false,false);
                    //                    }
                    //                    if(cutStatus==Status_Success){
                    //                        setEnable(false,false,false,false,false,false);
                    //                    }

                    //                    if(cutStatus==Status_All){
                    //                        if(cur_order.Current.Status==Status_Cancle){
                    //                    setEnable(false,false,false,false,false,true);
                    // }else{
                    bool produce = cur_order.Current.Status==Status_New||\
                            cur_order.Current.Status==Status_PartProduce||\
                            cur_order.Current.Status==Status_Part_Part;
                    bool out = cur_order.Current.Status==Status_Produce||\
                            cur_order.Current.Status==Status_PartProduce||\
                            cur_order.Current.Status==Status_PartSuccess||\
                            cur_order.Current.Status==Status_Part_Part;
                    bool mod = cur_order.Current.Status==Status_New;
                    bool cancel = cur_order.Current.Status== Status_Cancle;
                    setEnable(mod,mod,produce,out,true,cancel);
                    //      }
                    //                    }
                    if(m_menu->actions().size()>0)
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
    QTableWidgetItem* item = this->item(row,2);
    if(item==NULL)
        return;
    bool exist =false;
    Order cur_order = dataCenter::instance()->pub_getOrder(item->text(),exist);
    if(!exist){
        return;
    }
    DialogOrderDetail order_detail;

    order_detail.init(cur_order);
    if (order_detail.exec()==123){

    }
}

void OrderTable::clickRow(int row, int col)
{
    if(row<0){
        return;
    }
    col =0;
    QTableWidgetItem* item = this->item(row,2);
    if (item!=NULL){
        emit orderClick(item->text());
    }
}






