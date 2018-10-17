#ifndef DIALOGGOODSIN_H
#define DIALOGGOODSIN_H
#include "goodsService.h"
#include <QDialog>

namespace Ui {
class DialogGoodsIn;
}

class DialogGoodsIn : public QDialog
{
    Q_OBJECT

public:
    explicit DialogGoodsIn(QWidget *parent = 0);
    ~DialogGoodsIn();
    void initGoods(Goods goods);
    Goods getCurgoods() const;

private slots:
    void on_pushButton_ok_clicked();
    void on_pushButton_exit_clicked();
    void intGoodsCb(Goods g,bool ok);
    void GoodsChange();

private:
    void initCombox();
    void clearUI();
    Ui::DialogGoodsIn *ui;
    Goods  curgoods;
};

#endif // DIALOGGOODSIN_H
