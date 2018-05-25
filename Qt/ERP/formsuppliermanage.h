#ifndef FORMSUPPLIERMANAGE_H
#define FORMSUPPLIERMANAGE_H

#include <QWidget>
#include <QCheckBox>
#include "service_global.h"

namespace Ui {
class FormSupplierManage;
}

class FormSupplierManage : public QWidget
{
    Q_OBJECT

public:
    explicit FormSupplierManage(QWidget *parent = 0);
    ~FormSupplierManage();
    void closeAllStatus();
private slots:

    void on_pushButton_new_clicked();

    void on_pushButton_mod_clicked();

    void on_pushButton_del_clicked();

    void on_pushButton_export_clicked();

    void on_pushButton_exit_clicked();

    void checkBox();

    void exportCb(bool ok);

    void checkAll();

    void delSupplierCb(Supplier cu,bool ok);


signals:
    void sig_exportCb(bool);
private:
    void initData();
    void setRowData(Supplier ma,int row);
    void removeOne(Supplier ma);
    void appendOne(Supplier ma);
    void modOne(Supplier ma);
    void doExport(QVector<Supplier> ls, QString filepath);
private:
    Ui::FormSupplierManage *ui;
    QVector<QCheckBox*> m_boxs;
};

#endif // FORMSUPPLIERMANAGE_H
