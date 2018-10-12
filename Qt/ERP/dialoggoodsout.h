#ifndef DIALOGGOODSOUT_H
#define DIALOGGOODSOUT_H

#include <QDialog>

namespace Ui {
class DialogGoodsOut;
}

class DialogGoodsOut : public QDialog
{
    Q_OBJECT

public:
    explicit DialogGoodsOut(QWidget *parent = 0);
    ~DialogGoodsOut();

private:
    Ui::DialogGoodsOut *ui;
};

#endif // DIALOGGOODSOUT_H
