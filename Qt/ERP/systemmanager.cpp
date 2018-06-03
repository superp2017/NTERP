#include "systemmanager.h"
#include "ui_systemmanager.h"
#include "datacenter.h"

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
