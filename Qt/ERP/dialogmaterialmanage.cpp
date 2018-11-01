#include "dialogmaterialmanage.h"
#include "ui_dialogmaterialmanage.h"
#include "datacenter.h"
#include <QMessageBox>
#include "boost/thread.hpp"
#include <QFileDialog>
#include "materialservice.h"
#include "dialognewmateriel.h"
#include "dialognewmateriel.h"

DialogMaterialManage::DialogMaterialManage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMaterialManage)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(6);

    //设置表头内容
    QStringList header;
    header<<tr("物料编号")<<tr("物料描述")<<tr("客户名称")<<tr("未税单价")<<tr("单位")<<tr("创建时间");
    ui->tableWidget->setHorizontalHeaderLabels(header);

    connect(this,SIGNAL(sig_exportCb(bool)),this,SLOT(exportCb(bool)));
    connect(dataCenter::instance(),SIGNAL(sig_delMaterial(QString,bool)),this,SLOT(delMaterCb(QString,bool)));

    connect(ui->checkBox_check_all,SIGNAL(clicked(bool)),this,SLOT(checkAll()));

    connect(ui->tableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(cellDoubleClicked(int,int)));

    connect(ui->radioButton_ave,SIGNAL(clicked(bool)),this,SLOT(changeCol()));
    connect(ui->radioButton_content,SIGNAL(clicked(bool)),this,SLOT(changeCol()));
    connect(ui->radioButton_manu,SIGNAL(clicked(bool)),this,SLOT(changeCol()));
    ui->pushButton_mod->setEnabled(false);
    ui->pushButton_del->setEnabled(false);

}

DialogMaterialManage::~DialogMaterialManage()
{
    delete ui;
}

void DialogMaterialManage::closeAllStatus()
{
    ui->tableWidget->checkSelect();
}

void DialogMaterialManage::on_pushButton_exit_clicked()
{
    done(0);
}


void DialogMaterialManage::initData()
{
    ui->tableWidget->removeAllRow();
    changeCol();
    QVector<Materiel>ls = dataCenter::instance()->pub_Materiels();
    for(Materiel m:ls){
        appendOne(m);
    }
    ui->tableWidget->checkSelect();
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

void DialogMaterialManage::cellDoubleClicked(int row, int col)
{
    ui->tableWidget->checkSelect();
    QWidget *w = ui->tableWidget->cellWidget(row,0);
    if(w!=NULL){
        QCheckBox *box = reinterpret_cast<QCheckBox *>(w);
        bool ok =false;
        Materiel ma=  dataCenter::instance()->pub_getMateriel(box->text(),ok);
        if(ok){
            DialogNewMateriel dialog;
            dialog.setModel(2);
            dialog.initMater(ma);
            dialog.exec();
        }
    }
}

void DialogMaterialManage::changeCol()
{
    if(ui->radioButton_ave->isChecked()){
        ui->tableWidget->setHeaderColModel(QHeaderView::Stretch);
    }
    if(ui->radioButton_content->isChecked()){
        ui->tableWidget->setHeaderColModel(QHeaderView::ResizeToContents);

    }
    if(ui->radioButton_manu->isChecked()){
        ui->tableWidget->setHeaderColModel(QHeaderView::Interactive);
    }
    closeAllStatus();
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
    int checkSize = 0;
    for(QCheckBox* ch:m_boxs){
        check    &= ch->isChecked();
        if(ch->isChecked()){
            checkSize++;
        }
    }
    if(check){
        ui->checkBox_check_all->setCheckState(Qt::Checked);
    }else{
        ui->checkBox_check_all->setCheckState(Qt::Unchecked);
    }
    ui->pushButton_del->setEnabled(checkSize==1);
    ui->pushButton_mod->setEnabled(checkSize==1);
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

    item1->setText(ma.MaterDes);
    item2->setText(ma.CustomName);
    item3->setText(QString("%1").arg(ma.Money));
    item4->setText(ma.Unit);
    item5->setText(ma.CreatTime);

    item1->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    item2->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    item3->setTextAlignment(Qt::AlignCenter);
    item4->setTextAlignment(Qt::AlignCenter);
    item5->setTextAlignment(Qt::AlignCenter);
}

void DialogMaterialManage::removeOne(QString ma)
{
    int count = ui->tableWidget->rowCount();
    for(int i=0;i<count;++i){
        QWidget *w = ui->tableWidget->cellWidget(i,0);
        if(w!=NULL){
            QCheckBox *box = reinterpret_cast<QCheckBox *>(w);
            if(box->text()==ma){

                ui->tableWidget->removeRow(i);
                m_boxs.remove(i);
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




void DialogMaterialManage::on_pushButton_new_clicked()
{
    DialogNewMateriel dialog;
    dialog.setModel(0);
    if(dialog.exec()==123){
        Materiel ma = dialog.getMater();
        appendOne(ma);
    }
}

void DialogMaterialManage::on_pushButton_mod_clicked()
{
    for(int i=0;i<m_boxs.size();++i){
        QCheckBox *box = m_boxs.at(i);
        if(box!=NULL&&box->isChecked()){
            bool ok = false;
            Materiel ma = dataCenter::instance()->pub_getMateriel(box->text(),ok);
            if(ok){
                DialogNewMateriel dialog;
                dialog.setModel(1);
                dialog.initMater(ma);
                if(dialog.exec()==123){
                    setRowData(dialog.getMater(),i);
                }
            }
            break;
        }
    }
}

void DialogMaterialManage::on_pushButton_del_clicked()
{
    for(int i=0;i<m_boxs.size();++i){
        QCheckBox *box = m_boxs.at(i);
        if(box!=NULL&&box->isChecked()){
            QString MID = box->text();
            bool ok = false;
            Materiel ma = dataCenter::instance()->pub_getMateriel(MID,ok);
            if(ok){
                QMessageBox msgBox;
                msgBox.setWindowTitle("警告");
                msgBox.setText("您即将删除("+ma.MaterID+")"+ma.MaterDes);
                msgBox.setInformativeText("是否继续操作?");
                msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
                msgBox.setDefaultButton(QMessageBox::Ok);
                int ret = msgBox.exec();
                switch (ret) {
                case QMessageBox::Ok:
                {
                    QJsonObject obj;
                    obj.insert("CID",ma.CID);
                    obj.insert("MaterID",MID);
                    boost::thread(boost::bind(&dataCenter::net_delMaterial,dataCenter::instance(),obj));
                    dataCenter::instance()->pub_showLoadding("正在网络请求...",5000,Qt::black);
                    break;
                }
                case QMessageBox::Cancel:
                    break;
                default:
                    // should never be reached
                    break;
                }
            }
            break;
        }
    }
}

void DialogMaterialManage::delMaterCb(QString MID, bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        dataCenter::instance()->pub_showMessage("删除成功!",4000);
        removeOne(MID);
    }else{
        dataCenter::instance()->pub_showMessage("删除失败!",4000);
    }
}
