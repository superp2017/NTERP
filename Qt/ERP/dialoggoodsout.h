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

private slots:
    void on_pushButton_out_clicked();

private:
    Ui::DialogGoodsOut *ui;
};

#endif // DIALOGGOODSOUT_H
