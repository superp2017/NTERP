#ifndef FORMUNITMANAGE_H
#define FORMUNITMANAGE_H

#include <QWidget>

namespace Ui {
class FormUnitManage;
}

class FormUnitManage : public QWidget
{
    Q_OBJECT

public:
    explicit FormUnitManage(QWidget *parent = 0);
    ~FormUnitManage();

private:
    Ui::FormUnitManage *ui;
};

#endif // FORMUNITMANAGE_H
