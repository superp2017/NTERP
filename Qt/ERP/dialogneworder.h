#ifndef DIALOGNEWORDER_H
#define DIALOGNEWORDER_H

#include <QDialog>
#include "global.h"
#include <QJsonObject>
namespace Ui {
class DialogNewOrder;
}


class DialogNewOrder : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewOrder(QWidget *parent = 0);
    ~DialogNewOrder();
    QJsonObject getPara() const;
    Order getCurorder() const;

public slots:
    void newOrderCb(Order order, bool ok);


private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton_cancel_clicked();

private:
    void initCombox(QVector<Customer> custom,QVector<QString> batch,\
                    QVector<Materiel> materID,QVector<QString> unit);
    bool checkOrder(Order curorder);

    Ui::DialogNewOrder *ui;
    QJsonObject para;
    Order  curorder;
};

#endif // DIALOGNEWORDER_H
