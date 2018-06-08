#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "formcenter.h"
#include <QSystemTrayIcon>
#include <QCloseEvent>

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

protected:
    void closeEvent(QCloseEvent *e);

private slots:

    void on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason);
     void showMini();
    void on_showMainAction();
    void on_exitAppAction();
private:
    void newIcon();
    void newMenu();


    Ui::MainWindow *ui;
    FormCenter  m_center;

    QSystemTrayIcon *mSysTrayIcon;
    QMenu   *mMenu;
    QAction *mShowMainAction;
    QAction *mExitAppAction;
};

#endif // MAINWINDOW_H
