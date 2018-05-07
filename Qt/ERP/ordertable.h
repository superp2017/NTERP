#ifndef ORDERTABLE_H
#define ORDERTABLE_H

#include <QTableWidget>
#include <QHeaderView>
#include "global"
#include "dialogorderdetail.h"

#pragma execution_character_set("utf-8")

class OrderTable : public QTableWidget
{
    Q_OBJECT
public:
    OrderTable(QTableWidget*w=0);
    void setHeaderColModel( QHeaderView::ResizeMode mode);
    void appendOrder(Order *para);


private slots:
    void clickRow(int row,int ool);

private:
    void removeAllRow();
    void setRowData(Order *para, int row);



private:
    DialogOrderDetail *order_detail;
    Order* cur_order;
};

#endif // ORDERTABLE_H
