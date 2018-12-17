#include "dialognewmateriel.h"
#include "ui_dialognewmateriel.h"
#include <QDateTime>
#include <QCompleter>
#include "datacenter.h"
#include "boost/thread.hpp"
#include "materialservice.h"
#include <QDebug>
#include <QVector>
#include <QToolTip>

DialogNewMateriel::DialogNewMateriel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewMateriel)
{
    ui->setupUi(this);

    connect(dataCenter::instance(),SIGNAL(sig_newMaterial(Materiel,bool)),this,SLOT(newMaterCb(Materiel,bool)));
    connect(dataCenter::instance(),SIGNAL(sig_modMaterial(Materiel,bool)),this,SLOT(modMaterCb(Materiel,bool)));

    initCommbox();
    connect(ui->comboBox_friction,SIGNAL(currentTextChanged(QString)),this,SLOT(DesChange()));
    connect(ui->comboBox_thickness,SIGNAL(currentTextChanged(QString)),this,SLOT(DesChange()));
    connect(ui->comboBox_Salt,SIGNAL(currentTextChanged(QString)),this,SLOT(DesChange()));
    connect(ui->comboBox_type,SIGNAL(currentTextChanged(QString)),this,SLOT(DesChange()));
    connect(ui->comboBox_factoury,SIGNAL(currentIndexChanged(int)),this,SLOT(factoryChange(int)));
    factoryChange(0);
    DesChange();
}

DialogNewMateriel::~DialogNewMateriel()
{
    delete ui;
}

void DialogNewMateriel::initMater(Materiel ma)
{
    mater = ma;
    ui->comboBox_customer->setCurrentText(ma.CustomName);
    ui->comboBox_format->setCurrentText(ma.ComponentFormat);
    ui->comboBox_friction->setCurrentText(ma.Friction);
    ui->comboBox_Salt->setCurrentText(ma.Salt);
    ui->comboBox_solid->setCurrentText(ma.ComponentSolid);
    ui->comboBox_thickness->setCurrentText(ma.Thickness);
    ui->comboBox_type->setCurrentText(ma.Plating);
    ui->textEdit->setText(ma.MaterDes);
    ui->doubleSpinBox_money->setValue(ma.Money);
    ui->comboBox_unit->setCurrentText(mater.Unit);
    if(m_Model==2){
        this->setWindowTitle(ma.MaterID);
    }
}

void DialogNewMateriel::setModel(int  model_index)
{
    m_Model = model_index;
    if (m_Model==0) {
        ui->pushButton_ok->setText("新建");
        this->setWindowTitle("新建");
    }else if(m_Model==1){
        ui->pushButton_ok->setText("修改");
        this->setWindowTitle("修改");
    }else{
        ui->pushButton_ok->hide();
        this->setWindowTitle("物料详情");
        ui->textEdit->setEnabled(false);
        ui->comboBox_format->setEnabled(false);
        ui->comboBox_solid->setEnabled(false);
        ui->comboBox_friction->setEnabled(false);
        ui->comboBox_type->setEnabled(false);
        ui->comboBox_Salt->setEnabled(false);
        ui->comboBox_thickness->setEnabled(false);
        ui->comboBox_customer->setEnabled(false);
        ui->comboBox_customer->setEnabled(false);
        ui->comboBox_factoury->setEnabled(false);
        ui->comboBox_factoury->setEnabled(false);
        ui->comboBox_product_line->setEnabled(false);
        ui->doubleSpinBox_money->setEnabled(false);
        ui->pushButton_clear->setEnabled(false);
        ui->comboBox_unit->setEnabled(false);
    }
}

