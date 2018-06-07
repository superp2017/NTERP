#ifndef DIALOGGOODSPRINT_H
#define DIALOGGOODSPRINT_H

#include <QDialog>
#include "goodsService.h"
namespace Ui {
class DialogGoodsPrint;
}

class DialogGoodsPrint : public QDialog
{
    Q_OBJECT

public:
    explicit DialogGoodsPrint(QWidget *parent = 0);
    ~DialogGoodsPrint();
    void initData(QVector<Goods> data);

private:
    Ui::DialogGoodsPrint *ui;
};

#endif // DIALOGGOODSPRINT_H
