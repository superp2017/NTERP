#ifndef FORMGOODSTYPEMANAGE_H
#define FORMGOODSTYPEMANAGE_H

#include <QWidget>
#include <QListWidgetItem>

#pragma execution_character_set("utf-8")

namespace Ui {
class FormGoodsTypeManage;
}

class FormGoodsTypeManage : public QWidget
{
    Q_OBJECT

public:
    explicit FormGoodsTypeManage(QWidget *parent = 0);
    ~FormGoodsTypeManage();
    void initData();
private slots:
    void on_pushButton_add_clicked();

    void on_pushButton_del_clicked();

    void on_pushButton_exit_clicked();

    void newTypeCb(QString type,bool ok);

    void delTypeCb(QString type,bool ok);

    void itemClicked(QListWidgetItem*);
private:
    Ui::FormGoodsTypeManage *ui;
     QString curType;
};

#endif // FORMGOODSTYPEMANAGE_H
