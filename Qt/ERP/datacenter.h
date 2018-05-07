#ifndef DATACENTER_H
#define DATACENTER_H

#include <QObject>

#include "global"

class dataCenter : public QObject
{
    Q_OBJECT
private:
    explicit dataCenter(QObject *parent = 0);

public:
   static dataCenter* instance(){
       static dataCenter*u =new dataCenter();
       return u;
    }
    void setCurUser(User* u);
    User* CurUser();
    QVector<Order*> getStatusOrder(QString status);
    Order *getOrder(QString OrderID);
    void appendOrder(Order *order);
signals:

public slots:


private:
    User* cur_user;
    QVector<Order*> m_orders;

};

#endif // DATACENTER_H
