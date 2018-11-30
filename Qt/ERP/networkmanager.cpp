#include "networkmanager.h"

NetworkManager::NetworkManager(QObject *w):QObject(w)
{

}

bool NetworkManager::NetIsOnline()
{
    return mgr.isOnline();
}

void NetworkManager::checkNetWork(QString hostName)
{
    QHostInfo::lookupHost(hostName,this,SLOT(onLookupHost(QHostInfo)));
}

void NetworkManager::onLookupHost(QHostInfo host)
{

    if (host.error() != QHostInfo::NoError) {
        qDebug() <<host.hostName()<< "Lookup failed:" << host.errorString();
        //网络未连接，发送信号通知
        emit sigLookUpHostResult(host.hostName(),false);
    }
    else{
        emit sigLookUpHostResult(host.hostName(),true);
    }
}
