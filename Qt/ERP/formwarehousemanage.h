#ifndef FORMWAREHOUSEMANAGE_H
#define FORMWAREHOUSEMANAGE_H

#include <QWidget>

namespace Ui {
class FormWarehouseManage;
}

class FormWarehouseManage : public QWidget
{
    Q_OBJECT

public:
    explicit FormWarehouseManage(QWidget *parent = 0);
    ~FormWarehouseManage();

private:
    Ui::FormWarehouseManage *ui;
};

#endif // FORMWAREHOUSEMANAGE_H
