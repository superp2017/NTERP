#ifndef ORDERMANAGER_H
#define ORDERMANAGER_H

#include <QWidget>
#include <QHeaderView>
#include "ordertable.h"
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

private slots:
    void on_pushButton_new_clicked();

    void changeCol();

private:
    Ui::OrderManager *ui;
    QHeaderView::ResizeMode tab_mode;
    OrderTable *m_tab_new;
    OrderTable *m_tab_success;
    OrderTable *m_tab_all;

    int id;
};

#endif // ORDERMANAGER_H
