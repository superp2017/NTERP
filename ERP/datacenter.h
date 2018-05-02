#ifndef DATACENTER_H
#define DATACENTER_H

#include <QObject>


struct User {
   std::string UID;
   std::string Name;
   std::string Author;
};

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
signals:

public slots:


private:
    User* cur_user;

};

#endif // DATACENTER_H
