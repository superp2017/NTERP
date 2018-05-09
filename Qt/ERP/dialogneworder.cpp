#include "dialogneworder.h"
#include "ui_dialogneworder.h"
#include "datacenter.h"
#include <QCompleter>

DialogNewOrder::DialogNewOrder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewOrder)
{
    ui->setupUi(this);
    curorder = NULL;
    initCombox(dataCenter::instance()->Customers(),\
               dataCenter::instance()->Batchs(),\
               dataCenter::instance()->Materiels(),\
               dataCenter::instance()->Units());
}

DialogNewOrder::~DialogNewOrder()
{
    delete ui;
}

Order *DialogNewOrder::getOrder()
{
    return curorder;
}

void DialogNewOrder::on_pushButton_ok_clicked()
{
    done(123);
}
void DialogNewOrder::initCombox(QVector<Customer*> custom, QVector<QString> batch, \
                                QVector<Materiel *> materID, QVector<QString> unit)
{
    ui->comboBox_customerName->clear();
    QStringList customlist;
    for(Customer* c:custom){
        customlist<<c->Name;
        ui->comboBox_customerName->addItem(c->Name,c->CID);
    }
    QCompleter *completerCustomer = new QCompleter(customlist, this);

    ui->comboBox_customerName->setEditable(true);
    ui->comboBox_customerName->setCompleter(completerCustomer);



    QCompleter *completerBatch = new QCompleter(batch.toList(), this);
    ui->comboBox_CustomBatch->clear();
    ui->comboBox_CustomBatch->addItems(batch.toList());
    ui->comboBox_CustomBatch->setEditable(true);
    ui->comboBox_CustomBatch->setCompleter(completerBatch);

    QStringList materlist;
    for(Materiel* c:materID){
        materlist<<c->MaterDes;
    }

    QCompleter *completerMater = new QCompleter(materlist, this);
    ui->comboBox_MaterielID->clear();
    ui->comboBox_MaterielID->addItems(materlist);
    ui->comboBox_MaterielID->setEditable(true);
    ui->comboBox_MaterielID->setCompleter(completerMater);

    QCompleter *completerUnit = new QCompleter(unit.toList(), this);
    ui->comboBox_MaterielID->clear();
    ui->comboBox_MaterielID->addItems(unit.toList());
    ui->comboBox_MaterielID->setEditable(true);
    ui->comboBox_MaterielID->setCompleter(completerUnit);


    ui->comboBox_MaterielID->addItem(ItemNewMater);
    ui->comboBox_customerName->addItem(ItemNewCustom);
    ui->comboBox_unit->addItem(ItemNewUnit);
    ui->comboBox_customerName->setCurrentIndex(-1);
    ui->comboBox_MaterielID->setCurrentIndex(-1);
}
