#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include <QWidget>
#include "nsysconfig.h"
#include "dialogmaterialmanage.h"
#include "formcustommanage.h"
#include "formsuppliermanage.h"
#include "formunitmanage.h"
#include "formwarehousemanage.h"


#pragma execution_character_set("utf-8")


namespace Ui {
class SystemManager;
}

class SystemManager : public QWidget
{
    Q_OBJECT

public:
    explicit SystemManager(QWidget *parent = 0);
    ~SystemManager();

private slots:
    void on_pushButton_update_clicked();
    void aboutCommany();
    void abountSystem();
    void on_pushButton_supplier_manage_clicked();

public slots:

    void  action_material_manage();
    void  action_supplier_manage();
    void  action_customs_manage();
    void  action_unit_manage();
private:
    void initSetting();
    Ui::SystemManager *ui;
    SysSetting set;
    DialogMaterialManage Material;   //材料管理
    FormCustommanage     Custom;     //客户管理
    FormUnitManage       Unit;       //单位管理
    FormSupplierManage   Supplier;   //供应商管理
};

#endif // SYSTEMMANAGER_H
