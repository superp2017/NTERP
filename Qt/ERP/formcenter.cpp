#include "formcenter.h"
#include "ui_formcenter.h"
#include <QMessageBox>
#include <QDockWidget>
FormCenter::FormCenter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormCenter)
{
    ui->setupUi(this);
    ui->exit->setStyleSheet("QPushButton{border-image: url(:/icon/SignoutFront.png);}"
                            "QPushButton:hover{border-image: url(:/icon/SignInoutpng.png);}"
                            "QPushButton:pressed{border-image: url(:/icon/SignoutAFter.png);}");

    ui->order_btn->setStyleSheet("QToolButton{border-image: url(:/icon/monitorFront.png);}"
                                 "QToolButton:hover{border-image: url(:/icon/monitorIn.png);}"
                                 "QToolButton:pressed{border-image: url(:/icon/monitorAfter.png);}"
                                 "QToolButton:checked{border-image: url(:/icon/monitorAfter.png);}");
    ui->person_btn->setStyleSheet("QToolButton{border-image: url(:/icon/valverFront.png);}"
                                  "QToolButton:hover{border-image: url(:/icon/valverIn.png);}"
                                  "QToolButton:pressed{border-image: url(:/icon/valverAfter.png);}"
                                  "QToolButton:checked{border-image: url(:/icon/valverAfter.png);}");
    ui->store_btn->setStyleSheet("QToolButton{border-image: url(:/icon/planFront.png);}"
                                 "QToolButton:hover{border-image: url(:/icon/planIn.png);}"
                                 "QToolButton:pressed{border-image: url(:/icon/planAfter.png);}"
                                 "QToolButton:checked{border-image: url(:/icon/planAfter.png);}");
    ui->set_Btn->setStyleSheet("QToolButton{border-image: url(:/icon/deviceFront.png);}"
                               "QToolButton:hover{border-image: url(:/icon/deciceIn.png);}"
                               "QToolButton:pressed{border-image: url(:/icon/deciceAfter.png);}"
                               "QToolButton:checked{border-image: url(:/icon/deciceAfter.png);}");
    ui->stackedWidget->addWidget(&m_order);
    ui->stackedWidget->addWidget(&m_store);
    ui->stackedWidget->addWidget(&m_person);
    ui->stackedWidget->addWidget(&m_sys);

    ui->order_btn->setChecked(true);
}

FormCenter::~FormCenter()
{
    delete ui;
}

void FormCenter::on_order_btn_clicked()
{
    ui->person_btn->setChecked(false);
    ui->store_btn->setChecked(false);
    ui->set_Btn->setChecked(false);
    ui->stackedWidget->setCurrentWidget(&m_order);

}

void FormCenter::on_store_btn_clicked()
{
    ui->order_btn->setChecked(false);
    ui->person_btn->setChecked(false);
    ui->set_Btn->setChecked(false);
    ui->stackedWidget->setCurrentWidget(&m_store);
}

void FormCenter::on_person_btn_clicked()
{
    ui->order_btn->setChecked(false);
    ui->store_btn->setChecked(false);
    ui->set_Btn->setChecked(false);
    ui->stackedWidget->setCurrentWidget(&m_person);
}

void FormCenter::on_set_Btn_clicked()
{
    ui->order_btn->setChecked(false);
    ui->store_btn->setChecked(false);
    ui->person_btn->setChecked(false);
    ui->stackedWidget->setCurrentWidget(&m_sys);
}

void FormCenter::on_exit_clicked()
{
    QMessageBox msg;
    msg.setWindowTitle("提示");
    msg.setText("系统即将退出");
    msg.setInformativeText("是否继续？");
    msg.setStandardButtons(QMessageBox::Ok| QMessageBox::Cancel);
    msg.setDefaultButton(QMessageBox::Ok);
    int ret = msg.exec();
    switch (ret) {
    case QMessageBox::Ok:
        QApplication* app;
        app->quit();
        break;
    case QMessageBox::Cancel:
        break;
    }
}

void FormCenter::clearChecked()
{
    ui->set_Btn->setChecked(false);
    ui->order_btn->setChecked(false);
    ui->store_btn->setChecked(false);
    ui->person_btn->setChecked(false);
}
