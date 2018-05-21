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

private slots:
    void on_pushButton_newUser_clicked();

    void on_pushButton_out_clicked();

    void on_pushButton_del_clicked();

    void changeCol();

    void on_pushButton_mod_clicked();

    void userClick(QString UID);

    void outUserCb(User user, bool ok);

    void delUserCb(User user,bool ok);

private:
    Ui::PersonManager *ui;
    DialogNewUser *newuser;
    QHeaderView::ResizeMode tab_mode;
    User        curUser;
};

#endif // PERSONMANAGER_H
