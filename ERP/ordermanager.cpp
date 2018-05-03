#include "ordermanager.h"
#include "ui_ordermanager.h"

OrderManager::OrderManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OrderManager)
{
    ui->setupUi(this);
}

OrderManager::~OrderManager()
{
    delete ui;
}
