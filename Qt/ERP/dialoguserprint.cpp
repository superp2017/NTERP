#include "dialoguserprint.h"
#include "ui_dialoguserprint.h"
#include <QMessageBox>
#include "datacenter.h"

DialogUserPrint::DialogUserPrint(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogUserPrint)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
    //    ui->tableWidget->setSelectionBehavior ( QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    //    ui->tableWidget->setSelectionMode ( QAbstractItemView::SingleSelection); //设置选择模式，选择单行
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); //设置表头背景色
    QFont font = ui->tableWidget->horizontalHeader()->font();
    font.setBold(true);
    ui->tableWidget->horizontalHeader()->setFont(font);

    //设置表头内容
    QStringList header;
    header<<tr("员工姓名");
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->setSortingEnabled(true);//允许列排序

    connect(ui->tableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(cellChecked(int,int)));
    connect(ui->checkBox_check_all,SIGNAL(clicked(bool)),this,SLOT(checkAll()));

    m_checkboxs.clear();
    m_employeess.clear();
}

DialogUserPrint::~DialogUserPrint()
{
    delete ui;
}

void DialogUserPrint::initData(QVector<User> list)
{
    removeAllRow();
    m_checkboxs.clear();
    m_employeess.clear();
    m_employeess =list;
    for(User o:list){
        ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
        int row=ui->tableWidget->rowCount()-1;
        setRowData(o,row);
    }
}

void DialogUserPrint::on_pushButton_cancle_clicked()
{
    done(-1);
}

void DialogUserPrint::on_pushButton_print_clicked()
{
    QVector<User> ls= getSelectUsers();
    if(ls.size()==0){
        QMessageBox::information(this,"提示","请至少选择一个员工!");
        return;
    }
    if(UserService::printUser(ls)){
        dataCenter::instance()->showMessage("打印成功!",3000);
    }else{
        dataCenter::instance()->showMessage("打印失败!",3000);
    }
    done(123);
}

void DialogUserPrint::on_pushButton_export_clicked()
{
    QVector<User> ls= getSelectUsers();
    if(ls.size()==0){
        QMessageBox::information(this,"提示","请至少选择一个员工!");
        return;
    }
    if(UserService::exportUser(ls)){
        dataCenter::instance()->showMessage("导出成功!",3000);
    }else{
        dataCenter::instance()->showMessage("导出失败!",3000);
    }
    done(123);
}


QVector<User> DialogUserPrint::getSelectUsers()
{
    QVector<User> ls;
    if(m_employeess.size()!=m_checkboxs.size()){
        dataCenter::instance()->showMessage("操作失败!",3000);
        return ls;
    }

    for(int i =0;i<m_checkboxs.size();++i){
        if(m_checkboxs.at(i)->isChecked()){
            ls.push_back(m_employeess.at(i));
        }
    }
    return ls;
}

void DialogUserPrint::setRowData(User user, int row)
{
    QCheckBox *check1 = new QCheckBox(user.Name);
    ui->tableWidget->setCellWidget(row,0,check1);
    m_checkboxs.push_back(check1);
}


void DialogUserPrint::checkAll()
{
    for(QCheckBox *check:m_checkboxs){
        check->setChecked(ui->checkBox_check_all->isChecked());
    }
}

void DialogUserPrint::cellChecked(int row, int col)
{
    col =0;
    if(row<m_checkboxs.size()&&row>=0){
        m_checkboxs.at(row)->setChecked(!m_checkboxs.at(row)->isChecked());
    }
}

void DialogUserPrint::removeAllRow()
{
    int count = ui->tableWidget->rowCount();
    for(int i=0;i<count;++i){
        ui->tableWidget->removeRow(0);
    }
}

