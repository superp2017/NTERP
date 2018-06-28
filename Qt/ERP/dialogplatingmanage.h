#ifndef DIALOGPLATINGMANAGE_H
#define DIALOGPLATINGMANAGE_H

#include <QDialog>
#include <QListWidgetItem>

namespace Ui {
class DialogPlatingManage;
}

class DialogPlatingManage : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPlatingManage(QWidget *parent = 0);
    ~DialogPlatingManage();
   void initData();
private slots:
    void on_pushButton_add_clicked();

    void on_pushButton_del_clicked();

    void on_pushButton_exit_clicked();

    void delPlatingCb(QString unit,bool ok);

    void newPlatingCb(QString unit,bool ok);


    void itemClicked(QListWidgetItem*item);

private:
    Ui::DialogPlatingManage *ui;
     QString curPlating;
};

#endif // DIALOGPLATINGMANAGE_H
