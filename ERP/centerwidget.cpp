#include "centerwidget.h"
#include "ui_centerwidget.h"

CenterWidget::CenterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CenterWidget)
{
    ui->setupUi(this);
}

CenterWidget::~CenterWidget()
{
    delete ui;
}
