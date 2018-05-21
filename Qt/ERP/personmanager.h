#ifndef PERSONMANAGER_H
#define PERSONMANAGER_H

#include <QWidget>
#include "dialognewuser.h"
#include <QHeaderView>


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

private:
    Ui::PersonManager *ui;
    DialogNewUser *newuser;
    QHeaderView::ResizeMode tab_mode;
};

#endif // PERSONMANAGER_H
