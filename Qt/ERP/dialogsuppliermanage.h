#ifndef DIALOGSUPPLIERMANAGE_H
#define DIALOGSUPPLIERMANAGE_H

#include <QDialog>

namespace Ui {
class DialogSupplierManage;
}

class DialogSupplierManage : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSupplierManage(QWidget *parent = 0);
    ~DialogSupplierManage();

private:
    Ui::DialogSupplierManage *ui;
};

#endif // DIALOGSUPPLIERMANAGE_H
