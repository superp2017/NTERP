#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTabWidget>
#include "datacenter.h"
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QApplication>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setCentralWidget(&m_center);

    setWindowFlags(Qt::Window|\
                   Qt::FramelessWindowHint |\
                   Qt::WindowSystemMenuHint|\
                   Qt::WindowMinimizeButtonHint|\
                   Qt::WindowMaximizeButtonHint);
    connect(&m_center,SIGNAL(show_mini()),this,SLOT(showMini()));
    connect(&m_center,SIGNAL(exitApp()),this,SLOT(on_exitAppAction()));

    mSysTrayIcon = NULL;
    mMenu= NULL;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    if(mSysTrayIcon!=NULL)
        mSysTrayIcon->hide();
    e->accept();
}

void MainWindow::newIcon()
{
    //新建QSystemTrayIcon对象
    mSysTrayIcon = new QSystemTrayIcon(this);
    //新建托盘要显示的icon
    QIcon icon = QIcon(":/icon/logo.png");
    //将icon设到QSystemTrayIcon对象中
    mSysTrayIcon->setIcon(icon);
    //当鼠标移动到托盘上的图标时，会显示此处设置的内容
    mSysTrayIcon->setToolTip(QObject::trUtf8("生产管理系统"));
    //给QSystemTrayIcon添加槽函数
    connect(mSysTrayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason)));

}

void MainWindow::newMenu()
{
    //建立托盘操作的菜单
    mShowMainAction = new QAction(QObject::trUtf8("显示主界面"),this);
    connect(mShowMainAction,SIGNAL(triggered()),this,SLOT(on_showMainAction()));
    mExitAppAction = new QAction(QObject::trUtf8("退出"),this);
    connect(mExitAppAction,SIGNAL(triggered()),this,SLOT(on_exitAppAction()));

    mMenu = new QMenu(this);
    mMenu->addAction(mShowMainAction);
    mMenu->addSeparator();
    mMenu->addAction(mExitAppAction);
    mSysTrayIcon->setContextMenu(mMenu);
}


void MainWindow::showMini()
{
    //隐藏主窗口
    this->hide();
    //在系统托盘显示此对象
    if(mSysTrayIcon==NULL){
        newIcon();
    }
    if(mMenu == NULL){
        newMenu();
    }
    mSysTrayIcon->show();
}



void MainWindow::on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason){
    //    case QSystemTrayIcon::Trigger:
    //        break;
    case QSystemTrayIcon::DoubleClick:
        on_showMainAction();
        break;
    default:
        break;
    }
}


void MainWindow::on_showMainAction()
{
    if(this->isHidden()){
        this->showMaximized();
        QString title="";
        if(QDateTime::currentDateTime().time().hour()>=12){
            title = "下午好!";
        }else{
            title = "上午好!";
        }
        mSysTrayIcon->showMessage(title,
                                  QObject::trUtf8("欢迎使用本系统"),
                                  QSystemTrayIcon::Information,
                                  1000);

    }
}

void MainWindow::on_exitAppAction()
{
    if(mSysTrayIcon!=NULL)
        mSysTrayIcon->hide();
    exit(0);
}







