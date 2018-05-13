#ifndef DIALOGNEWORDER_H
#define DIALOGNEWORDER_H

#include <QDialog>
#include "global.h"

namespace Ui {
class DialogNewOrder;
}


class DialogNewOrder : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewOrder(QWidget *parent = 0);
    ~DialogNewOrder();
    Order* getOrder();
    void test();

public slots:
    void newOrderCb(bool ok);


private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton_cancel_clicked();

private:
    void initCombox(QVector<Customer*> custom,QVector<QString> batch,\
                    QVector<Materiel*> materID,QVector<QString> unit);
    bool checkOrder();



    Ui::DialogNewOrder *ui;
    Order * curorder;
};

#endif // DIALOGNEWORDER_H
