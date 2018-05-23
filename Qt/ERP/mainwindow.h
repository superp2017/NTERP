#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "formcenter.h"
namespace Ui {
class MainWindow;
}
#pragma execution_character_set("utf-8")
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void showMessage(QString msg,int delay);

private slots:
    void aboutCommany();
    void abountSystem();

private:
    Ui::MainWindow *ui;
    FormCenter  m_center;
};

#endif // MAINWINDOW_H
