#ifndef DIALOGNEWGOODS_H
#define DIALOGNEWGOODS_H

#include <QDialog>
#include "goodsService.h"

namespace Ui {
class DialogNewGoods;
}

class DialogNewGoods : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewGoods(QWidget *parent = 0);
    ~DialogNewGoods();
    void setModule(bool isNew);
    void initUI();
    void initData();
    void initGoods(Goods goods);
    Goods getCurGoods() const;

private slots:
    void on_pushButton_ok_clicked();
    void on_pushButton_exit_clicked();
    void newGoodsCb(Goods goods,bool ok);
    void modGoodsCb(Goods goods,bool ok);
private:
    void initCombox(QVector<Supplier> sup,QVector<QString> type,QVector<QString>units);
    Ui::DialogNewGoods *ui;
    bool m_module;
    Goods curGoods;
};

#endif // DIALOGNEWGOODS_H
