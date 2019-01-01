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

   // void initData(QString status, QString factory);

private slots:
    void on_pushButton_export_clicked();

    void on_pushButton_cancle_clicked();

//    void orderStatusChange(int index);

//    void factoryChange(int index);

    void selectAll(bool checked);

    void cellChecked(int row,int col);

    void exportCb(bool ok);

    void checkBox();
    void onSliderChanged(int v);



    void on_pushButton_query_clicked();

signals:
    void sig_exportCb(bool);

private:
    void doExport(QVector<Order> ls, QString filepath);
    void removeAllRow();
    void setRowData(Order order, int row);
   // void updateData(QString status,QString factory);
    QVector<Order> getSelectOrders();


private:
    Ui::DialogOrderPrint *ui;
    QVector<Order>    m_orders;
    QVector<QCheckBox*> m_checkboxs;
//    QString cur_Status;
//    QString cur_factory;
};

#endif // DIALOGORDERPRINT_H
