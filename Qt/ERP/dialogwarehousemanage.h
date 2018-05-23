#ifndef DIALOGWAREHOUSEMANAGE_H
#define DIALOGWAREHOUSEMANAGE_H

#include <QDialog>

namespace Ui {
class DialogWarehouseManage;
}

class DialogWarehouseManage : public QDialog
{
    Q_OBJECT

public:
    explicit DialogWarehouseManage(QWidget *parent = 0);
    ~DialogWarehouseManage();

private:
    Ui::DialogWarehouseManage *ui;
};

#endif // DIALOGWAREHOUSEMANAGE_H
