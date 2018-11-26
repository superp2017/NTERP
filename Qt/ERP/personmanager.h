﻿#ifndef PERSONMANAGER_H
#define PERSONMANAGER_H

#include <QWidget>
#include "dialognewuser.h"
#include <QHeaderView>
#include "userservice.h"

#pragma execution_character_set("utf-8")


namespace Ui {
class PersonManager;
}

class PersonManager : public QWidget
{
    Q_OBJECT

public:
    explicit PersonManager(QWidget *parent = 0);
    ~PersonManager();
     void checkAuthor(int author);
public slots:
    void new_employee();

private slots:
    void on_pushButton_newUser_clicked();
    void on_pushButton_mod_clicked();
    void on_pushButton_out_clicked();
    void on_pushButton_del_clicked();
    void on_pushButton_export_clicked();
    void on_pushButton_reflash_clicked();

    void changeCol();
    void userClick(QString UID);
    void outUserCb(User user, bool ok);
    void delUserCb(QString user,bool ok);
    void getGlobalUserCb(bool ok);
private:
    void updateData();
    void clearAllSelect();
    void setBtnEnable(bool m, bool o, bool d);

    Ui::PersonManager *ui;
    QHeaderView::ResizeMode tab_mode;
    User        curUser;
};

#endif // PERSONMANAGER_H
