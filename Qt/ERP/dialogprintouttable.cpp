#include "dialogprintouttable.h"
#include "ui_dialogprintouttable.h"
#include "datacenter.h"
#include <QMessageBox>
#include "boost/thread.hpp"

DialogPrintOutTable::DialogPrintOutTable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPrintOutTable)
{
    ui->setupUi(this);

    ui->tableWidget->setColumnCount(7);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    //设置表头内容
    QStringList header;
    header<<tr("生产批号")<<tr("物料描述")<<tr("订单数量")<<tr("单位")<<tr("打印次数")<<tr("客户名称")<<tr("备注");
    ui->tableWidget->setHorizontalHeaderLabels(header);

    connect(&m_printer,SIGNAL(updateOrderPrintNum(QVector<Order>)),this,SLOT(updatePrintNum(QVector<Order>)));
    connect(dataCenter::instance(),SIGNAL(sig_updatePrintNum(QVector<Order>,bool)),this,SLOT(updatePrintNumCb(QVector<Order>,bool)));

    for(Order o:dataCenter::instance()->pub_StatusOrders(Status_Success)){
        appendOrder(o);
    }
    dataCenter::instance()->net_getPrintNumber();
}

DialogPrintOutTable::~DialogPrintOutTable()
{
    delete ui;
}

QVector<Order> DialogPrintOutTable::getSelectOrder()
{
    QVector<Order> data;
    for (QCheckBox* box:m_boxs){
        if(box->isChecked()){
            data.push_back(m_data[box]);
        }
    }
    return data;
}

void DialogPrintOutTable::clearSelect()
{
    for(QCheckBox*box:m_boxs){
        box->setChecked(false);
    }
    ui->tableWidget->clearSelection();
    ui->tableWidget->checkSelect();
}

void DialogPrintOutTable::on_pushButton_preview_clicked()
{
    QVector<Order> ls = getSelectOrder();
    if(ls.size()==0){
        QMessageBox::warning(this,"提示","请至少选择一个订单打印!");
        return ;
    }
    m_printer.setData(ls,dataCenter::instance()->put_getPrintNumber());
    m_printer.doPreview(this);

}

void DialogPrintOutTable::on_pushButton_print_clicked()
{
    QVector<Order> ls = getSelectOrder();
    if(ls.size()==0){
        QMessageBox::warning(this,"提示","请至少选择一个订单打印!");
        return ;
    }
    m_printer.setData(ls,dataCenter::instance()->put_getPrintNumber());
    m_printer.doPrint(this);
}

void DialogPrintOutTable::appendOrder(Order para)
{
    ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
    int row=ui->tableWidget->rowCount()-1;
    setRowData(para,row);
}

void DialogPrintOutTable::checkboxChecked()
{
    QCheckBox* b =  reinterpret_cast<QCheckBox *>(sender());
    QString customName = m_data[b].CustomName;

    int selected = 0;
    for (QCheckBox* box:m_boxs){
        if(b!=box&&box->isChecked()){
            if(m_data[box].CustomName!=customName){
                b->setChecked(false);
                QMessageBox::warning(this,"提示","不同公司的送货单不能同时打印!");
                return ;
            }
            selected++;
        }
    }
    if (selected>9){
        QMessageBox::warning(this,"提示","打印数量上限!");
        b->setChecked(false);
    }

}

void DialogPrintOutTable::updatePrintNum(QVector<Order> list)
{
    QJsonArray arr;
    for (Order o:list){
        arr.append(o.OrderID);
    }
    QJsonObject obj;
    obj.insert("Orders",arr);
    obj.insert("UserID",dataCenter::instance()->pub_CurUser().UID);
    obj.insert("UserName",dataCenter::instance()->pub_CurUser().Name);

    boost::thread t(boost::bind(&dataCenter::net_updatePrintNum,dataCenter::instance(),obj));
    t.detach();

    dataCenter::instance()->pub_showLoadding("正在网络请求...",5000,Qt::black);
}

void DialogPrintOutTable::updatePrintNumCb(QVector<Order> list, bool ok)
{
    clearSelect();
    dataCenter::instance()->pub_hideLoadding();
    int row =  ui->tableWidget->rowCount();
    for(Order o:list){
        for(int i=0;i<row;++i){
            QWidget *w=  ui->tableWidget->cellWidget(i,0);
            if(w!=NULL){
                QCheckBox * box =reinterpret_cast<QCheckBox *>(w);
                if(box->text()==o.OrderID){
                    setRowData(o,i);
                }
            }
        }
    }
}

void DialogPrintOutTable::setRowData(Order para, int row)
{
    QWidget *item0 = ui->tableWidget->cellWidget(row,0);
    QTableWidgetItem *item1 = ui->tableWidget->item(row,1);
    QTableWidgetItem *item2 = ui->tableWidget->item(row,2);
    QTableWidgetItem *item3 = ui->tableWidget->item(row,3);
    QTableWidgetItem *item4 = ui->tableWidget->item(row,4);
    QTableWidgetItem *item5 = ui->tableWidget->item(row,5);
    QTableWidgetItem *item6 = ui->tableWidget->item(row,6);

    if(item0==NULL){
        ui->tableWidget->setCellWidget(row,0,item0);
        QCheckBox* box  = new QCheckBox(para.OrderID);
        ui->tableWidget->setCellWidget(row,0,box);
        m_boxs.push_back(box);
        m_data[box] = para;
        connect(box,SIGNAL(clicked(bool)),this,SLOT(checkboxChecked()));
    }else{
        QCheckBox * box = reinterpret_cast<QCheckBox *>(item0);
        m_data[box] = para;
    }

    if(item1==NULL){
        item1 = new QTableWidgetItem();
        ui->tableWidget->setItem(row,1,item1);
    }
    if(item2==NULL){
        item2 = new QTableWidgetItem();
        ui->tableWidget->setItem(row,2,item2);
    }
    if(item3==NULL){
        item3 = new QTableWidgetItem();
        ui->tableWidget->setItem(row,3,item3);
    }
    if(item4==NULL){
        item4 = new QTableWidgetItem();
        ui->tableWidget->setItem(row,4,item4);
    }
    if(item5==NULL){
        item5 = new QTableWidgetItem();
        ui->tableWidget->setItem(row,5,item5);
    }
    if(item6==NULL){
        item6 = new QTableWidgetItem();
        ui->tableWidget->setItem(row,6,item6);
    }
    item1->setText(para.MaterielDes);
    item2->setText(QString("%1").arg(para.SuccessNum));
    item3->setText(para.Unit);
    item4->setText(QString("%1").arg(para.PrintNum));
    item5->setText(para.CustomName);
    item6->setText(QString("%1").arg(para.CustomNote));

    item1->setTextAlignment(Qt::AlignCenter);
    item2->setTextAlignment(Qt::AlignCenter);
    item3->setTextAlignment(Qt::AlignCenter);
    item4->setTextAlignment(Qt::AlignCenter);
    item5->setTextAlignment(Qt::AlignCenter);
    item6->setTextAlignment(Qt::AlignCenter);
}




void DialogPrintOutTable::on_pushButton_cancle_clicked()
{
    done(-1);
}
