#ifndef DIALOGMODPRICE_H
#define DIALOGMODPRICE_H

#include <QDialog>
#include "global.h"

namespace Ui {
class DialogModPrice;
}

class DialogModPrice : public QDialog
{
    Q_OBJECT

public:
    explicit DialogModPrice(QWidget *parent = 0);
    ~DialogModPrice();
    void initData(Order order);
    Order getCurOrder() const;

private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton__cancle_clicked();

    void modOrderPriceCb(Order order,bool ok);

private:
    Ui::DialogModPrice *ui;
    Order  curOrder;
};

#endif // DIALOGMODPRICE_H
