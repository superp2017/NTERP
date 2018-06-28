#include "dialogordersearch.h"
#include "ui_dialogordersearch.h"
#include <QDateTime>
#include <QToolTip>
#include <QMessageBox>

DialogOrderSearch::DialogOrderSearch(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogOrderSearch)
{
    ui->setupUi(this);
    connect(ui->checkBox_other,SIGNAL(clicked(bool)),this,SLOT(changeUIEnable()));
    connect(ui->checkBox_time,SIGNAL(clicked(bool)),this,SLOT(changeUIEnable()));
    connect(ui->comboBox_type,SIGNAL(currentIndexChanged(QString)),this,SLOT(changeContent(QString)));

    ui->dateEdit_start->setDate(QDate::currentDate().addDays(-1));
    ui->dateEdit_end->setDate(QDate::currentDate());
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
    for(QString s:list){
        ui->comboBox_type->addItem(s);
    }
    ui->comboBox_type->setCurrentIndex(0);
}

void DialogOrderSearch::on_pushButton_ok_clicked()
{
    if(!ui->checkBox_other->isChecked()&&!ui->checkBox_time->isChecked()){
        QMessageBox::information(this,"提示","请至少选择一种搜索方式!");
        return;
    }
    QString  type,content;
    if(ui->checkBox_other->isChecked()){
          type = ui->comboBox_type->currentText();
          content = ui->comboBox_content->currentText();
        if(type==""||content==""){
            QToolTip::showText(ui->comboBox_content->mapToGlobal(QPoint(100, 0)), "搜索内容不能为空!");
            return;
        }
    }

     qint64 start,end;
    if(ui->checkBox_time->isChecked()){
         start = ui->dateEdit_start->dateTime().toMSecsSinceEpoch();
         end = ui->dateEdit_end->dateTime().toMSecsSinceEpoch();
        if(end<start){
            QToolTip::showText(ui->dateEdit_end->mapToGlobal(QPoint(100, 0)), "结束时间不能小于开始时间!");
            return;
        }
    }

    emit searchOrder(ui->checkBox_time->isChecked(),\
                     ui->checkBox_other->isChecked(),\
                     start,end,type,content);

    done(123);
}

void DialogOrderSearch::on_pushButton_exit_clicked()
{
    done(-1);
}

void DialogOrderSearch::changeUIEnable()
{
    setenable(ui->checkBox_time->isChecked(),ui->checkBox_other->isChecked());
}

void DialogOrderSearch::changeContent(QString type)
{
    ui->comboBox_content->clear();
    if(m_search.contains(type)){
        QVector<QString> data = m_search[type];
        ui->comboBox_content->addItems(data.toList());
    }
}

void DialogOrderSearch::setenable(bool time,  bool other)
{
    ui->comboBox_content->setEnabled(other);
    ui->comboBox_type->setEnabled(other);
    ui->dateEdit_start->setEnabled(time);
    ui->dateEdit_end->setEnabled(time);
}



void DialogOrderSearch::on_pushButton_showall_clicked()
{
    emit showAll();
    done(123);
}
