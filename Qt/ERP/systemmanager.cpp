#include "systemmanager.h"
#include "ui_systemmanager.h"
#include "datacenter.h"
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>


SystemManager::SystemManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SystemManager)
{
    ui->setupUi(this);
    initSetting();
}

SystemManager::~SystemManager()
{
    delete ui;
}

void SystemManager::on_pushButton_update_clicked()
{
    set.isExportOpen = ui->checkBox_export_open->isChecked();
    dataCenter::instance()->setCurSettings(set);
}

void SystemManager::initSetting()
{
    set = dataCenter::instance()->CurSettings();
    ui->checkBox_export_open->setChecked(set.isExportOpen);
}


void SystemManager::aboutCommany()
{
    QString qtManulFile="CompanyIntroduction.pdf";
    QDesktopServices::openUrl(QUrl::fromLocalFile(qtManulFile));


}

void SystemManager::abountSystem()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("关于系统");
    msgBox.setText("本系统由mp开发\n系统版本 V1.0\n公司网站:www.baidu.com");
    msgBox.exec();
}


void SystemManager::on_pushButton_supplier_manage_clicked()
{
     Supplier.showNormal();
}

/*! 物料管理
 * \brief FormCenter::action_material_manage
 */
void SystemManager::action_material_manage()
{
    Material.initData();
    Material.closeAllStatus();
    Material.exec();
}

/*! 供货商管理
 * \brief FormCenter::action_supplier_manage
 */
void SystemManager::action_supplier_manage()
{

}

/*! 客户管理
 * \brief FormCenter::action_customs_manage
 */
void SystemManager::action_customs_manage()
{
    Custom.closeAllStatus();
    Custom.showNormal();
}

/*! 单位管理
 * \brief FormCenter::action_unit_manage
 */
void SystemManager::action_unit_manage()
{
    Unit.showNormal();
}
