#include "systemmanager.h"
#include "ui_systemmanager.h"

SystemManager::SystemManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SystemManager)
{
    ui->setupUi(this);
}

SystemManager::~SystemManager()
{
    delete ui;
}
