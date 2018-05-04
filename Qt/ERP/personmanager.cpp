#include "personmanager.h"
#include "ui_personmanager.h"

PersonManager::PersonManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PersonManager)
{
    ui->setupUi(this);
}

PersonManager::~PersonManager()
{
    delete ui;
}
