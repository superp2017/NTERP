#ifndef DIALOGNEWORDER_H
#define DIALOGNEWORDER_H

#include <QDialog>
#include "global.h"
#include <QJsonObject>

#pragma execution_character_set("utf-8")

namespace Ui {
class DialogNewOrder;
}


class DialogNewOrder : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewOrder( QWidget *parent = 0);
    ~DialogNewOrder();
    void  initOrder(Order order);
    void  setModel(bool isNew);
    void  clearUI();
    Order getCurorder() const;
    void  initData();
private slots:
    void on_pushButton_ok_clicked();
    void on_pushButton_cancel_clicked();
    void customChange(int index);
    void unitChange(int index);
    void newOrderCb(Order order, bool ok);
    void modOrderCb(Order order,bool ok);
    void on_pushButton_edit_des_clicked();

private:
    void initCombox(QVector<Customer> custom, QSet<QString> batch, QVector<QString> unit);
    bool checkOrder(Order curorder);

    void changeModel();

    Ui::DialogNewOrder *ui;
    Order  curorder;
    bool   m_isNewMode;
};

#endif // DIALOGNEWORDER_H
