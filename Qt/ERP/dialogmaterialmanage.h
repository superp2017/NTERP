#ifndef DIALOGMATERIALMANAGE_H
#define DIALOGMATERIALMANAGE_H

#include <QDialog>

namespace Ui {
class DialogMaterialManage;
}

class DialogMaterialManage : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMaterialManage(QWidget *parent = 0);
    ~DialogMaterialManage();

private:
    Ui::DialogMaterialManage *ui;
};

#endif // DIALOGMATERIALMANAGE_H
