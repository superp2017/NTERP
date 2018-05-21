#include "usertable.h"

userTable::userTable(QWidget *w):QTableWidget(w)
{
    this->setColumnCount(10);
    this->setEditTriggers(QTableWidget::NoEditTriggers);
    this->setSelectionBehavior ( QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    this->setSelectionMode ( QAbstractItemView::SingleSelection); //设置选择模式，选择单行
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    this->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); //设置表头背景色
    QFont font = this->horizontalHeader()->font();
    font.setBold(true);
    this->horizontalHeader()->setFont(font);

    //设置表头内容
    QStringList header;
    header<<tr("用户编号")<<tr("姓名")<<tr("性别")<<tr("联系方式")<<tr("部门")<<tr("账号")<<tr("密码")<<tr("入职时间")<<tr("离职时间")<<tr("基本工资");
    this->setHorizontalHeaderLabels(header);

    this->setSortingEnabled(true);//允许列排序

    connect(this,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(doubleclickRow(int,int)));
    connect(this,SIGNAL(cellClicked(int,int)),this,SLOT(clickRow(int,int)));
}

void userTable::initUser(QVector<User> list)
{
    removeAllRow();
    for(User o:list){
        appendUser(o);
    }
}

void userTable::updateUser(QVector<User> list)
{
    this->setRowCount(list.size());
    for(int i=0;i<list.size();++i){
        setRowData(list.at(i),i);
    }
}

void userTable::appendUser(User para)
{
    this->setRowCount(this->rowCount()+1);
    int row=this->rowCount()-1;
    setRowData(para,row);
}

void userTable::modUser(User para)
{
    int count = this->rowCount();
    for(int i=0;i<count;++i){
        QTableWidgetItem *item0 =  this->item(i,0);
        if(item0!=NULL&&item0->text()==para.UID){
            setRowData(para,i);
            break;
        }
    }
}

void userTable::removeUser(User para)
{
    int count = this->rowCount();
    for(int i=0;i<count;++i){
        QTableWidgetItem *item0 =  this->item(i,0);
        if(item0!=NULL&&item0->text()==para.Name){
            this->removeRow(i);
            break;
        }
    }
}

void userTable::setHeaderColModel(QHeaderView::ResizeMode mode)
{
    this->horizontalHeader()->setSectionResizeMode(mode);
}

void userTable::doubleclickRow(int row, int ool)
{

}

void userTable::clickRow(int row, int col)
{
    if(row<0){
        return;
    }
    col =0;
    QTableWidgetItem* item = this->item(row,0);
    if (item!=NULL){
        emit userClick(item->text());
    }
}

void userTable::removeAllRow()
{
    int count = this->rowCount();
    for(int i=0;i<count;++i){
        this->removeRow(0);
    }
}

void userTable::setRowData(User para, int row)
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

    item0->setText(para.UID);
    item1->setText(para.Name);

    if(para.Sex=="0")
        item2->setText("男");
    else
        item2->setText("女");
    item3->setText(para.Cell);
    item4->setText(para.Department);
    item5->setText(para.Account);
    item6->setText(para.Code);
    item7->setText(para.InTime);
    item8->setText(para.OutTime);
    item9->setText(QString("%1").arg(para.Salary));


    item0->setTextAlignment(Qt::AlignCenter);
    item1->setTextAlignment(Qt::AlignCenter);
    item2->setTextAlignment(Qt::AlignCenter);
    item3->setTextAlignment(Qt::AlignCenter);
    item4->setTextAlignment(Qt::AlignCenter);
    item5->setTextAlignment(Qt::AlignCenter);
    item6->setTextAlignment(Qt::AlignCenter);
    item7->setTextAlignment(Qt::AlignCenter);
    item8->setTextAlignment(Qt::AlignCenter);
    item9->setTextAlignment(Qt::AlignCenter);
}
