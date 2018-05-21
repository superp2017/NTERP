#ifndef USERTABLE_H
#define USERTABLE_H

#include <QTableWidget>
#include <QHeaderView>
#include "userservice.h"
#include <QMouseEvent>

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
protected:
  //  void mousePressEvent(QMouseEvent *e);

private slots:
    void doubleclickRow(int row,int ool);
    void clickRow(int row,int col);

private:
    void removeAllRow();
    void setRowData(User para, int row);

};

#endif // USERTABLE_H
