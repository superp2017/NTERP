#include "storemanager.h"
#include "ui_storemanager.h"

StoreManager::StoreManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StoreManager)
{
    ui->setupUi(this);
}

StoreManager::~StoreManager()
{
    delete ui;
}
