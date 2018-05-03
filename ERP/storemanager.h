#ifndef STOREMANAGER_H
#define STOREMANAGER_H

#include <QWidget>

namespace Ui {
class StoreManager;
}

class StoreManager : public QWidget
{
    Q_OBJECT

public:
    explicit StoreManager(QWidget *parent = 0);
    ~StoreManager();

private:
    Ui::StoreManager *ui;
};

#endif // STOREMANAGER_H
