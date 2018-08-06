#include "dialognewmateriel.h"
#include "ui_dialognewmateriel.h"
#include <QDateTime>
#include <QCompleter>
#include "datacenter.h"

#include<QDebug>
DialogNewMateriel::DialogNewMateriel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewMateriel)
{
    ui->setupUi(this);

    connect(ui->comboBox_friction,SIGNAL(currentTextChanged(QString)),this,SLOT(DesChange()));
    connect(ui->comboBox_thickness,SIGNAL(currentTextChanged(QString)),this,SLOT(DesChange()));
    connect(ui->comboBox_Salt,SIGNAL(currentTextChanged(QString)),this,SLOT(DesChange()));
    connect(ui->comboBox_type,SIGNAL(currentTextChanged(QString)),this,SLOT(DesChange()));
}

DialogNewMateriel::~DialogNewMateriel()
{
    delete ui;
}

void DialogNewMateriel::on_pushButton_ok_clicked()
{
    mater.MaterID = QString("%1").arg(QDateTime::currentDateTime().toMSecsSinceEpoch());
    mater.MaterDes = ui->textEdit->toPlainText();
    mater.ComponentFormat = ui->comboBox_format->currentText();
    mater.ComponentSolid = ui->comboBox_solid->currentText();
    mater.Friction = ui->comboBox_friction->currentText();
    mater.Plating = ui->comboBox_type->currentText();
    mater.Salt = ui->comboBox_Salt->currentText();
    mater.Thickness = ui->comboBox_thickness->currentText();

    done(123);
}

void DialogNewMateriel::on_pushButton_cancle_clicked()
{
    done(-1);
}



void DialogNewMateriel::initCommbox()
{
    ui->comboBox_solid->blockSignals(true);
    ui->comboBox_friction->blockSignals(true);
    ui->comboBox_format->blockSignals(true);
    ui->comboBox_Salt->blockSignals(true);
    ui->comboBox_thickness->blockSignals(true);
    ui->comboBox_type->blockSignals(true);

    QStringList solid;
    QStringList format;
    mater_list=   dataCenter::instance()->pub_Materiels();
    for(Materiel ma:mater_list){
        if(!ma.ComponentSolid.trimmed().isEmpty())
            solid<<ma.ComponentSolid;
        if(!ma.ComponentFormat.trimmed().isEmpty())
            format<<ma.ComponentFormat;
    }
    if(solid.size()==0)
        solid<<"";
    if(format.size()==0)
        format<<"";
    if(solid.size()>0){
        QCompleter *completersolid= new QCompleter(solid, this);
        ui->comboBox_solid->clear();
        ui->comboBox_solid->setEditable(true);
        ui->comboBox_solid->addItems(solid);
        ui->comboBox_solid->setCompleter(completersolid);
    }
    if(format.size()>0){
        QCompleter *completerformat = new QCompleter(format, this);
        ui->comboBox_format->clear();
        ui->comboBox_format->setEditable(true);
        ui->comboBox_format->addItems(format);
        ui->comboBox_format->setCompleter(completerformat);
    }

    QVector<QString> platings =  dataCenter::instance()->pub_Platings();
    QStringList type;
    for(QString p:platings){
        type <<p;
    }
    QCompleter *completertype = new QCompleter(type, this);
    ui->comboBox_type->clear();
    ui->comboBox_type->addItems(type);
    ui->comboBox_type->setEditable(true);
    ui->comboBox_type->setCompleter(completertype);

    QStringList friction;
    friction<<"0.06~0.09"<<"0.09~0.15"<<"0.08~0.14"<<"0.10~0.16"<<"0.12~0.18"<<"0.18以上";
    QCompleter *completerCus = new QCompleter(friction, this);
    ui->comboBox_friction->clear();
    ui->comboBox_friction->setEditable(true);
    ui->comboBox_friction->addItems(friction);
    ui->comboBox_friction->setCompleter(completerCus);

    QStringList salt;
    salt<<"120h"<<"480h"<<"500h"<<"1000h"<<"120/240h"<<"240/720h"<<"840h"<<"240/1000h";
    QCompleter *completersalt = new QCompleter(salt, this);
    ui->comboBox_Salt->clear();
    ui->comboBox_Salt->setEditable(true);
    ui->comboBox_Salt->addItems(salt);
    ui->comboBox_Salt->setCompleter(completersalt);

    QStringList thick;
    thick<<"5um以上"<<"8um以上"<<"10um以上"<<"12um以上"<<"13um以上"<<"8~20um"<<"10~25um"<<"12~25um"<<"10~20um"<<"6~20um";
    QCompleter *completerThickness= new QCompleter(thick, this);
    ui->comboBox_thickness->clear();
    ui->comboBox_thickness->setEditable(true);
    ui->comboBox_thickness->addItems(thick);
    ui->comboBox_thickness->setCompleter(completerThickness);

    ui->comboBox_solid->blockSignals(false);
    ui->comboBox_friction->blockSignals(false);
    ui->comboBox_format->blockSignals(false);
    ui->comboBox_Salt->blockSignals(false);
    ui->comboBox_thickness->blockSignals(false);
    ui->comboBox_type->blockSignals(false);

    DesChange();
}

