#ifndef STOREMANAGER_H
#define STOREMANAGER_H

#include <QWidget>
#include "goodsService.h"
#include "dialognewgoods.h"
#include "dialoginoutgoods.h"
#include "goodstable.h"
#include "goodsoutrecord.h"

#pragma execution_character_set("utf-8")


namespace Ui {
class StoreManager;
}

class StoreManager : public QWidget
{
    Q_OBJECT

public:
    explicit StoreManager(QWidget *parent = 0);
    ~StoreManager();

private slots:
    void on_pushButton_new_clicked();
    void on_pushButton_in_store_clicked();
    void on_pushButton_out_store_clicked();
    void on_pushButton_del_clicked();
    void on_pushButton_export_clicked();
    void on_pushButton_reflash_clicked();

    void GoodsClick(QString id);
    void delGoodsCb(QString ID,bool ok);
    void getGlobalGoodsCb(bool ok);
    void changeCol();
    void on_pushButton_mod_clicked();
    void clearSelectSection();
private:
    void initData();
    void setBtnEnable(bool mod, bool in, bool out, bool del);
    Ui::StoreManager *ui;
    DialogNewGoods  *newGoods;
    DialogInOutGoods inout;
    goodsTable  m_goods_Table;
    Tab_GoodsOutRecord m_record_Table;
    Goods   cur_Goods;
};

#endif // STOREMANAGER_H
