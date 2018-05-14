#include "dialognewunit.h"
#include "ui_dialognewunit.h"
#include <QToolTip>
DialogNewUnit::DialogNewUnit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewUnit)
{
    ui->setupUi(this);
}

DialogNewUnit::~DialogNewUnit()
{
    delete ui;
}

void DialogNewUnit::on_pushButton__ok_clicked()
{
    cur_unit = ui->lineEdit->text().trimmed();
    if(cur_unit.isEmpty()){
        QToolTip::showText(ui->lineEdit->mapToGlobal(QPoint(100, 0)), "单位不能为空!");
        return ;
    }
    done(123);
}

void DialogNewUnit::on_pushButton_cancle_clicked()
{
    done(-1);
}

QString DialogNewUnit::getUnit() const
{
    return cur_unit;
}
