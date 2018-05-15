#ifndef ORDERTABLE_H
#define ORDERTABLE_H

#include <QTableWidget>
#include <QHeaderView>
#include "global.h"
#include "dialogorderdetail.h"

#pragma execution_character_set("utf-8")

class OrderTable : public QTableWidget
{
    Q_OBJECT
public:
    OrderTable(QTableWidget*w=0);
    void setHeaderColModel( QHeaderView::ResizeMode mode);
    void appendOrder(Order para);


signals:
    void orderClick(QString orderID);

private slots:
    void doubleclickRow(int row,int ool);
    void clickRow(int row,int col);
private:
    void removeAllRow();
    void setRowData(Order para, int row);

private:
    DialogOrderDetail *order_detail;
};

#endif // ORDERTABLE_H
