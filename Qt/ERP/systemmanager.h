#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include <QWidget>

namespace Ui {
class SystemManager;
}

class SystemManager : public QWidget
{
    Q_OBJECT

public:
    explicit SystemManager(QWidget *parent = 0);
    ~SystemManager();

private:
    Ui::SystemManager *ui;
};

#endif // SYSTEMMANAGER_H
