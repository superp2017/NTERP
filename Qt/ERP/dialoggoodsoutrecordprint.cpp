#include "dialoggoodsoutrecordprint.h"
#include "ui_dialoggoodsoutrecordprint.h"
#include <QMessageBox>
#include <QFileDialog>
#include "boost/thread.hpp"
#include "datacenter.h"

DialogGoodsOutRecordPrint::DialogGoodsOutRecordPrint(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogGoodsOutRecordPrint)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); //设置表头背景色

    ui->tableWidget->setSortingEnabled(true);//允许列排序

    QFont font = ui->tableWidget->horizontalHeader()->font();
    font.setBold(true);
    ui->tableWidget->horizontalHeader()->setFont(font);

    //设置表头内容
    QStringList header;
    header<<tr("凭证号");
    ui->tableWidget->setHorizontalHeaderLabels(header);

    connect(ui->tableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(cellChecked(int,int)));
    connect(ui->checkBox_check_all,SIGNAL(clicked(bool)),this,SLOT(checkAll()));

    connect(this,SIGNAL(sig_exportCb(bool)),this,SLOT(exportCb(bool)));

    m_checkboxs.clear();
    m_records.clear();

    initData(dataCenter::instance()->pub_AllOutRecord());
}

DialogGoodsOutRecordPrint::~DialogGoodsOutRecordPrint()
{
    delete ui;
}

void DialogGoodsOutRecordPrint::initData(QVector<GoodsOutRecord> list)
{
    removeAllRow();
    m_checkboxs.clear();
    m_records.clear();
    m_records =list;
    for(GoodsOutRecord o:list){
        ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
        int row=ui->tableWidget->rowCount()-1;
        setRowData(o,row);
    }
}

void DialogGoodsOutRecordPrint::on_pushButton_export_clicked()
{
    QVector<GoodsOutRecord> ls= getSelectGoodsRecord();
    if(ls.size()==0){
        QMessageBox::information(this,"提示","请至少选择一条记录!");
        return;
    }
    QString filepath= QFileDialog::getSaveFileName(NULL,"Save orders",".","Microsoft Office 2007 (*.xlsx)");//获取保存路径
    if(!filepath.isEmpty()){
        boost::thread t(boost::bind(&DialogGoodsOutRecordPrint::doExport,this,ls,filepath));
        t.detach();
        dataCenter::instance()->pub_showLoadding("正在导出...",20000);
    }else{
        dataCenter::instance()->pub_showMessage("保存路径为空!",3000);
    }
}

void DialogGoodsOutRecordPrint::on_pushButton_cancle_clicked()
{
    done(-1);
}

void DialogGoodsOutRecordPrint::checkAll()
{
    for(QCheckBox *check:m_checkboxs){
        check->setChecked(ui->checkBox_check_all->isChecked());
    }
}

void DialogGoodsOutRecordPrint::cellChecked(int row, int col)
{
    col =0;
    if(row<m_checkboxs.size()&&row>=0){
        m_checkboxs.at(row)->setChecked(!m_checkboxs.at(row)->isChecked());
    }
}

void DialogGoodsOutRecordPrint::removeAllRow()
{
    int count = ui->tableWidget->rowCount();
    for(int i=0;i<count;++i){
        ui->tableWidget->removeRow(0);
    }
}

void DialogGoodsOutRecordPrint::setRowData(GoodsOutRecord g, int row)
{
    QCheckBox *check1 = new QCheckBox(g.OutID);
    ui->tableWidget->setCellWidget(row,0,check1);
    connect(check1,SIGNAL(clicked(bool)),this,SLOT(checkBox()));
    m_checkboxs.push_back(check1);
}

void DialogGoodsOutRecordPrint::exportCb(bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        done(123);
        dataCenter::instance()->pub_showMessage("导出成功!",3000);
    }else{
        dataCenter::instance()->pub_showMessage("导出失败!",3000);
    }
}

void DialogGoodsOutRecordPrint::checkBox()
{
    bool check = true;
    bool check_one = false;
    for(QCheckBox* ch:m_checkboxs){
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

QVector<GoodsOutRecord> DialogGoodsOutRecordPrint::getSelectGoodsRecord()
{
    QVector<GoodsOutRecord> ls;
    if(m_records.size() != m_checkboxs.size()){
        dataCenter::instance()->pub_showMessage("操作失败!",3000);
        return ls;
    }

    for(int i =0;i<m_checkboxs.size();++i){
        if(m_checkboxs.at(i)->isChecked()){
            ls.push_back(m_records.at(i));
        }
    }
    return ls;
}

void DialogGoodsOutRecordPrint::doExport(QVector<GoodsOutRecord> ls, QString filepath)
{
    bool ok = GoodsOutRecordService::exportGoodsRecord(ls,filepath,true);
    emit sig_exportCb(ok);
}
