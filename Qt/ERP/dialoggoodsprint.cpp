#include "dialoggoodsprint.h"
#include "ui_dialoggoodsprint.h"
#include "datacenter.h"
#include <QMessageBox>
#include <QFileDialog>
#include "boost/thread.hpp"


DialogGoodsPrint::DialogGoodsPrint(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogGoodsPrint)
{
    ui->setupUi(this);


    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); //设置表头背景色
    QFont font = ui->tableWidget->horizontalHeader()->font();
    font.setBold(true);
    ui->tableWidget->horizontalHeader()->setFont(font);

    //设置表头内容
    QStringList header;
    header<<tr("商品编号")<<tr("商品名称");
    ui->tableWidget->setHorizontalHeaderLabels(header);

    connect(ui->comboBox__type,SIGNAL(currentIndexChanged(int)),this,SLOT(TypeChange()));


    m_checkboxs.clear();
    mData.clear();

    initComBox();
    ui->comboBox__type->setCurrentIndex(0);
    curType = ItemAllType;

}

DialogGoodsPrint::~DialogGoodsPrint()
{
    delete ui;
}

void DialogGoodsPrint::initComBox()
{
    ui->comboBox__type->blockSignals(true);
    ui->comboBox__type->clear();
    ui->comboBox__type->addItem(ItemAllType);
    QVector<QString> list = dataCenter::instance()->pub_goodsType();
    for(QString t:list){
        ui->comboBox__type->addItem(t);
    }
    ui->comboBox__type->blockSignals(false);
}


void DialogGoodsPrint::on_pushButton_cancle_clicked()
{
    done(-1);
}

void DialogGoodsPrint::doExport(QVector<Goods> ls, QString filepath)
{
    bool ok = GoodsService::exportGoods(ls,filepath,true);
    emit sig_exportCb(ok);
}

void DialogGoodsPrint::on_pushButton_export_clicked()
{
    QVector<Goods> ls;
    if(mData.size()!=m_checkboxs.size()){
        dataCenter::instance()->pub_showMessage("操作失败!",3000);
        return ;
    }

    for(int i =0;i<m_checkboxs.size();++i){
        if(m_checkboxs.at(i)->isChecked()){
            ls.push_back(mData.at(i));
        }
    }
    if(ls.size()==0){
        QMessageBox::information(this,"提示","请至少选择一个商品...");
        return;
    }
    QString filepath= QFileDialog::getSaveFileName(NULL,"Save orders",".","Microsoft Office 2007 (*.xlsx)");//获取保存路径
    if(!filepath.isEmpty()){
        boost::thread t(boost::bind(&DialogGoodsPrint::doExport,this,ls,filepath));
        t.detach();
        dataCenter::instance()->pub_showLoadding("正在导出...",10000);
    }else{
        dataCenter::instance()->pub_showMessage("保存路径为空!",3000);
    }
}

void DialogGoodsPrint::TypeChange()
{
    QString type = ui->comboBox__type->currentText();
    if(type==curType){
        return ;
    }
    curType = type;
     mData = dataCenter::instance()->pub_GetTypeGoods(curType);
    if(mData.size()>0){
        removeAllRow();
    }
    for(Goods g:mData){
        ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
        int row=ui->tableWidget->rowCount()-1;
        setRowData(g,row);
    }
}
void DialogGoodsPrint::setRowData(Goods goods, int row)
{
    QCheckBox *check1 = new QCheckBox(goods.ID);
    ui->tableWidget->setCellWidget(row,0,check1);
    ui->tableWidget->setItem(row,1,new QTableWidgetItem(goods.Name));
    connect(check1,SIGNAL(clicked(bool)),this,SLOT(checkBox()));
    m_checkboxs.push_back(check1);
}



void DialogGoodsPrint::removeAllRow()
{
    int count = ui->tableWidget->rowCount();
    for(int i=0;i<count;++i){
        ui->tableWidget->removeRow(0);
    }
}

void DialogGoodsPrint::checkBox()
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



void DialogGoodsPrint::on_checkBox_check_all_clicked()
{
    for(QCheckBox *box:m_checkboxs){
        if(box!=NULL){
            box->setChecked(ui->checkBox_check_all->isChecked());
        }
    }
}
