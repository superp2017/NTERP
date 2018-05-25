#ifndef DIALOGNEWSUPPLIER_H
#define DIALOGNEWSUPPLIER_H

#include <QDialog>

#include "service_global.h"

namespace Ui {
class DialogNewSupplier;
}

class DialogNewSupplier : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewSupplier(QWidget *parent = 0);
    ~DialogNewSupplier();

    Supplier getCursupplier() const;
    void setMode(bool isNew);
    void initSupplier(Supplier sup);
private slots:
    void on_pushButton_cancel_clicked();

    void on_pushButton_ok_clicked();

    void modSupplierCb(Supplier c,bool ok);
    void newSupplierCb(Supplier c,bool ok);

private:
    Ui::DialogNewSupplier *ui;
    Supplier cursupplier;
    bool m_isNewMod;
};

#endif // DIALOGNEWSUPPLIER_H
