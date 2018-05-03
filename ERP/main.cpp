#include "mainwindow.h"
#include <QApplication>
#include "login.h"
#pragma execution_character_set("utf-8")
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
//    NLogin login;
//    login.showMaximized();
//    if (login.exec()==123){
        w.showMaximized();
    //}
    return a.exec();
}
