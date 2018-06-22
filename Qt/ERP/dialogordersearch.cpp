#include "dialogordersearch.h"
#include "ui_dialogordersearch.h"
#include <QDateTime>
#include <QToolTip>
DialogOrderSearch::DialogOrderSearch(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogOrderSearch)
{
    ui->setupUi(this);
    connect(ui->radioButton_other,SIGNAL(clicked(bool)),this,SLOT(changeUIEnable()));
    connect(ui->radioButton_time,SIGNAL(clicked(bool)),this,SLOT(changeUIEnable()));
    connect(ui->radioButton_price,SIGNAL(clicked(bool)),this,SLOT(changeUIEnable()));
    connect(ui->comboBox_type,SIGNAL(currentIndexChanged(QString)),this,SLOT(changeContent(QString)));

    ui->dateTimeEdit_start->setDateTime(QDateTime::currentDateTime().addDays(-1));
    ui->dateTimeEdit_end->setDateTime(QDateTime::currentDateTime());
    changeUIEnable();
}

DialogOrderSearch::~DialogOrderSearch()
{
    delete ui;
}

void DialogOrderSearch::initSearchContent(QMap<QString, QVector<QString> > data)
{
    m_search = data;
    QList<QString> list=  data.keys();
    ui->comboBox_type->blockSignals(true);
    for(QString s:list){
        ui->comboBox_type->addItem(s);
    }
    ui->comboBox_type->blockSignals(false);
}

void DialogOrderSearch::on_pushButton_ok_clicked()
{
    if(ui->radioButton_other->isChecked()){
        QString  type = ui->comboBox_type->currentText();
        QString  content = ui->comboBox_content->currentText();
        if(type==""||content==""){
            QToolTip::showText(ui->comboBox_content->mapToGlobal(QPoint(100, 0)), "搜索内容不能为空!");
            return;
        }
        emit searchOther(type,content);
    }
    if(ui->radioButton_price->isChecked()){
        double min = ui->doubleSpinBox_min->value();
        double max = ui->doubleSpinBox_max->value();
        if(min>max){
            QToolTip::showText(ui->doubleSpinBox_max->mapToGlobal(QPoint(100, 0)), "最大金额不能小于最小金额!");
            return;
        }
        emit searchPrice(min,max);
    }
    if(ui->radioButton_time->isChecked()){
        qint64 start = ui->dateTimeEdit_start->dateTime().toMSecsSinceEpoch();
        qint64 end = ui->dateTimeEdit_end->dateTime().toMSecsSinceEpoch();
        if(end<start){
            QToolTip::showText(ui->dateTimeEdit_end->mapToGlobal(QPoint(100, 0)), "结束时间不能小于开始时间!");
            return;
        }
        emit searchTime(start,end);
    }

    done(123);
}

void DialogOrderSearch::on_pushButton_exit_clicked()
{
    done(-1);
}

void DialogOrderSearch::changeUIEnable()
{
    if(ui->radioButton_time->isChecked()){
        setenable(true,false,false);
    }
    if(ui->radioButton_other->isChecked()){
        setenable(false,false,true);
    }
    if(ui->radioButton_price->isChecked()){
        setenable(false,true,false);
    }
}

void DialogOrderSearch::changeContent(QString type)
{
    ui->comboBox_content->clear();
    if(m_search.contains(type)){
        QVector<QString> data = m_search[type];
        ui->comboBox_content->addItems(data.toList());
    }
}

void DialogOrderSearch::setenable(bool time, bool price, bool other)
{
    ui->comboBox_content->setEnabled(other);
    ui->comboBox_type->setEnabled(other);
    ui->dateTimeEdit_start->setEnabled(time);
    ui->dateTimeEdit_end->setEnabled(time);
    ui->doubleSpinBox_min->setEnabled(price);
    ui->doubleSpinBox_max->setEnabled(price);
}



void DialogOrderSearch::on_pushButton_showall_clicked()
{
    emit showAll();
    done(123);
}
