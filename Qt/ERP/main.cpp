#include "mainwindow.h"
#include <QApplication>
#include "login.h"
#include <QMainWindow>
#include <QDockWidget>
#include "service_global.h"

#include <QMetaType>
#include <QAbstractSocket>
#include "datacenter.h"
#pragma execution_character_set("utf-8")
#include <QSplashScreen>


/*! 多线程信号，需要对自定义的类型进行注册类型
 * \brief registerMetaType
 */
void registerMetaType(){
    qRegisterMetaType<QAbstractSocket::SocketState>("QAbstractSocket::SocketStater");
    qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");
    qRegisterMetaType<Order>("Order");
    qRegisterMetaType<User>("User");
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    registerMetaType();
    dataCenter::instance();

   // QPixmap pixmap(":/icon/SignoutFront.png");
   // QSplashScreen *splash = new QSplashScreen(pixmap);
   // splash->show();                            //显示启动画面
    //splash->showMessage("正在加载模块..");
   /// a.processEvents();                       //加载其他的模块
    MainWindow w;
    w.setMinimumSize(800,800);

    //    NLogin login;
    //    login.showMaximized();
    //    if (login.exec()==123){
    w.showMaximized();
    //    }


   // splash->finish(&w);     //当主窗口启动后，启动画面隐藏
   // delete splash;


    return a.exec();
}
