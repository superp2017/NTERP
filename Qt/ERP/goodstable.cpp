#include "goodstable.h"
#include <QDebug>
#include "datacenter.h"
#include "dialognewgoods.h"
#include <QMessageBox>
#include "boost/thread.hpp"

goodsTable::goodsTable(QWidget *w):M_TableWidget(w)
{
    this->setColumnCount(7);

    //设置表头内容
    QStringList header;
    header<<tr("商品编号")<<tr("商品名称")<<tr("类别")<<tr("规格")<<tr("库存数量")<<tr("单位")<<tr("供应商");
    this->setHorizontalHeaderLabels(header);

    m_menu = new QMenu();
    m_mod  = m_menu->addAction("修改");
    m_del  = m_menu->addAction("删除");
    m_in   = m_menu->addAction("入库");
    m_out  = m_menu->addAction("出库");
    connect(m_mod,SIGNAL(triggered(bool)),this,SLOT(modGoods()));
    connect(m_del,SIGNAL(triggered(bool)),this,SLOT(delGoods()));
    connect(m_in,SIGNAL(triggered(bool)),this,SLOT(inGoods()));
    connect(m_out,SIGNAL(triggered(bool)),this,SLOT(outGoods()));
    connect(dataCenter::instance(),SIGNAL(sig_delGoods(QString,bool)),this,SLOT(delGoodsCb(QString,bool)));
    initGoods(dataCenter::instance()->pub_goods());
}

void goodsTable::updateData()
{
    updataGoods(dataCenter::instance()->pub_goods());
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

void goodsTable::mousePressEvent(QMouseEvent *e)
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
                    QTableWidgetItem* item = this->item(row,0);
                    if(item==NULL||item->text()=="")
                        return;
                    m_menu->exec(e->globalPos());
                }
            }
        }
    }
}


void goodsTable::modGoods()
{
    int row =currentRow();
    if(row<0) return;
    QTableWidgetItem* item = this->item(row,0);
    if (item!=NULL&&!item->text().isEmpty()){
        bool ok = false;
        Goods g = dataCenter::instance()->pub_getGoods(item->text(),ok);
        if(ok){
            DialogNewGoods newGoods;
            newGoods.setModule(false);
            newGoods.initData();
            newGoods.initGoods(g);
            if(newGoods.exec()==123){
                Goods goods = newGoods.getCurGoods();
                this->modGoods(goods);
            }
        }
    }
}

void goodsTable::delGoods()
{
    int row =currentRow();
    if(row<0) return;
    QTableWidgetItem* item = this->item(row,0);
    if (item!=NULL&&!item->text().isEmpty()){
        bool ok = false;
        Goods g  = dataCenter::instance()->pub_getGoods(item->text(),ok);
        if(ok){
            QMessageBox msgBox;
            msgBox.setWindowTitle("提示");
            msgBox.setText("您将删除商品:"+g.Name);
            msgBox.setInformativeText("是否继续操作?");
            msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Ok);
            int ret = msgBox.exec();
            switch (ret) {
            case QMessageBox::Ok:
            {
                boost::thread t(boost::bind(&dataCenter::net_delGoods,dataCenter::instance(),GoodsService::toJsonObject(g)));
                t.detach();
                dataCenter::instance()->pub_showLoadding("正在网络请求...",5000,Qt::black);
                break;
            }
            case QMessageBox::Cancel:
                break;
            default:
                break;
            }
        }
    }
}

void goodsTable::inGoods()
{
    int row =currentRow();
    if(row<0) return;
    QTableWidgetItem* item = this->item(row,0);
    if (item!=NULL&&!item->text().isEmpty()){
        bool ok = false;
        Goods g = dataCenter::instance()->pub_getGoods(item->text(),ok);
        if(ok){
            emit inGoods(g,true);
        }
    }
}

void goodsTable::outGoods()
{
    int row =currentRow();
    if(row<0) return;
    QTableWidgetItem* item = this->item(row,0);
    if (item!=NULL&&!item->text().isEmpty()){
        bool ok = false;
        Goods g = dataCenter::instance()->pub_getGoods(item->text(),ok);
        if(ok){
            emit outGoods(g,true);
        }
    }
}

void goodsTable::delGoodsCb(QString GoodsID,bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        this->removeGoods(GoodsID);
        dataCenter::instance()->pub_showMessage("删除成功",3000);
    }else{
        dataCenter::instance()->pub_showMessage("删除失败",4000);
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
