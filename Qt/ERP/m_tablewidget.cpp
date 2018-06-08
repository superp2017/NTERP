#include "m_tablewidget.h"

M_TableWidget::M_TableWidget(QWidget *w):QTableWidget(w)
{
    this->setEditTriggers(QTableWidget::NoEditTriggers);
    this->setSelectionBehavior ( QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    this->setSelectionMode ( QAbstractItemView::SingleSelection); //设置选择模式，选择单行
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    QFont font = this->horizontalHeader()->font();
    font.setBold(true);
    this->horizontalHeader()->setFont(font);
    this->setSortingEnabled(true);//允许列排序
    checkSelect();
}

void M_TableWidget::removeAllRow()
{
    int count = this->rowCount();
    for(int i=0;i<count;++i){
        this->removeRow(0);
    }
}

void M_TableWidget::setHeaderColModel(QHeaderView::ResizeMode mode)
{
    this->horizontalHeader()->setSectionResizeMode(mode);
}

void M_TableWidget::checkSelect()
{
    if(this->selectedRanges().size()>0){
        this->horizontalHeader()->setStyleSheet("QHeaderView::section{background:#324864;color:#202020}"); //设置表头背景色
    }else{
        this->horizontalHeader()->setStyleSheet("QHeaderView::section{background:#324864;color:#e5e5e5}"); //设置表头背景色
    }
}