void DialogNewMateriel::initMater(Materiel ma)
{
    mater = ma;
    ui->comboBox_format->setCurrentText(ma.ComponentFormat);
    ui->comboBox_friction->setCurrentText(ma.Friction);
    ui->comboBox_Salt->setCurrentText(ma.Salt);
    ui->comboBox_solid->setCurrentText(ma.ComponentSolid);
    ui->comboBox_thickness->setCurrentText(ma.Thickness);
    ui->comboBox_type->setCurrentText(ma.Plating);
    ui->textEdit->setText(ma.MaterDes);
}

Materiel DialogNewMateriel::getMater() const
{
    return mater;
}


void DialogNewMateriel::on_pushButton_clear_clicked()
{
    ui->textEdit->clear();
}

void DialogNewMateriel::DesChange()
{
    QString str ;
    if(!ui->comboBox_solid->currentText().trimmed().isEmpty())
        str+="/"+ui->comboBox_solid->currentText();
    if(!ui->comboBox_format->currentText().trimmed().isEmpty())
        str+="/"+ui->comboBox_format->currentText();
    if(!ui->comboBox_type->currentText().trimmed().isEmpty())
        str+= "/"+ui->comboBox_type->currentText();
    if(!ui->comboBox_thickness->currentText().trimmed().isEmpty())
        str+= "/"+ui->comboBox_thickness->currentText();
    if(!ui->comboBox_Salt->currentText().trimmed().isEmpty())
        str+= "/"+ui->comboBox_Salt->currentText();
    if(!ui->comboBox_friction->currentText().trimmed().isEmpty())
        str+= "/"+ui->comboBox_friction->currentText();

    ui->textEdit->setText(str);
}


void DialogNewMateriel::on_comboBox_solid_currentTextChanged(const QString &arg1)
{
    if (!arg1.isEmpty()){
        ui->comboBox_friction->blockSignals(true);
        ui->comboBox_format->blockSignals(true);
        ui->comboBox_Salt->blockSignals(true);
        ui->comboBox_thickness->blockSignals(true);
        ui->comboBox_type->blockSignals(true);
        for(Materiel ma:mater_list){
            if(ma.ComponentSolid == arg1){
                ui->comboBox_friction->setCurrentText(ma.Friction);
                ui->comboBox_format->setCurrentText(ma.ComponentFormat);
                ui->comboBox_Salt->setCurrentText(ma.Salt);
                ui->comboBox_thickness->setCurrentText(ma.Thickness);
                ui->comboBox_type->setCurrentText(ma.Plating);
                break;
            }
        }
        ui->comboBox_friction->blockSignals(false);
        ui->comboBox_format->blockSignals(false);
        ui->comboBox_Salt->blockSignals(false);
        ui->comboBox_thickness->blockSignals(false);
        ui->comboBox_type->blockSignals(false);
    }
    DesChange();
}

void DialogNewMateriel::on_comboBox_format_currentTextChanged(const QString &arg1)
{
    if (!arg1.isEmpty()){
        ui->comboBox_friction->blockSignals(true);
        ui->comboBox_solid->blockSignals(true);
        ui->comboBox_Salt->blockSignals(true);
        ui->comboBox_thickness->blockSignals(true);
        ui->comboBox_type->blockSignals(true);
        for(Materiel ma:mater_list){
            if(ma.ComponentSolid == arg1){
                ui->comboBox_friction->setCurrentText(ma.Friction);
                ui->comboBox_solid->setCurrentText(ma.ComponentSolid);
                ui->comboBox_Salt->setCurrentText(ma.Salt);
                ui->comboBox_thickness->setCurrentText(ma.Thickness);
                ui->comboBox_type->setCurrentText(ma.Plating);
                break;
            }
        }
        ui->comboBox_friction->blockSignals(false);
        ui->comboBox_solid->blockSignals(false);
        ui->comboBox_Salt->blockSignals(false);
        ui->comboBox_thickness->blockSignals(false);
        ui->comboBox_type->blockSignals(false);
    }
    DesChange();
}

