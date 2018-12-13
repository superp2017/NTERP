#ifndef FORMMATERMANAGER_H
#define FORMMATERMANAGER_H

#include <QWidget>
#include <QCheckBox>
#include "service_global.h"
#pragma execution_character_set("utf-8")
namespace Ui {
class FormMaterManager;
}

class FormMaterManager : public QWidget
{
    Q_OBJECT

public:
    explicit FormMaterManager(QWidget *parent = 0);
    ~FormMaterManager();
    void closeAllStatus();
    void initData();
private slots:
    void on_pushButton_new_clicked();

    void on_pushButton_mod_clicked();

    void on_pushButton_del_clicked();

    void on_pushButton_export_clicked();

    void on_pushButton_exit_clicked();

    void checkBox();

    void exportCb(bool ok);

    void checkAll();

    void cellDoubleClicked(int row,int col);

    void changeCol();

      void delMaterCb(QString MID,bool ok);
signals:
    void sig_exportCb(bool);
private:
    void setRowData(Materiel ma,int row);
    void removeOne(QString ma);
    void appendOne(Materiel ma);
    void doExport(QVector<Materiel> ls, QString filepath);
private:
    Ui::FormMaterManager *ui;
     QVector<QCheckBox*> m_boxs;
};

#endif // FORMMATERMANAGER_H
