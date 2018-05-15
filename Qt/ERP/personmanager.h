#ifndef PERSONMANAGER_H
#define PERSONMANAGER_H

#include <QWidget>
#pragma execution_character_set("utf-8")


namespace Ui {
class PersonManager;
}

class PersonManager : public QWidget
{
    Q_OBJECT

public:
    explicit PersonManager(QWidget *parent = 0);
    ~PersonManager();

private:
    Ui::PersonManager *ui;
};

#endif // PERSONMANAGER_H
