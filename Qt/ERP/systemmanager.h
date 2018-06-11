#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include <QWidget>
#include "nsysconfig.h"
#include "dialogmaterialmanage.h"
#include "formcustommanage.h"
#include "formsuppliermanage.h"
#include "formunitmanage.h"
#include "formwarehousemanage.h"
#include "formgoodstypemanage.h"
#include "formdepartmentmanage.h"
#include "formauthormanage.h"


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
    void on_pushButton_supplier_manage_clicked();
    void on_pushButton_goodstype_manage_clicked();
    void on_pushButton_custon_manage_clicked();
    void on_pushButton_mateial_manage_clicked();
    void on_pushButton_unit_manage_clicked();
    void on_pushButton_about_commpany_clicked();
    void on_pushButton__about_system_clicked();
    void on_pushButton_newOrder_clicked();

    void on_pushButton_department_manage_clicked();

    void on_pushButton_author_manage_clicked();

signals:
    void newOrder();

private:
    void initSetting();
    Ui::SystemManager *ui;
    SysSetting set;
    FormCustommanage     Custom;     //客户管理
    FormSupplierManage   Supplier;   //供应商管理
    DialogMaterialManage Material;   //材料管理
    FormGoodsTypeManage  typeMange;  //分类管理
    FormUnitManage       Unit;       //单位管理
    FormDepartmentManage department; //部门管理
    FormAuthorManage     author;     //权限管理
};

#endif // SYSTEMMANAGER_H
