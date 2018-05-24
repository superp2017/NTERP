#ifndef FORMSUPPLIERMANAGE_H
#define FORMSUPPLIERMANAGE_H

#include <QWidget>

namespace Ui {
class FormSupplierManage;
}

class FormSupplierManage : public QWidget
{
    Q_OBJECT

public:
    explicit FormSupplierManage(QWidget *parent = 0);
    ~FormSupplierManage();

private:
    Ui::FormSupplierManage *ui;
};

#endif // FORMSUPPLIERMANAGE_H
