#ifndef ORDERTABLE_H
#define ORDERTABLE_H

#include <QTableWidget>
#include <QHeaderView>
#include "global.h"
#include "dialogorderdetail.h"
#include <QMenu>
#include <QMouseEvent>
#include "m_tablewidget.h"

#pragma execution_character_set("utf-8")

class OrderTable : public M_TableWidget
{
    Q_OBJECT
public:
    OrderTable(QWidget*w=0);

    void initOrder(QVector<Order>list);
    void updateOrder(QVector<Order>list);
    void appendOrder(Order para);
    void modOrder(Order para);
    void removeOrder(Order para);
signals:
    void orderClick(QString orderID);
    void newOrder();
    void cancleOrder();
    void produceOrder();
    void modOrder();
    void outOrder();
    void modPrice();
private slots:
    void doubleclickRow(int row,int ool);
    void clickRow(int row,int col);
private:
    void setRowData(Order para, int row);

protected:
    void mousePressEvent(QMouseEvent *e);
private:
    DialogOrderDetail *order_detail;
    QMenu   *m_menu;
    QAction *m_new;
    QAction *m_mod;
    QAction *m_produce;
    QAction *m_cancle;
    QAction *m_out;
    QAction *m_mod_price;
};

#endif // ORDERTABLE_H
