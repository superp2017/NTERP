#ifndef DIALOGGOODSOUTRECORDPRINT_H
#define DIALOGGOODSOUTRECORDPRINT_H

#include <QDialog>
#include <QCheckBox>
#include "goodsoutrecordservice.h"

namespace Ui {
class DialogGoodsOutRecordPrint;
}

class DialogGoodsOutRecordPrint : public QDialog
{
    Q_OBJECT

public:
    explicit DialogGoodsOutRecordPrint(QWidget *parent = 0);
    ~DialogGoodsOutRecordPrint();

private slots:
    void on_pushButton_export_clicked();

    void on_pushButton_cancle_clicked();

    void checkAll();

    void cellChecked(int row, int col);

    void removeAllRow();

    void setRowData(GoodsOutRecord user, int row);

    void exportCb(bool ok);

    void checkBox();

    QVector<GoodsOutRecord> getSelectGoodsRecord();
private:
    void doExport(QVector<GoodsOutRecord> ls, QString filepath);
    void initData(QVector<GoodsOutRecord> list);

signals:
    void sig_exportCb(bool);
private:
    Ui::DialogGoodsOutRecordPrint *ui;
    QVector<GoodsOutRecord>    m_records;
    QVector<QCheckBox*>        m_checkboxs;
};

#endif // DIALOGGOODSOUTRECORDPRINT_H
