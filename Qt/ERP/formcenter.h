#ifndef FORMCENTER_H
#define FORMCENTER_H

#include <QWidget>
#include "storemanager.h"
#include "personmanager.h"
#include "ordermanager.h"
#include "systemmanager.h"

#include "dialogmaterialmanage.h"
#include "formcustommanage.h"
#include "formsuppliermanage.h"
#include "formunitmanage.h"
#include "formwarehousemanage.h"

#include <QTimer>

#pragma execution_character_set("utf-8")
namespace Ui {
class FormCenter;
}

class FormCenter : public QWidget
{
    Q_OBJECT

public:
    explicit FormCenter(QWidget *parent = 0);
    ~FormCenter();

signals:
    void  action_new_order();
    void  action_new_user();
    void  show_mini();
    void  exitApp();
public slots:

    void  action_material_manage();
    void  action_supplier_manage();
    void  action_customs_manage();
    void  action_unit_manage();

private slots:
    void on_order_btn_clicked();

    void on_store_btn_clicked();

    void on_person_btn_clicked();

    void on_set_Btn_clicked();

    void on_exit_clicked();

    void clearChecked();

    void timeoutslot();
    void showMessage(QString msg,int delay);
    void on_pushButton_mini_clicked();

private:
    Ui::FormCenter *ui;
    OrderManager         m_order;    //订单管理
    StoreManager         m_store;    //仓库管理
    PersonManager        m_person;   //员工管理
    SystemManager        m_sys;      //系统管理
    DialogMaterialManage Material;   //材料管理
    FormCustommanage     Custom;     //客户管理
    FormUnitManage       Unit;       //单位管理
    FormSupplierManage   Supplier;   //供应商管理
    QTimer               *m_timer;
};

#endif // FORMCENTER_H
