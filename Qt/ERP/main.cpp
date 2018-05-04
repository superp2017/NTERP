#include "mainwindow.h"
#include <QApplication>
#include "login.h"
#include <QMainWindow>
#include <QDockWidget>
#pragma execution_character_set("utf-8")
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setMinimumSize(800,800);
//    NLogin login;
//    login.showMaximized();
//    if (login.exec()==123){
        w.showMaximized();
//    }
    return a.exec();
}
