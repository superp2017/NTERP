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
    MainWindow w;
    w.setMinimumSize(800,800);


    //    NLogin login;
    //    login.showMaximized();
    //    if (login.exec()==123){
    w.showMaximized();
    //    }


    return a.exec();
}
