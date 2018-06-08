#include "mainwindow.h"
#include <QApplication>
#include "login.h"
#include <QMainWindow>
#include "service_global.h"
#include <QMetaType>
#include <QSplashScreen>
#include <QDebug>
#include "datacenter.h"

#pragma execution_character_set("utf-8")

/*! 多线程信号，需要对自定义的类型进行注册类型
 * \brief registerMetaType
 */
void registerMetaType(){
    qRegisterMetaType<Order>("Order");
    qRegisterMetaType<User>("User");
    qRegisterMetaType<Customer>("Customer");
    qRegisterMetaType<Supplier>("Supplier");
    qRegisterMetaType<Goods>("Goods");
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    registerMetaType();

    // QPixmap pixmap(":/icon/SignoutFront.png");
    // QSplashScreen *splash = new QSplashScreen(pixmap);
    // splash->show();                            //显示启动画面
    //splash->showMessage("正在加载模块..");
    /// a.processEvents();                       //加载其他的模块

    dataCenter::instance()->initData();
    NLogin login;
    if (login.exec()==123){
        MainWindow w;
        //        w.setWindowFlags(Qt::FramelessWindowHint);
        //       w.setWindowFlags(Qt::FramelessWindowHint |
        //                             Qt::WindowSystemMenuHint |
        //                             Qt::WindowMinMaxButtonsHint);
        w.setMinimumSize(800,800);
        w.showMaximized();
        return a.exec();
    }

    // splash->finish(&w);     //当主窗口启动后，启动画面隐藏
    // delete splash;

    return 0;
}
