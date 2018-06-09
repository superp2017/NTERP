#ifndef DIALOGINOUTGOODS_H
#define DIALOGINOUTGOODS_H

#include <QDialog>
#include "goodsService.h"

namespace Ui {
class DialogInOutGoods;
}

class DialogInOutGoods : public QDialog
{
    Q_OBJECT

public:
    explicit DialogInOutGoods(QWidget *parent = 0);
    ~DialogInOutGoods();
    void setModule(bool in);
    void initGoods(Goods goods);
    Goods getCurgoods() const;

private slots:
    void on_pushButton_ok_clicked();
    void on_pushButton_exit_clicked();
    void inoutGoodsCb(Goods goods,bool ok);

private:
    Ui::DialogInOutGoods *ui;
    bool isIn;
    Goods  curgoods;
};

#endif // DIALOGINOUTGOODS_H
