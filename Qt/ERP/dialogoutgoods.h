#ifndef DIALOGOUTGOODS_H
#define DIALOGOUTGOODS_H

#include <QDialog>
#include "service_global.h"
namespace Ui {
class DialogOutGoods;
}

class DialogOutGoods : public QDialog
{
    Q_OBJECT

public:
    explicit DialogOutGoods(QWidget *parent = 0);
    ~DialogOutGoods();

    GoodsOutRecord getCur_record() const;

private slots:
    void initCombox();
    void on_pushButton_ok_clicked();
    void on_pushButton_cancel_clicked();
    void GoodIDChange();
    void NewOutCb(GoodsOutRecord g,bool ok);

private:
    Ui::DialogOutGoods *ui;
    GoodsOutRecord  cur_record;
};

#endif // DIALOGOUTGOODS_H
