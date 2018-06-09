#ifndef FORMCUSTOMMANAGE_H
#define FORMCUSTOMMANAGE_H

#include <QWidget>
#include <QCheckBox>
#include "service_global.h"

namespace Ui {
class FormCustommanage;
}

class FormCustommanage : public QWidget
{
    Q_OBJECT

public:
    explicit FormCustommanage(QWidget *parent = 0);
    ~FormCustommanage();
    void closeAllStatus();
    void initData();
private slots:
    void on_pushButton_del_clicked();

    void on_pushButton_mod_clicked();

    void on_pushButton_exit_clicked();

    void checkBox();

    void exportCb(bool ok);

    void checkAll();

    void on_pushButton_export_clicked();

    void on_pushButton_new_clicked();

    void delCustomerCb(QString cu,bool ok);

    void changeCol();

signals:
    void sig_exportCb(bool);
private:

    void setRowData(Customer ma,int row);
    void removeOne(QString ma);
    void appendOne(Customer ma);
    void modOne(Customer ma);
    void doExport(QVector<Customer> ls, QString filepath);
private:
    Ui::FormCustommanage *ui;
    QVector<QCheckBox*> m_boxs;
};

#endif // FORMCUSTOMMANAGE_H
