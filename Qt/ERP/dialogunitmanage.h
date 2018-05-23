#ifndef DIALOGUNITMANAGE_H
#define DIALOGUNITMANAGE_H

#include <QDialog>

namespace Ui {
class DialogUnitManage;
}

class DialogUnitManage : public QDialog
{
    Q_OBJECT

public:
    explicit DialogUnitManage(QWidget *parent = 0);
    ~DialogUnitManage();

private:
    Ui::DialogUnitManage *ui;
};

#endif // DIALOGUNITMANAGE_H