void DialogNewMateriel::on_pushButton_ok_clicked()
{
    mater.MaterDes          = ui->textEdit->toPlainText();
    mater.ComponentFormat   = ui->comboBox_format->currentText();
    mater.ComponentSolid    = ui->comboBox_solid->currentText();
    mater.Friction          = ui->comboBox_friction->currentText();
    mater.Plating           = ui->comboBox_type->currentText();
    mater.Salt              = ui->comboBox_Salt->currentText();
    mater.Thickness         = ui->comboBox_thickness->currentText();
    mater.CID               = ui->comboBox_customer->currentData().toString();
    mater.CustomName        = ui->comboBox_customer->currentText();
    mater.Factory           = ui->comboBox_factoury->currentText();
    mater.FactoryNumber     = ui->comboBox_factoury->currentData().toString();
    mater.ProductionLine    = ui->comboBox_product_line->currentText();
    mater.Money             = ui->doubleSpinBox_money->value();
    mater.Unit              = ui->comboBox_unit->currentText();

    if(mater.CID==""||mater.CustomName==""){
        QToolTip::showText(ui->comboBox_customer->mapToGlobal(QPoint(100, 0)), "客户姓名不能为空!");
        return;
    }
    if(mater.MaterDes==""){
        QToolTip::showText(ui->textEdit->mapToGlobal(QPoint(100, 0)), "物料描述不能为空!");
        return;
    }
    if(mater.Unit==""){
        QToolTip::showText(ui->comboBox_unit->mapToGlobal(QPoint(100, 0)), "计量单位不能为空!");
        return;
    }

    if(m_Model==0&&dataCenter::instance()->pub_checkComponentSolid(mater.ComponentSolid)){
        QToolTip::showText(ui->comboBox_solid->mapToGlobal(QPoint(100, 0)), "零件固号已经存在!");
        return;
    }


    QJsonObject para        = MaterialService::toJsonObject(mater);
    if(m_Model==0){
        boost::thread(boost::bind(&dataCenter::net_newMaterial,dataCenter::instance(),para));
    }else if(m_Model==1){
        boost::thread(boost::bind(&dataCenter::net_modMaterial,dataCenter::instance(),para));
    }else{
        return;
    }
    dataCenter::instance()->pub_showLoadding("正在网络请求...",5000,Qt::black);
}

void DialogNewMateriel::on_pushButton_cancle_clicked()
{
    done(-1);
}


void DialogNewMateriel::initCommbox()
{
    ui->comboBox_factoury->blockSignals(true);
    ui->comboBox_customer->blockSignals(true);
    ui->comboBox_solid->blockSignals(true);
    ui->comboBox_friction->blockSignals(true);
    ui->comboBox_format->blockSignals(true);
    ui->comboBox_Salt->blockSignals(true);
    ui->comboBox_thickness->blockSignals(true);
    ui->comboBox_type->blockSignals(true);

    ui->comboBox_factoury->addItem("涂覆分厂","01");
    ui->comboBox_factoury->addItem("滚镀分厂","02");
    ui->comboBox_factoury->addItem("挂镀分厂","03");

    for(Customer cus:dataCenter::instance()->pub_Customers()){
        ui->comboBox_customer->addItem(cus.Name,cus.CID);
    }


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
//    ui->comboBox_type->setEditable(true);
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


    QVector<QString >unit = dataCenter::instance()->pub_Units();
    ui->comboBox_unit->clear();
    ui->comboBox_unit->setEditable(true);
    ui->comboBox_unit->addItems(unit.toList());
    QCompleter *unit_com= new QCompleter(unit.toList(), this);
    ui->comboBox_unit->setCompleter(unit_com);

    ui->comboBox_solid->blockSignals(false);
    ui->comboBox_friction->blockSignals(false);
    ui->comboBox_format->blockSignals(false);
    ui->comboBox_Salt->blockSignals(false);
    ui->comboBox_thickness->blockSignals(false);
    ui->comboBox_type->blockSignals(false);
    ui->comboBox_customer->blockSignals(false);
    ui->comboBox_factoury->blockSignals(false);
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

void DialogNewMateriel::newMaterCb(Materiel ma, bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        dataCenter::instance()->pub_showMessage("创建成功!",4000);
        mater = ma;
        done(123);
    }else{
        dataCenter::instance()->pub_showMessage("创建失败!",4000);
        mater = ma;
        done(0);
    }
}

void DialogNewMateriel::modMaterCb(Materiel ma, bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        dataCenter::instance()->pub_showMessage("修改成功!",4000);
        mater = ma;
        done(123);
    }else{
        dataCenter::instance()->pub_showMessage("修改失败!",4000);
        mater = ma;
        done(0);
    }
}

void DialogNewMateriel::factoryChange(int index)
{
    QString fac = ui->comboBox_factoury->currentData().toString();

    if(fac=="01"){
        ui->comboBox_product_line->clear();
        ui->comboBox_product_line->addItem("涂覆线");
    }
    if(fac=="02"){
        ui->comboBox_product_line->clear();
        ui->comboBox_product_line->addItem("滚镀锌线");
        ui->comboBox_product_line->addItem("滚镀锌镍线");
        ui->comboBox_product_line->addItem("磷化线");
        ui->comboBox_product_line->addItem("镀铜镀锡线");
    }
    if(fac=="03"){
        ui->comboBox_product_line->clear();
        ui->comboBox_product_line->addItem("挂镀锌线");
        ui->comboBox_product_line->addItem("挂镀锌镍线");
    }
}

