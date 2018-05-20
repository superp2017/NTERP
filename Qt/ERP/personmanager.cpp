#include "personmanager.h"
#include "ui_personmanager.h"
#include "dialognewuser.h"


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

void PersonManager::on_pushButton_newUser_clicked()
{
    DialogNewUser newuser;
    newuser.exec();
}

void PersonManager::on_pushButton_out_clicked()
{

}

void PersonManager::on_pushButton_del_clicked()
{

}
