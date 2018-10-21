#ifndef GOODSOUTRECORD_H
#define GOODSOUTRECORD_H
#include "goodsService.h"
#include "m_tablewidget.h"
#pragma execution_character_set("utf-8")


class Tab_GoodsOutRecord: public M_TableWidget
{
    Q_OBJECT
public:
    Tab_GoodsOutRecord(QWidget *w=0);
    void appendRecord(GoodsOutRecord g);
    void removeGoodsRecord(QString g);
    void updateData();
private:
    void initRecords(QVector<GoodsOutRecord>list);
    void setRowData(GoodsOutRecord para, int row);

};

#endif // GOODSOUTRECORD_H
