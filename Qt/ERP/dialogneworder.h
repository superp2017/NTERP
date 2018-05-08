#ifndef DIALOGNEWORDER_H
#define DIALOGNEWORDER_H

#include <QDialog>

namespace Ui {
class DialogNewOrder;
}

class DialogNewOrder : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewOrder(QWidget *parent = 0);
    ~DialogNewOrder();

private slots:
    void on_pushButton_ok_clicked();

private:
    Ui::DialogNewOrder *ui;
};

#endif // DIALOGNEWORDER_H
