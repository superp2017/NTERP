#ifndef DIALOGOUTGOODS_H
#define DIALOGOUTGOODS_H

#include <QDialog>

namespace Ui {
class DialogOutGoods;
}

class DialogOutGoods : public QDialog
{
    Q_OBJECT

public:
    explicit DialogOutGoods(QWidget *parent = 0);
    ~DialogOutGoods();

private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::DialogOutGoods *ui;
};

#endif // DIALOGOUTGOODS_H
