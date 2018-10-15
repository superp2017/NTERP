#ifndef GOODSTABLE_H
#define GOODSTABLE_H
#include "m_tablewidget.h"
#include "goodsService.h"
#include <QAction>
#include <QMenu>
#include <QMouseEvent>
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
    void modGoods();
    void delGoods();
protected:
    void mousePressEvent(QMouseEvent *e);
private slots:
    void clickRow(int row,int col);
private:
    void setRowData(Goods para, int row);
    QMenu   *m_menu;
    QAction *m_mod;
    QAction *m_del;
};

#endif // GOODSTABLE_H
