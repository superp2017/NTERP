#ifndef DIALOGGOODSPRINT_H
#define DIALOGGOODSPRINT_H

#include <QDialog>
#include "goodsService.h"
#include <QCheckBox>
#pragma execution_character_set("utf-8")

namespace Ui {
class DialogGoodsPrint;
}

class DialogGoodsPrint : public QDialog
{
    Q_OBJECT

public:
    explicit DialogGoodsPrint(QWidget *parent = 0);
    ~DialogGoodsPrint();

private slots:
    void on_pushButton_cancle_clicked();
    void on_pushButton_export_clicked();
    void TypeChange();
    void removeAllRow();
    void checkBox();

    void on_checkBox_check_all_clicked();

signals:
    void sig_exportCb(bool);

private:
    void setRowData(Goods goods, int row);
    void doExport(QVector<Goods> ls, QString filepath);
    void initComBox();
    Ui::DialogGoodsPrint *ui;
    QVector<Goods>      mData;
    QString             curType;
    QVector<QCheckBox*> m_checkboxs;
};

#endif // DIALOGGOODSPRINT_H
