#include "systemmanager.h"
#include "ui_systemmanager.h"
#include "datacenter.h"
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include "dialogplatingmanage.h"
#include "dialogprintouttable.h"

SystemManager::SystemManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SystemManager)
{
    ui->setupUi(this);
    initSetting();

    ui->pushButton_update->setStyleSheet("QPushButton{border-image: url(:/icon/modify-red.png);}"
                                         "QPushButton:hover{border-image: url(:/icon/modify.png);}"
                                         "QPushButton:pressed{border-image: url(:/icon/modify.png);}"
                                         "QPushButton:checked{border-image: url(:/icon/modify.png);}");


    ui->pushButton_newOrder->setStyleSheet("QPushButton{border-image: url(:/icon/Order-red.png);}"
                                           "QPushButton:hover{border-image: url(:/icon/Order.png);}"
                                           "QPushButton:pressed{border-image: url(:/icon/Order.png);}"
                                           "QPushButton:checked{border-image: url(:/icon/Order.png);}");
    ui->pushButton_about_commpany->setStyleSheet("QPushButton{border-image: url(:/icon/company-red.png);}"
                                                 "QPushButton:hover{border-image: url(:/icon/company.png);}"
                                                 "QPushButton:pressed{border-image: url(:/icon/company.png);}"
                                                 "QPushButton:checked{border-image: url(:/icon/company.png);}");
    ui->pushButton_custon_manage->setStyleSheet("QPushButton{border-image: url(:/icon/Customer-red.png);}"
                                                "QPushButton:hover{border-image: url(:/icon/Customer.png);}"
                                                "QPushButton:pressed{border-image: url(:/icon/Customer.png);}"
                                                "QPushButton:checked{border-image: url(:/icon/Customer.png);}");
    ui->pushButton_goodstype_manage->setStyleSheet("QPushButton{border-image: url(:/icon/classification-red.png);}"
                                                   "QPushButton:hover{border-image: url(:/icon/classification.png);}"
                                                   "QPushButton:pressed{border-image: url(:/icon/classification.png);}"
                                                   "QPushButton:checked{border-image: url(:/icon/classification.png);}");
    ui->pushButton_mateial_manage->setStyleSheet("QPushButton{border-image: url(:/icon/materiel-red.png);}"
                                                 "QPushButton:hover{border-image: url(:/icon/materiel.png);}"
                                                 "QPushButton:pressed{border-image: url(:/icon/materiel.png);}"
                                                 "QPushButton:checked{border-image: url(:/icon/materiel.png);}");
    ui->pushButton_supplier_manage->setStyleSheet("QPushButton{border-image: url(:/icon/Supply-red.png);}"
                                                  "QPushButton:hover{border-image: url(:/icon/Supply.png);}"
                                                  "QPushButton:pressed{border-image: url(:/icon/Supply.png);}"
                                                  "QPushButton:checked{border-image: url(:/icon/Supply.png);}");
    ui->pushButton_unit_manage->setStyleSheet("QPushButton{border-image: url(:/icon/t-red.png);}"
                                              "QPushButton:hover{border-image: url(:/icon/t.png);}"
                                              "QPushButton:pressed{border-image: url(:/icon/t.png);}"
                                              "QPushButton:checked{border-image: url(:/icon/t.png);}");

    ui->pushButton_plating->setStyleSheet("QPushButton{border-image: url(:/icon/duzhong-red.png);}"
                                          "QPushButton:hover{border-image: url(:/icon/duzhong.png);}"
                                          "QPushButton:pressed{border-image: url(:/icon/duzhong.png);}"
                                          "QPushButton:checked{border-image: url(:/icon/duzhong.png);}");

    ui->pushButton__about_system->setStyleSheet("QPushButton{border-image: url(:/icon/system-red.png);}"
                                                "QPushButton:hover{border-image: url(:/icon/system.png);}"
                                                "QPushButton:pressed{border-image: url(:/icon/system.png);}"
                                                "QPushButton:checked{border-image: url(:/icon/system.png);}");

    ui->pushButton_department_manage->setStyleSheet("QPushButton{border-image: url(:/icon/department-red.png);}"
                                                    "QPushButton:hover{border-image: url(:/icon/department.png);}"
                                                    "QPushButton:pressed{border-image: url(:/icon/department.png);}"
                                                    "QPushButton:checked{border-image: url(:/icon/department.png);}");
    ui->pushButton_author_manage->setStyleSheet("QPushButton{border-image: url(:/icon/Jurisdiction-red.png);}"
                                                "QPushButton:hover{border-image: url(:/icon/Jurisdiction.png);}"
                                                "QPushButton:pressed{border-image: url(:/icon/Jurisdiction.png);}"
                                                "QPushButton:checked{border-image: url(:/icon/Jurisdiction.png);}");
    ui->pushButton_out_print->setStyleSheet("QPushButton{border-image: url(:/icon/outprint_big-red.png);}"
                                            "QPushButton:hover{border-image: url(:/icon/outprint_big.png);}"
                                            "QPushButton:pressed{border-image: url(:/icon/outprint_big.png);}"
                                            "QPushButton:checked{border-image: url(:/icon/outprint_big.png);}");
    checkAuthor(dataCenter::instance()->pub_CurUser().Author);
}

