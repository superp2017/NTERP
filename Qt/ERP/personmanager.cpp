#include "personmanager.h"
#include "ui_personmanager.h"


PersonManager::PersonManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PersonManager)
{
    ui->setupUi(this);
    tab_mode = QHeaderView::Stretch;
    newuser =NULL;
    ui->radioButton_ave->setChecked(true);
}

PersonManager::~PersonManager()
{
    delete ui;
}

void PersonManager::on_pushButton_newUser_clicked()
{
    if(newuser==NULL){
        newuser = new DialogNewUser();
    }
    newuser->setModel(true);
    newuser->clearUI();
    if(newuser->exec()==123){
        User user = newuser->CurUser();
        ui->tableWidget->appendUser(user);
    }
}

void PersonManager::on_pushButton_out_clicked()
{

}

void PersonManager::on_pushButton_del_clicked()
{

}
