#ifndef DIALOGNEWUSER_H
#define DIALOGNEWUSER_H

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

private slots:
    void on_pushButton_creat_clicked();

    void on_pushButton_cancle_clicked();

private:
    void initComBox(QVector<QString> department, QVector<QString> autor);

private:
    Ui::DialogNewUser *ui;
};

#endif // DIALOGNEWUSER_H
