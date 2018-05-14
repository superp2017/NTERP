#include "mainwindow.h"
#include <QApplication>
#include "login.h"
#include <QMainWindow>
#include <QDockWidget>
#include "service_global.h"

#include <QMetaType>
#include <QAbstractSocket>
#pragma execution_character_set("utf-8")

/*! 多线程信号，需要对自定义的类型进行注册类型
 * \brief registerMetaType
 */
void registerMetaType(){
    qRegisterMetaType<QAbstractSocket::SocketState>("QAbstractSocket::SocketStater");
    qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");
    qRegisterMetaType<Order>("Order");
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    registerMetaType();
    MainWindow w;
    w.setMinimumSize(800,800);


    //    NLogin login;
    //    login.showMaximized();
    //    if (login.exec()==123){
    w.showMaximized();
    //    }


    return a.exec();
}
