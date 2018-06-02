#include "formauthormanage.h"
#include "ui_formauthormanage.h"

FormAuthorManage::FormAuthorManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormAuthorManage)
{
    ui->setupUi(this);
}

FormAuthorManage::~FormAuthorManage()
{
    delete ui;
}