SystemManager::~SystemManager()
{
    delete ui;
}

void SystemManager::checkAuthor(int author)
{
    switch (author) {
    case 0:
        ui->pushButton_author_manage->setStyleSheet("QPushButton{border-image: url(:/icon/Jurisdiction.png);}");
        ui->pushButton_department_manage->setStyleSheet("QPushButton{border-image: url(:/icon/department.png);}");
        ui->pushButton_supplier_manage->setStyleSheet("QPushButton{border-image: url(:/icon/Supply.png);}");
        ui->pushButton_custon_manage->setStyleSheet("QPushButton{border-image: url(:/icon/Customer.png);}");
        ui->pushButton_goodstype_manage->setStyleSheet("QPushButton{border-image: url(:/icon/classification.png);}");
        break;
    case 1:
        ui->pushButton_author_manage->setStyleSheet("QPushButton{border-image: url(:/icon/Jurisdiction.png);}");
        ui->pushButton_department_manage->setStyleSheet("QPushButton{border-image: url(:/icon/department.png);}");
        ui->pushButton_newOrder->setStyleSheet("QPushButton{border-image: url(:/icon/Order.png);}");
        ui->pushButton_out_print->setStyleSheet("QPushButton{border-image: url(:/icon/outprint_big.png);}");
        ui->pushButton_mateial_manage->setStyleSheet("QPushButton{border-image: url(:/icon/materiel.png);}");
        ui->pushButton_unit_manage->setStyleSheet("QPushButton{border-image: url(:/icon/t.png);}");
        ui->pushButton_plating->setStyleSheet("QPushButton{border-image: url(:/icon/duzhong.png);}");
        ui->pushButton_custon_manage->setStyleSheet("QPushButton{border-image: url(:/icon/Customer.png);}");
        break;
    case 2:
        ui->pushButton_author_manage->setStyleSheet("QPushButton{border-image: url(:/icon/Jurisdiction.png);}");
        ui->pushButton_newOrder->setStyleSheet("QPushButton{border-image: url(:/icon/Order.png);}");
        ui->pushButton_mateial_manage->setStyleSheet("QPushButton{border-image: url(:/icon/materiel.png);}");
        ui->pushButton_unit_manage->setStyleSheet("QPushButton{border-image: url(:/icon/t.png);}");
        ui->pushButton_plating->setStyleSheet("QPushButton{border-image: url(:/icon/duzhong.png);}");
        ui->pushButton_goodstype_manage->setStyleSheet("QPushButton{border-image: url(:/icon/classification.png);}");
        break;
    default:
        break;
    }
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


void SystemManager::on_pushButton_supplier_manage_clicked()
{
    AUTHOR_Equal(0);
    Supplier.initData();
    Supplier.showNormal();
}

void SystemManager::on_pushButton_goodstype_manage_clicked()
{
       AUTHOR_Equal(0);
       AUTHOR_Equal(2);
    typeMange.initData();
    typeMange.showNormal();
}

void SystemManager::on_pushButton_custon_manage_clicked()
{
    AUTHOR_Equal(0);
    AUTHOR_Equal(1);
    Custom.initData();
    Custom.showNormal();
}

void SystemManager::on_pushButton_mateial_manage_clicked()
{
    AUTHOR_Equal(1);
    AUTHOR_Equal(2);
    Material.initData();
    Material.showNormal();
}

void SystemManager::on_pushButton_unit_manage_clicked()
{
    AUTHOR_Equal(1);
    AUTHOR_Equal(2);
    Unit.initData();
    Unit.showNormal();
}

void SystemManager::on_pushButton_about_commpany_clicked()
{
    QString qtManulFile="CompanyIntroduction.pdf";
    QDesktopServices::openUrl(QUrl::fromLocalFile(qtManulFile));
}

void SystemManager::on_pushButton__about_system_clicked()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("关于系统");
    msgBox.setText("系统版本 v1.0\n如出现问题,请联系运维人员！");
    msgBox.exec();
}

void SystemManager::on_pushButton_newOrder_clicked()
{
    AUTHOR_Equal(1);
    AUTHOR_Equal(2);
    emit newOrder();
}

void SystemManager::on_pushButton_department_manage_clicked()
{
    AUTHOR_Equal(0);
    AUTHOR_Equal(1);
    department.initData();
    department.showNormal();
}

void SystemManager::on_pushButton_author_manage_clicked()
{
    AUTHOR_Equal(0);
    AUTHOR_Equal(1);
    AUTHOR_Equal(2);
    QString qtManulFile="权限说明.pdf";
    QDesktopServices::openUrl(QUrl::fromLocalFile(qtManulFile));
}

void SystemManager::on_pushButton_plating_clicked()
{
    AUTHOR_Equal(1);
    AUTHOR_Equal(2);
    DialogPlatingManage e;
    e.exec();
}

void SystemManager::on_pushButton_out_print_clicked()
{
    AUTHOR_Equal(1);
    DialogPrintOutTable print;
    print.exec();
}
