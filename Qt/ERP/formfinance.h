#ifndef FORMFINANCE_H
#define FORMFINANCE_H

#include <QWidget>
#include "service_global.h"

namespace Ui {
class FormFinance;
}

class FormFinance : public QWidget
{
    Q_OBJECT

public:
    explicit FormFinance(QWidget *parent = 0);
    ~FormFinance();

private slots:
    void on_pushButton_search_clicked();

    void on_pushButton_export_clicked();

    void do_search(QString cusName,QString status,QString type,\
                   QString fac, qint64 start,qint64 end,\
                   bool Iscus ,bool Isrtype ,\
                   bool IsStatus ,bool isFac,\
                   bool Istime);
     void initOrder(QVector<Order>list);
private:
    void initUI();
    void setRowData(Order para, int row);
    Ui::FormFinance *ui;
    QVector<Order>m_data;
};

#endif // FORMFINANCE_H
