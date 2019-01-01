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
     void checkAuthor(int author);
public slots:
    void new_order();
private slots:
    void on_pushButton_new_clicked();
    void on_pushButton_mod_clicked();
    void on_pushButton_cancle_clicked();
    void on_pushButton_success_clicked();
    void on_pushButton_reflash_clicked();
    void on_pushButton_print_clicked();
    void on_pushButton_produce_clicked();
    void on_pushButton_del_clicked();
    void on_pushButton_change_price_clicked();

    void changeCol();
    void orderClick(QString orderID);
    void clearAllSelect();
    void cancleOrderCb(Order order,bool ok);
    void produceOrderCb(Order order, bool ok);
    void finishOrderCb(Order order,bool ok);
    void delOrderCb(Order order, bool ok);
    void GlobalOrdersCb(bool ok,int type,bool hasNew);
    void showAll();
    void on_pushButton_out_print_clicked();
    void tabChange();

protected:
    void mousePressEvent(QMouseEvent *e);

private:
    void clearCurOrder();
    void setBtnEnable(bool mod, bool cancel, bool produce, bool out, bool change, bool del,bool isnew = true);
    void checkSelect();
private:
    Ui::OrderManager  *ui;
    OrderTable        *m_tab_new;
    OrderTable        *m_tab_success;
    OrderTable        *m_tab_produce;
    OrderTable        *m_tab_all;
    Order             cur_order;
};

#endif // ORDERMANAGER_H
