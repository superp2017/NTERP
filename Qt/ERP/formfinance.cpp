#include "formfinance.h"
#include "ui_formfinance.h"

FormFinance::FormFinance(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormFinance)
{
    ui->setupUi(this);
    ui->pushButton_export->setStyleSheet("QPushButton{border-image: url(:/icon/export.png);}"
                                         "QPushButton:hover{border-image: url(:/icon/export-a.png);}"
                                         "QPushButton:pressed{border-image: url(:/icon/export-a.png);}"
                                         "QPushButton:checked{border-image: url(:/icon/export-a.png);}");

    ui->pushButton_search->setStyleSheet("QPushButton{border-image: url(:/icon/reflash.png);}"
                                          "QPushButton:hover{border-image: url(:/icon/reflash-a.png);}"
                                          "QPushButton:pressed{border-image: url(:/icon/reflash-a.png);}"
                                          "QPushButton:checked{border-image: url(:/icon/reflash-a.png);}");
}

FormFinance::~FormFinance()
{
    delete ui;
}

void FormFinance::on_pushButton_search_clicked()
{

}

void FormFinance::on_pushButton_export_clicked()
{

}
