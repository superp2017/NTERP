#include "dialogneworder.h"
#include "ui_dialogneworder.h"
#include <QCompleter>

DialogNewOrder::DialogNewOrder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewOrder)
{
    ui->setupUi(this);
    QStringList strings;

    strings << "Biao" << "Biao Huang" << "Mac" << "MacBook" << "MacBook Pro" << "Mac Pro";

   QCompleter *completer = new QCompleter(strings, this);

   ui->comboBox_2->clear();

   ui->comboBox_2->addItems(strings);

   ui->comboBox_2->setEditable(true);

   ui->comboBox_2->setCompleter(completer);

}

DialogNewOrder::~DialogNewOrder()
{
    delete ui;
}

void DialogNewOrder::on_pushButton_ok_clicked()
{

}
