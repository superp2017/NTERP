#ifndef GOODSTABLE_H
#define GOODSTABLE_H
#include "m_tablewidget.h"
#include "goodsService.h"
#pragma execution_character_set("utf-8")


class goodsTable : public M_TableWidget
{
    Q_OBJECT
public:
    goodsTable(QWidget *w=0);
    void initGoods(QVector<Goods>list);
    void updataGoods(QVector<Goods>list);
    void appendGoods(Goods g);
    void modGoods(Goods g);
    void removeGoods(QString g);
signals:
    void GoodsClick(QString ID);
private slots:
    void clickRow(int row,int col);
private:
    void setRowData(Goods para, int row);

};

#endif // GOODSTABLE_H
