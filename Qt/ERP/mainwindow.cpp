#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTabWidget>
#include "datacenter.h"
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

    QAction * neworder=  order->addAction("新订单");
    QAction * out=  order->addAction("出单");
    QAction * rk=  store->addAction("入库");
    QAction * ck=  store->addAction("出库");
    QAction * newemplyee=  person->addAction("新成员");
    QAction * gys=  dbs->addAction("供应商管理");
    QAction * kh=  dbs->addAction("客户管理");
    QAction * gsjj=  about->addAction("公司简介");

    QPalette pal(palette());
    pal.setBrush(QPalette::Window, QBrush(QImage(":/icon/home.png")));
    setPalette(pal);

    setCentralWidget(&m_center);
    connect(dataCenter::instance(),SIGNAL(showStatusMessage(QString,int)),this,SLOT(showMessage(QString,int)));
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
