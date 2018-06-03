#ifndef DIALOGORDERDETAIL_H
#define DIALOGORDERDETAIL_H

#include <QDialog>
#include "global.h"

#pragma execution_character_set("utf-8")


namespace Ui {
class DialogOrderDetail;
}

class DialogOrderDetail : public QDialog
{
    Q_OBJECT

public:
    explicit DialogOrderDetail(QWidget *parent = 0);
    ~DialogOrderDetail();
    void init(Order order);

private slots:
    void on_pushButton_ok_clicked();
    void removeAllRow();

    void on_pushButton_out_table_clicked();

private:

    Ui::DialogOrderDetail *ui;
};

#endif // DIALOGORDERDETAIL_H
