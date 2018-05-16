#ifndef ORDERMANAGER_H
#define ORDERMANAGER_H

#include <QWidget>
#include <QHeaderView>
#include "ordertable.h"
#pragma execution_character_set("utf-8")

#include "dialogneworder.h"
namespace Ui {
class OrderManager;
}

class OrderManager : public QWidget
{
    Q_OBJECT

public:
    explicit OrderManager(QWidget *parent = 0);
    ~OrderManager();

private slots:
    void on_pushButton_new_clicked();
    void changeCol();
    void orderClick(QString orderID);
    void on_pushButton_mod_clicked();
    void on_pushButton_cancle_clicked();
    void on_pushButton_success_clicked();
    void clearAllSelect();
private:
    void clearCurOrder();


private:
    Ui::OrderManager *ui;
    QHeaderView::ResizeMode tab_mode;
    OrderTable *m_tab_new;
    OrderTable *m_tab_success;
    OrderTable *m_tab_all;
    DialogNewOrder *neworer;
    Order cur_order;
};

#endif // ORDERMANAGER_H
