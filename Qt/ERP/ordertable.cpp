#include "ordertable.h"

OrderTable::OrderTable(QTableWidget *w):
    QTableWidget(w)
{
      this->setRowCount(50);
    this->setColumnCount(9);
    this->setEditTriggers(QTableWidget::NoEditTriggers);
    this->setSelectionBehavior ( QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    this->setSelectionMode ( QAbstractItemView::SingleSelection); //设置选择模式，选择单行
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->horizontalHeader()->setFixedHeight(25); //设置表头的高度
    this->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); //设置表头背景色
    QFont font = this->horizontalHeader()->font();
    font.setBold(true);
    this->horizontalHeader()->setFont(font);

    //设置表头内容
     QStringList header;
     header<<tr("生产订单")<<tr("物料编码")<<tr("物料描述")\
          <<tr("订单数量")<<tr("单位")<<tr("客户批次")<<tr("客户备注")<<tr("生产批号")<<tr("价格");
     this->setHorizontalHeaderLabels(header);

     this->setSortingEnabled(true);

}

void OrderTable::setHeaderColModel(QHeaderView::ResizeMode mode)
{
    this->horizontalHeader()->setSectionResizeMode(mode);
}
