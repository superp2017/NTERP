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
    qRegisterMetaType<Materiel>("Materiel");
    qRegisterMetaType<GoodsOutRecord>("GoodsOutRecord");


}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    registerMetaType();

    dataCenter::instance()->initData();
    NLogin login;
    login.showFullScreen();
    if (login.exec()==123){
        MainWindow w;
        w.setMinimumSize(800,800);
        w.showMaximized();
        return a.exec();
    }

    return 0;
}





