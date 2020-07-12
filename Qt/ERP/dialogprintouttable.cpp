#include "dialogprintouttable.h"
#include "ui_dialogprintouttable.h"
#include "datacenter.h"
#include <QMessageBox>
#include "boost/thread.hpp"
#include <QToolTip>
#include <QIntValidator>
#include <QCompleter>
DialogPrintOutTable::DialogPrintOutTable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPrintOutTable),
    m_printer(false,"","南通神州金属涂覆有限公司")
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

    connect(dataCenter::instance(),SIGNAL(sig_searchOutOrder(QVector<Order>,bool)),this,SLOT(orderSearchCb(QVector<Order>,bool)));

    ui->dateEdit_start->setDate(QDate::currentDate().addDays(-30));
    ui->dateEdit_end->setDate(QDate::currentDate());

    QStringList cuslist;
    for(Customer cus: dataCenter::instance()->pub_Customers()){
        ui->comboBox_company->addItem(cus.Name);
        cuslist<<cus.Name;
    }
    ui->comboBox_company->setEditable(true);
    QCompleter *completermater = new QCompleter(cuslist, this);
    completermater->setCaseSensitivity(Qt::CaseInsensitive);
    completermater->setFilterMode(Qt::MatchContains);
    ui->comboBox_company->setCompleter(completermater);

    QIntValidator* IntValidator = new QIntValidator();
    IntValidator->setRange(1, 999999);
    ui->lineEdit_curpage->setValidator(IntValidator);
    m_CurNum = 0;
    m_MaxSearch = 0;
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
        if(box&&box->isChecked()){
            data.push_back(m_data[box]);
        }
    }
    return data;
}

void DialogPrintOutTable::clearSelect()
{
    for(QCheckBox*box:m_boxs){
        if(box)
            box->setChecked(false);
    }
    ui->tableWidget->clearSelection();
    ui->tableWidget->checkSelect();
}

void DialogPrintOutTable::do_search(bool isCom, bool isTime, qint64 start, qint64 end, QString comName)
{
    ui->pushButton_search->setEnabled(false);
    QJsonObject para;
    para.insert("CusName",comName);
    para.insert("StartStamp",start);
    para.insert("EndStamp",end);
    para.insert("IsCus",isCom);
    para.insert("IsTime",isTime);
    boost::thread (boost::bind(&dataCenter::net_searchOutOrder,dataCenter::instance(),para)).detach();
    dataCenter::instance()->pub_showLoadding("正在网络请求...",5000,Qt::black);
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
        if(b&&box&&b!=box&&box->isChecked()){
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
                if(box&&box->text()==o.OrderID){
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



void DialogPrintOutTable::on_pushButton_search_clicked()
{
    qint64 start = 0;
    qint64 end   = 0;

    if(!ui->groupBox_company->isChecked()&&!ui->groupBox_time->isChecked()){
        QMessageBox::warning(this,"提示","请至少选择一种搜索方式!");
        return ;
    }

    if(ui->groupBox_company->isChecked()&&ui->comboBox_company->currentText()==""){
        QToolTip::showText(ui->comboBox_company->mapToGlobal(QPoint(100, 0)), "请至少选择一个公司!");
        return;
    }

    if(ui->groupBox_time->isChecked()){
        start = ui->dateEdit_start->dateTime().toSecsSinceEpoch();
        end   = ui->dateEdit_end->dateTime().toSecsSinceEpoch();
        if(end<start){
            QToolTip::showText(ui->dateEdit_end->mapToGlobal(QPoint(100, 0)), "结束时间不能小于开始时间!");
            return;
        }
    }

    ui->tableWidget->removeAllRow();
    do_search(ui->groupBox_company->isChecked(),\
              ui->groupBox_time->isChecked(), start,end,\
              ui->comboBox_company->currentText());
}

void DialogPrintOutTable::orderSearchCb(QVector<Order> list, bool ok)
{
    ui->pushButton_search->setEnabled(true);
    dataCenter::instance()->pub_hideLoadding();
    qDebug()<<"orderSearchCb"<<ok<<list.size();
    m_searchOrder = list;
    m_MaxSearch = m_searchOrder.size()/50+1;
    m_CurNum    = 1;
    ui->label_curPage->setText(QString("%1").arg(0));
    ui->label_numPage->setText(QString("%1").arg(0));
    initOrder();
    if(!ok){
        dataCenter::instance()->pub_showMessage("搜索失败!",2000);
    }

}

void DialogPrintOutTable::initOrder()
{
    if(m_CurNum<=0||m_CurNum>m_MaxSearch||m_searchOrder.size()==0){
        return;
    }
    ui->tableWidget->removeAllRow();

    m_boxs.clear();
    m_data.clear();

    for(int i = m_CurNum*50-1;i>=(m_CurNum-1)*50;--i){
        if(i<=m_searchOrder.size()-1)
            appendOrder(m_searchOrder.at(i));
    }

    ui->label_curPage->setText(QString("%1").arg(m_CurNum));
    ui->label_numPage->setText(QString("%1").arg(m_MaxSearch));

}

void DialogPrintOutTable::on_pushButton_go_clicked()
{
    int page = ui->lineEdit_curpage->text().trimmed().toInt();
    if(ui->lineEdit_curpage->text().isEmpty()||page<=0||page>m_MaxSearch){
        QToolTip::showText(ui->lineEdit_curpage->mapToGlobal(QPoint(100, 0)), "请输入有效页码!");
        return;
    }
    m_CurNum = page;
    ui->pushButton_go->setEnabled(false);
    initOrder();
    ui->pushButton_go->setEnabled(true);
}

void DialogPrintOutTable::on_pushButton_up_page_clicked()
{
    if(m_CurNum<=1){
        QToolTip::showText(ui->pushButton_up_page->mapToGlobal(QPoint(100, 0)), "已经第一页!");
        return;
    }
    ui->pushButton_up_page->setEnabled(false);
    --m_CurNum;
    initOrder();
    ui->pushButton_up_page->setEnabled(true);
}

void DialogPrintOutTable::on_pushButton_down_page_clicked()
{
    if(m_CurNum>=m_MaxSearch){
        QToolTip::showText(ui->pushButton_up_page->mapToGlobal(QPoint(100, 0)), "已经最后一页!");
        return;
    }
    ui->pushButton_down_page->setEnabled(false);
    ++m_CurNum;
    initOrder();
    ui->pushButton_down_page->setEnabled(true);
}
