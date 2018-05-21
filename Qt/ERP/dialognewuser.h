#ifndef DIALOGNEWUSER_H
#define DIALOGNEWUSER_H

#include "userservice.h"
#include <QDialog>
#pragma execution_character_set("utf-8")

namespace Ui {
class DialogNewUser;
}

class DialogNewUser : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewUser(QWidget *parent = 0);
    ~DialogNewUser();
    User CurUser()const;
    void initUI(User user);
    void setModel(bool isNew);
    void clearUI();
private slots:
    void on_pushButton_creat_clicked();
    void on_pushButton_cancle_clicked();

    void creatUserCb(User user,bool ok);
    void modUserCb(User user,bool ok);
private:
    void initComBox(QVector<QString> department, QVector<QString> autor);
    void changeModel();
private:
    Ui::DialogNewUser *ui;
    bool m_isNewMode;
    User m_curUser;
};

#endif // DIALOGNEWUSER_H
