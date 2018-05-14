#ifndef DIALOGORDERDETAIL_H
#define DIALOGORDERDETAIL_H

#include <QDialog>
#include "global.h"

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

    void on_pushButton_cancel_clicked();



private:

    Ui::DialogOrderDetail *ui;
};

#endif // DIALOGORDERDETAIL_H
