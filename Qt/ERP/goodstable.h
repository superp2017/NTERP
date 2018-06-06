#ifndef GOODSTABLE_H
#define GOODSTABLE_H
#include "m_tablewidget.h"

#pragma execution_character_set("utf-8")


class goodsTable : public M_TableWidget
{
public:
    goodsTable(QWidget *w=0);
private:
    void setRowData(Goods para, int row);

};

#endif // GOODSTABLE_H
