#ifndef STOREMANAGER_H
#define STOREMANAGER_H

#include <QWidget>
#include "goodsService.h"
#include "goodstable.h"
#include "goodsoutrecord.h"
#include "formsuppliermanage.h"

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
    void checkAuthor(int author);

private slots:
    void on_pushButton_new_clicked();
    void on_pushButton_in_store_clicked();
    void on_pushButton_out_store_clicked();
    void on_pushButton_export_clicked();
    void on_pushButton_reflash_clicked();
    void inGoods(Goods g, bool e);
    void outGoods(Goods g,bool e);
    void queryGoods(Goods g);

    //  void GoodsClick(QString id);
    //  void delGoodsCb(QString ID,bool ok);
    void getGlobalGoodsCb(bool ok);
    void getAllGoodsRecordCb(bool ok);
    void changeCol();
    void clearSelectSection();

    void on_pushButton_supply_manager_clicked();

private:
    void initData();
    void setBtnEnable(bool mod, bool in, bool out, bool del);
    Ui::StoreManager *ui;
    goodsTable  m_goods_Table;
    Tab_GoodsOutRecord m_record_Table;
    Goods   cur_Goods;
    FormSupplierManage supplier;
};

#endif // STOREMANAGER_H
