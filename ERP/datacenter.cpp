#include "datacenter.h"

dataCenter::dataCenter(QObject *parent) : QObject(parent)
{

}

void dataCenter::setCurUser(User *u)
{
    cur_user = u;
}

User* dataCenter::CurUser()
{
   return cur_user;
}
