#ifndef ORDERMANAGER_H
#define ORDERMANAGER_H

#include <QWidget>

namespace Ui {
class OrderManager;
}

class OrderManager : public QWidget
{
    Q_OBJECT

public:
    explicit OrderManager(QWidget *parent = 0);
    ~OrderManager();

private:
    Ui::OrderManager *ui;
};

#endif // ORDERMANAGER_H
