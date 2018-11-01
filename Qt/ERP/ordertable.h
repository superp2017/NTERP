#ifndef ORDERTABLE_H
#define ORDERTABLE_H

#include <QTableWidget>
#include <QHeaderView>
#include "global.h"
#include <QMenu>
#include <QMouseEvent>
#include "m_tablewidget.h"

#pragma execution_character_set("utf-8")

class OrderTable : public M_TableWidget
{
    Q_OBJECT

public:
    OrderTable(QString status,QWidget*w=0);
    void initOrder(QVector<Order>list);
    void updateOrder(QVector<Order>list);
    void appendOrder(Order para);
    void modOrder(Order para);
    void removeOrder(Order para);
    void showAllRow();
    int  getTimeColNum();
signals:
    void orderClick(QString orderID);
    void newOrder();
    void cancleOrder();
    void produceOrder();
    void modOrder();
    void outOrder();
    void modPrice();
    void delOrder();
private slots:
    void doubleclickRow(int row,int ool);
    void clickRow(int row,int col);
private:
    void setRowData(Order para, int row);
    void setEnable(bool produce, bool cancel, bool mod, bool out, bool modPrice, bool del);
protected:
    void mousePressEvent(QMouseEvent *e);
private:
    QMenu   *m_menu;
    QAction *m_new;
    QAction *m_mod;
    QAction *m_cancle;
    QAction *m_produce;
    QAction *m_out;
    QAction *m_mod_price;
    QAction *m_del;
    QAction *m_giveup;
    QString cutStatus;
    int timecol;
};

#endif // ORDERTABLE_H
