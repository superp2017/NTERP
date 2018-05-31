#include "dialognewmateriel.h"
#include "ui_dialognewmateriel.h"
#include <QDateTime>
#include <QCompleter>
#include "datacenter.h"


DialogNewMateriel::DialogNewMateriel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewMateriel)
{
    ui->setupUi(this);
    connect(ui->comboBox_same,SIGNAL(currentTextChanged(QString)),this,SLOT(comBoxdes_change()));
    initCommbox(dataCenter::instance()->pub_Customers(),dataCenter::instance()->pub_Materiels());
}

DialogNewMateriel::~DialogNewMateriel()
{
    delete ui;
}

void DialogNewMateriel::on_pushButton_ok_clicked()
{
    mater.MaterID = QString("%1").arg(QDateTime::currentDateTime().toMSecsSinceEpoch());
    mater.MaterDes = ui->textEdit->toPlainText();
    done(123);
}

void DialogNewMateriel::on_pushButton_cancle_clicked()
{
    done(-1);
}

void DialogNewMateriel::initCommbox(QVector<Customer> cus,QVector<Materiel> ma)
{
    QStringList col;
    col<<"黑色"<<"红色"<<"白色"<<"银色"<<"银白色";
    QCompleter *completercol = new QCompleter(col, this);
    ui->comboBox_color->clear();
    ui->comboBox_color->addItems(col);
    ui->comboBox_color->setEditable(true);
    ui->comboBox_color->setCompleter(completercol);

    QStringList type;
    type<<"锌涂层"<<"铝涂层"<<"锌铝涂层";
    QCompleter *completertype = new QCompleter(type, this);
    ui->comboBox_type->clear();
    ui->comboBox_type->addItems(type);
    ui->comboBox_type->setEditable(true);
    ui->comboBox_type->setCompleter(completertype);

    QCompleter *completerCus = new QCompleter(type, this);
    ui->comboBox_custom->clear();
    ui->comboBox_custom->setEditable(true);
    for(Customer c:cus){
        ui->comboBox_custom->addItem(c.Name+"("+c.CID+")",c.CID);
    }
    ui->comboBox_custom->setCompleter(completerCus);

    for(Materiel m:ma){
        ui->comboBox_same->addItem(m.MaterDes);
    }

}

Materiel DialogNewMateriel::getMater() const
{
    return mater;
}

void DialogNewMateriel::on_pushButton_add_custom_clicked()
{
    ui->textEdit->setText(ui->textEdit->toPlainText()+"/"+ui->comboBox_custom->currentData().toString());

}

void DialogNewMateriel::on_pushButton_Add_color_clicked()
{
    ui->textEdit->setText(ui->textEdit->toPlainText()+"/"+ui->comboBox_color->currentText());
}

void DialogNewMateriel::on_pushButton_add_type_clicked()
{
    ui->textEdit->setText(ui->textEdit->toPlainText()+"/"+ui->comboBox_type->currentText());
}

void DialogNewMateriel::on_pushButton_clear_clicked()
{
    ui->textEdit->clear();
}

void DialogNewMateriel::comBoxdes_change()
{
    QString str = ui->comboBox_same->currentText();
    if(!str.isEmpty()){
        ui->textEdit->setText(str);
    }
}


