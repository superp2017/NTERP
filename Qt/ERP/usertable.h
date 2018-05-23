﻿#ifndef USERTABLE_H
#define USERTABLE_H

#include <QTableWidget>
#include <QHeaderView>
#include "userservice.h"
#include <QMouseEvent>
#include <QMenu>
#include <QAction>


#pragma execution_character_set("utf-8")

class userTable : public QTableWidget
{
    Q_OBJECT
public:
    userTable(QWidget *w=0);

    void initUser(QVector<User>list);
    void updateUser(QVector<User>list);
    void appendUser(User para);
    void modUser(User para);
    void removeUser(User para);
    void setHeaderColModel( QHeaderView::ResizeMode mode);
signals:
    void userClick(QString UID);
    void newUser();
    void modUser();
    void outUser();
    void delUser();
protected:
    void mousePressEvent(QMouseEvent *e);

private slots:
    void clickRow(int row,int col);

private:
    void removeAllRow();
    void setRowData(User para, int row);

    QMenu *m_menu;
    QAction *m_new;
    QAction *m_mod;
    QAction *m_out;
    QAction *m_del;
};

#endif // USERTABLE_H