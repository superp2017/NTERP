#ifndef DIALOGORDERPRINT_H
#define DIALOGORDERPRINT_H

#include <QDialog>
#include <QCheckBox>
#include "orderservice.h"

#pragma execution_character_set("utf-8")


namespace Ui {
class DialogOrderPrint;
}

class DialogOrderPrint : public QDialog
{
    Q_OBJECT

public:
    explicit DialogOrderPrint(QWidget *parent = 0);
    ~DialogOrderPrint();

    void initData(QString status);

private slots:
    void on_pushButton_export_clicked();

    void on_pushButton_print_clicked();

    void on_pushButton_cancle_clicked();

    void orderStatusChange(int index);

    void selectAll(bool checked);

    void cellChecked(int row,int col);


private:
    void removeAllRow();
    void setRowData(Order order, int row);
    void updateData(QString status);
    QVector<Order> getSelectOrders();


private:
    Ui::DialogOrderPrint *ui;
    QVector<Order>    m_orders;
    QVector<QCheckBox*> m_checkboxs;
    QString cur_Status;
};

#endif // DIALOGORDERPRINT_H
