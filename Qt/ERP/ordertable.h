#ifndef ORDERTABLE_H
#define ORDERTABLE_H

#include <QTableWidget>
#include <QHeaderView>

#pragma execution_character_set("utf-8")

class OrderTable : public QTableWidget
{
public:
    OrderTable(QTableWidget*w=0);
    void setHeaderColModel( QHeaderView::ResizeMode mode);
};

#endif // ORDERTABLE_H
