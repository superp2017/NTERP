#ifndef DIALOGPRINTOUTTABLE_H
#define DIALOGPRINTOUTTABLE_H

#include <QDialog>
#include <QCheckBox>
#include "global.h"
#include "printer.h"
#include <QMap>

namespace Ui {
class DialogPrintOutTable;
}
#pragma execution_character_set("utf-8")

class DialogPrintOutTable : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPrintOutTable(QWidget *parent = 0);
    ~DialogPrintOutTable();

private slots:
    void on_pushButton_cancle_clicked();

    void on_pushButton_preview_clicked();

    void on_pushButton_print_clicked();

    void appendOrder(Order para);
    void checkboxChecked();
private:
    void setRowData(Order para, int row);
    QVector<Order> getSelectOrder();

private:
    Ui::DialogPrintOutTable *ui;
    printer m_printer;
    QVector<QCheckBox*> m_boxs;
    QMap<QCheckBox*,Order> m_data;

};

#endif // DIALOGPRINTOUTTABLE_H
