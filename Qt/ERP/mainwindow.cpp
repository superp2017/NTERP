#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTabWidget>
#include "datacenter.h"
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QMenu * order  = ui->menuBar->addMenu("订单管理");
    QMenu * store  = ui->menuBar->addMenu("仓库管理");
    QMenu * person = ui->menuBar->addMenu("人事管理");
    QMenu * dbs    = ui->menuBar->addMenu("资料库");
    QMenu * about  = ui->menuBar->addMenu("关于公司");

    QAction * neworder      =  order->addAction("新订单");
    QAction * storeManage   =  store->addAction("库存管理");
    QAction * newemplyee    =  person->addAction("新员工");
    QAction * matermanage   =  dbs->addAction("物料管理");
    QAction * gys           =  dbs->addAction("供应商管理");
    QAction * custommanage  =  dbs->addAction("客户管理");
    QAction * unit          =  dbs->addAction("单位管理");
    QAction * gsjj          =  about->addAction("公司简介");
    QAction *aboutsys   =  about->addAction("关于系统");

    QPalette pal(palette());
    pal.setBrush(QPalette::Window, QBrush(QImage(":/icon/home.png")));
    setPalette(pal);

    setCentralWidget(&m_center);
    connect(dataCenter::instance(),SIGNAL(showStatusMessage(QString,int)),this,SLOT(showMessage(QString,int)));

    connect(neworder,     SIGNAL(triggered(bool)), &m_center, SIGNAL(action_new_order()));
    connect(storeManage,  SIGNAL(triggered(bool)), &m_center, SLOT(action_store_manage()));
    connect(newemplyee,   SIGNAL(triggered(bool)), &m_center, SIGNAL(action_new_user()));
    connect(matermanage,  SIGNAL(triggered(bool)), &m_center, SLOT(action_material_manage()));
    connect(gys,          SIGNAL(triggered(bool)), &m_center, SLOT(action_supplier_manage()));
    connect(custommanage, SIGNAL(triggered(bool)), &m_center, SLOT(action_customs_manage()));
    connect(unit,         SIGNAL(triggered(bool)), &m_center, SLOT(action_unit_manage()));
    connect(gsjj,         SIGNAL(triggered(bool)), this,      SLOT(aboutCommany()));
    connect(aboutsys,     SIGNAL(triggered(bool)), this,      SLOT(abountSystem()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showMessage(QString msg, int delay)
{
    statusBar()->setStyleSheet("color:white");
    ui->statusBar->showMessage(msg,delay);
}

void MainWindow::aboutCommany()
{
    QString qtManulFile="CompanyIntroduction.pdf";
    QDesktopServices::openUrl(QUrl::fromLocalFile(qtManulFile));


}

void MainWindow::abountSystem()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("关于系统");
    msgBox.setText("本系统由mp开发\n系统版本 V1.0\n公司网站:www.baidu.com");
    msgBox.exec();
}
