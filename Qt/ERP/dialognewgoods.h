#ifndef DIALOGNEWGOODS_H
#define DIALOGNEWGOODS_H

#include <QDialog>

namespace Ui {
class DialogNewGoods;
}

class DialogNewGoods : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewGoods(QWidget *parent = 0);
    ~DialogNewGoods();

private:
    Ui::DialogNewGoods *ui;
};

#endif // DIALOGNEWGOODS_H
