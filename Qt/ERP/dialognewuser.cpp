#include "dialognewuser.h"
#include "ui_dialognewuser.h"
#include <QCompleter>
#include <QToolTip>
#include "datacenter.h"


DialogNewUser::DialogNewUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewUser)
{
    ui->setupUi(this);
    initComBox(dataCenter::instance()->getDepartments(),dataCenter::instance()->getAuthors());
}

DialogNewUser::~DialogNewUser()
{
    delete ui;
}

void DialogNewUser::on_pushButton_creat_clicked()
{

}

void DialogNewUser::on_pushButton_cancle_clicked()
{

}

void DialogNewUser::initComBox(QVector<QString>department,QVector<QString>autor)
{
    ui->comboBox_sex->addItem("男","0");
    ui->comboBox_sex->addItem("女","1");

    ui->comboBox_department->clear();
    QCompleter *completerauthor = new QCompleter(department.toList(), this);
    ui->comboBox_department->setEditable(true);
    for(QString de:department){
        ui->comboBox_department->addItem(de);
    }
    ui->comboBox_department->addItem(ItemNewDepartment);
    ui->comboBox_department->setCompleter(completerauthor);


    ui->comboBox_author->clear();
    QCompleter *completerDe = new QCompleter(autor.toList(), this);
    ui->comboBox_author->setEditable(true);
    for(QString au:autor){
        ui->comboBox_author->addItem(au);
    }
    ui->comboBox_author->addItem(ItemNewAuthor);

    ui->comboBox_author->setCompleter(completerDe);

}
