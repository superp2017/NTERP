﻿#ifndef GOODSTABLE_H
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
    void updateData();
    void appendGoods(Goods g);
    void modGoods(Goods g);
    void removeGoods(QString g);
    void checkAuthor(int index);
signals:
    void inGoods(Goods,bool);
    void outGoods(Goods,bool);
protected:
    void mousePressEvent(QMouseEvent *e);
private slots:
    void modGoods();
    void delGoods();
    void inGoods();
    void outGoods();
    void delGoodsCb(QString GoodsID, bool ok);
private:
    void setRowData(Goods para, int row);
    void initGoods(QVector<Goods>list);
    void updataGoods(QVector<Goods>list);
    QMenu   *m_menu;
    QAction *m_mod;
    QAction *m_del;
    QAction *m_in;
    QAction *m_out;
    Goods   cur_Goods;
};

#endif // GOODSTABLE_H
