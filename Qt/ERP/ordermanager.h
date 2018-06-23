#ifndef ORDERMANAGER_H
#define ORDERMANAGER_H

#include <QWidget>
#include <QHeaderView>
#include "ordertable.h"
#include <QMouseEvent>
#include "dialogordersearch.h"

#include "dialogneworder.h"
#pragma execution_character_set("utf-8")

namespace Ui {
class OrderManager;
}

class OrderManager : public QWidget
{
    Q_OBJECT

public:
    explicit OrderManager(QWidget *parent = 0);
    ~OrderManager();
    void updataData();

public slots:
    void new_order();
private slots:
    void on_pushButton_new_clicked();
    void on_pushButton_mod_clicked();
    void on_pushButton_cancle_clicked();
    void on_pushButton_success_clicked();
    void on_pushButton_reflash_clicked();
    void on_pushButton_change_price_clicked();
    void on_pushButton_print_clicked();

    void changeCol();
    void orderClick(QString orderID);
    void clearAllSelect();
    void cancleOrderCb(Order order,bool ok);
    void produceOrderCb(Order order, bool ok);
    void finishOrderCb(Order order,bool ok);
    void GlobalOrdersCb(bool ok);
    void on_pushButton_search_clicked();

    void searchTime(qint64 min,qint64 max);
    void searchOther(QString type,QString content);
    void showAll();
    void on_pushButton_produce_clicked();

protected:
    void mousePressEvent(QMouseEvent *e);

private:
    void initSearch();
    void clearCurOrder();
    void setBtnEnable(bool mod, bool cancel, bool produce, bool out, bool change);
    void checkSelect();
private:
    Ui::OrderManager  *ui;
    OrderTable        *m_tab_new;
    OrderTable        *m_tab_success;
    OrderTable        *m_tab_produce;
    OrderTable        *m_tab_all;
    DialogNewOrder    *neworer;
    Order             cur_order;
    DialogOrderSearch m_search;
    QMap<QString, QVector<QString> > searchdata;
};

#endif // ORDERMANAGER_H
