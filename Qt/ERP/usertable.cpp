#include "usertable.h"
#include <QDebug>
#include "datacenter.h"

userTable::userTable(QWidget *w):M_TableWidget(w)
{
    this->setColumnCount(11);

    //设置表头内容
    QStringList header;
    header<<tr("用户编号")<<tr("姓名")<<tr("性别")\
         <<tr("联系方式")<<tr("部门")<<tr("账号")\
        <<tr("密码")<<tr("入职时间")<<tr("离职时间")\
       <<tr("基本工资")<<tr("状态");
    this->setHorizontalHeaderLabels(header);


    connect(this,SIGNAL(cellPressed(int,int)),this,SLOT(clickRow(int,int)));

    m_menu = new QMenu();

    m_new      = m_menu->addAction("新增");
    m_mod      = m_menu->addAction("修改");
    m_out      = m_menu->addAction("离职");
    m_del      = m_menu->addAction("删除");
    m_menu->addAction("放弃");

    connect(m_new,SIGNAL(triggered(bool)),this,SIGNAL(newUser()));
    connect(m_mod,SIGNAL(triggered(bool)),this,SIGNAL(modUser()));
    connect(m_out,SIGNAL(triggered(bool)),this,SIGNAL(outUser()));
    connect(m_del,SIGNAL(triggered(bool)),this,SIGNAL(delUser()));

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

void userTable::removeUser(QString para)
{
    int count = this->rowCount();
    for(int i=0;i<count;++i){
        QTableWidgetItem *item0 =  this->item(i,0);
        if(item0!=NULL&&item0->text()==para){
            this->removeRow(i);
            break;
        }
    }
}


void userTable::mousePressEvent(QMouseEvent *e)
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
                    if(item==NULL)
                        return;
                    bool exist =false;
                    User user = dataCenter::instance()->pub_getUser(item->text(),exist);
                    if(!exist){
                        return;
                    }
                    if(user.Status=="-1"){
                        m_new->setEnabled(false);
                        m_mod->setEnabled(false);
                        m_out->setEnabled(false);
                        m_del->setEnabled(false);
                    }
                    if(user.Status=="0"){
                        m_new->setEnabled(true);
                        m_mod->setEnabled(true);
                        m_out->setEnabled(true);
                        m_del->setEnabled(true);
                    }
                    if(user.Status=="1"){
                        m_new->setEnabled(true);
                        m_mod->setEnabled(false);
                        m_out->setEnabled(false);
                        m_del->setEnabled(true);
                    }
                    m_menu->exec(e->globalPos());
                }
            }
        }
    }

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
    QTableWidgetItem *item10 = this->item(row,10);
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
    item0->setText(para.UID);
    item1->setText(para.Name);
    item2->setText(para.Sex);
    item3->setText(para.Cell);
    item4->setText(para.Department);
    item5->setText(para.Account);
    item6->setText(para.Code);
    item7->setText(para.InTime);
    item8->setText(para.OutTime);
    item9->setText(QString("%1").arg(para.Salary));
    QString status;
    if(para.Status=="0"){
        status = "在职";
    }else{
        status = "已离职";
    }
    item10->setText(status);
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
    item10->setTextAlignment(Qt::AlignCenter);
}


