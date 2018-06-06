#ifndef GOODSTABLE_H
#define GOODSTABLE_H
#include "m_tablewidget.h"
#include "goodsService.h"
#pragma execution_character_set("utf-8")


class goodsTable : public M_TableWidget
{
public:
    goodsTable(QWidget *w=0);
private:
    void setRowData(Goods para, int row);
    void initGoods(QVector<Goods>list);
    void appendGoods(Goods g);
    void modGoods(Goods g);
    void removeGoods(QString g);
};

#endif // GOODSTABLE_H
