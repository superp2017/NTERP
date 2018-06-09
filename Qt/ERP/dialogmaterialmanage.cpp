#include "dialogmaterialmanage.h"
#include "ui_dialogmaterialmanage.h"
#include "datacenter.h"
#include <QMessageBox>
#include "boost/thread.hpp"
#include <QFileDialog>
#include "materialservice.h"
DialogMaterialManage::DialogMaterialManage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMaterialManage)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(7);

    //设置表头内容
    QStringList header;
    header<<tr("物料编号")<<tr("物料描述")<<tr("客户名称")<<tr("数量")<<tr("单位")<<tr("入库时间")<<tr("状态");
    ui->tableWidget->setHorizontalHeaderLabels(header);


    connect(this,SIGNAL(sig_exportCb(bool)),this,SLOT(exportCb(bool)));
    connect(ui->checkBox_check_all,SIGNAL(clicked(bool)),this,SLOT(checkAll()));

    connect(ui->tableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(onCellClick(int,int)));

}

DialogMaterialManage::~DialogMaterialManage()
{
    delete ui;
}

void DialogMaterialManage::closeAllStatus()
{
    ui->tableWidget->clearSelection();
}

void DialogMaterialManage::on_pushButton_exit_clicked()
{
    done(0);
}


void DialogMaterialManage::initData()
{
    ui->tableWidget->removeAllRow();
    closeAllStatus();
    QVector<Materiel>ls = dataCenter::instance()->pub_Materiels();
    for(Materiel m:ls){
        appendOne(m);
    }
}

void DialogMaterialManage::doExport(QVector<Materiel>ls,QString filepath)
{
    bool ok = MaterialService::exportMateriel(ls,filepath,true);
    emit sig_exportCb(ok);
}

void DialogMaterialManage::exportCb(bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        dataCenter::instance()->pub_showMessage("导出成功!",3000);
    }else{
        dataCenter::instance()->pub_showMessage("导出失败!",3000);
    }
}

void DialogMaterialManage::checkAll()
{
    for(QCheckBox* box:m_boxs){
        box->setChecked(ui->checkBox_check_all->isChecked());
    }
}

void DialogMaterialManage::onCellClick(int row, int col)
{
    this->childrenRect();
}



void DialogMaterialManage::on_pushButton_export_clicked()
{

    QVector<Materiel> ls;
    int count = ui->tableWidget->rowCount();
    for(int i=0;i<count;++i){
        ui->tableWidget->cellWidget(i,0);
        QWidget *w = ui->tableWidget->cellWidget(i,0);
        if(w!=NULL){
            QCheckBox *box = reinterpret_cast<QCheckBox *>(w);
            if(box->isChecked()){
                bool ok=false;
                Materiel m=  dataCenter::instance()->pub_getMateriel(box->text(),ok);
                if(ok){
                    ls.push_back(m);
                }
            }
        }
    }

    if(ls.size()==0){
        QMessageBox::information(this,"提示","请至少选择一个物料...");
        return;
    }

    QString filepath= QFileDialog::getSaveFileName(NULL,"Save orders",".","Microsoft Office 2007 (*.xlsx)");//获取保存路径
    if(!filepath.isEmpty()){
        boost::thread t(boost::bind(&DialogMaterialManage::doExport,this,ls,filepath));
        t.detach();
        dataCenter::instance()->pub_showLoadding("正在导出...",10000);
    }else{
        dataCenter::instance()->pub_showMessage("保存路径为空!",3000);
    }
}

void DialogMaterialManage::checkBox()
{
    bool check = true;
    bool check_one = false;
    for(QCheckBox* ch:m_boxs){
        check    &= ch->isChecked();
        check_one|= ch->isChecked();
    }
    if(check){
        ui->checkBox_check_all->setCheckState(Qt::Checked);
    }else{
        if(check_one)
            ui->checkBox_check_all->setCheckState(Qt::PartiallyChecked);
        else{
            ui->checkBox_check_all->setCheckState(Qt::Unchecked);
        }
    }
}



void DialogMaterialManage::setRowData(Materiel ma, int row)
{    
    QCheckBox *box ;
    QWidget *w= ui->tableWidget->cellWidget(row,0);
    if(w ==NULL){
        box  = new QCheckBox(ma.CID);
        m_boxs.push_back(box);
        ui->tableWidget->setCellWidget(row,0,box);
        connect(box,SIGNAL(clicked(bool)),this,SLOT(checkBox()));
    }else{
        box = reinterpret_cast<QCheckBox *>(w);
    }
    if(box!=NULL){
        box->setText(ma.MaterID);
    }

    QTableWidgetItem *item1 = ui->tableWidget->item(row,1);
    QTableWidgetItem *item2 = ui->tableWidget->item(row,2);
    QTableWidgetItem *item3 = ui->tableWidget->item(row,3);
    QTableWidgetItem *item4 = ui->tableWidget->item(row,4);
    QTableWidgetItem *item5 = ui->tableWidget->item(row,5);
    QTableWidgetItem *item6 = ui->tableWidget->item(row,6);
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
    QString status;
    if(ma.Status=="0")status="已入库";
    if(ma.Status=="1")
        status="已出库";
    if(ma.Status=="-1")
        status = "已取消";
    item1->setText(ma.MaterDes);
    item2->setText(ma.CustomName);
    item3->setText(QString("%1").arg(ma.OrderNum));
    item4->setText(ma.Unit);
    item5->setText(ma.CreatTime);
    item6->setText(status);

    item1->setTextAlignment(Qt::AlignCenter);
    item2->setTextAlignment(Qt::AlignCenter);
    item3->setTextAlignment(Qt::AlignCenter);
    item4->setTextAlignment(Qt::AlignCenter);
    item5->setTextAlignment(Qt::AlignCenter);
    item6->setTextAlignment(Qt::AlignCenter);

}

void DialogMaterialManage::removeOne(Materiel ma)
{
    int count = ui->tableWidget->rowCount();
    for(int i=0;i<count;++i){
        QWidget *w = ui->tableWidget->cellWidget(i,0);
        if(w!=NULL){
            QCheckBox *box = reinterpret_cast<QCheckBox *>(w);
            if(box->text()==ma.MaterID){
                ui->tableWidget->removeRow(i);
                break;
            }
        }
    }
}

void DialogMaterialManage::appendOne(Materiel ma)
{
    ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
    int row= ui->tableWidget->rowCount()-1;
    setRowData(ma,row);
}



