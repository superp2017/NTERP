#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include <QWidget>
#include "nsysconfig.h"

#pragma execution_character_set("utf-8")


namespace Ui {
class SystemManager;
}

class SystemManager : public QWidget
{
    Q_OBJECT

public:
    explicit SystemManager(QWidget *parent = 0);
    ~SystemManager();

private slots:
    void on_pushButton_update_clicked();
    void aboutCommany();
    void abountSystem();
private:
    void initSetting();
    Ui::SystemManager *ui;
    SysSetting set;
};

#endif // SYSTEMMANAGER_H
