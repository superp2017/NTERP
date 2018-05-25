#ifndef FORMUNITMANAGE_H
#define FORMUNITMANAGE_H

#include <QWidget>
#include <QListWidgetItem>

namespace Ui {
class FormUnitManage;
}

class FormUnitManage : public QWidget
{
    Q_OBJECT

public:
    explicit FormUnitManage(QWidget *parent = 0);
    ~FormUnitManage();

private slots:
    void itemClicked(QListWidgetItem*item);

    void on_pushButton_exit_clicked();

    void on_pushButton_del_clicked();

    void on_pushButton_add_clicked();

    void delUnitCb(QString unit,bool ok);

private:
    void initItem();

private:
    Ui::FormUnitManage *ui;
};

#endif // FORMUNITMANAGE_H
