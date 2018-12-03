#include "notification.h"
#include <QHostAddress>
#include <QJsonObject>
#include <QDebug>
#include <QJsonDocument>
#include "http.h"

Notification::Notification(QObject *parent) : QObject(parent)
{
    m_recev =NULL;
}

void Notification::Listen(quint16 prot)
{
    if(m_recev!=NULL) return;
    m_recev = new QUdpSocket(this);
    m_recev->bind(QHostAddress::LocalHost, port);
    connect(m_recev, SIGNAL(readyRead()),this, SLOT(readPendingDatagrams()));
}

NoticeInfo Notification::NoticeformJson(QJsonObject obj)
{
    NoticeInfo info;
    if(obj.contains("DataType")){
        QJsonValue v = obj.value("DataType");
        if (v.isDouble()){
            info.DataType = v.toInt();
        }
    }
    if(obj.contains("NoticeType")){
        QJsonValue v = obj.value("NoticeType");
        if (v.isDouble()){
            info.NoticeType = v.toInt();
        }
    }
    if(obj.contains("Data")){
        info.Data = obj.value("Data");
    }
    return info;
}

void Notification::HeartBeat(const QJsonObject para, bool &ok, QString hostname, QString hostport)
{
    std::string url = Net_HeartBeat;
    Materiel c;
    bool r   = false;
    Ret ret  = Http::fetch(url,para,r,hostname,hostport);
    if(r&&ret.ret){
        ok =true;
        return;
    }
    if(!ret.ret)
        qDebug()<<"HeartBeat ret is not 0"<<endl;
    ok = false;
    return ;
}

void Notification::readPendingDatagrams()
{
    while(m_recev!=NULL&&m_recev->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(m_recev->pendingDatagramSize());
        m_recev->readDatagram(datagram.data(), datagram.size());
        qDebug()<<"readPendingDatagrams";
        QJsonDocument doc = QJsonDocument::fromJson(datagram);
        if(!doc.isNull()){
            emit newNOtice(doc.object());
            qDebug()<<"notifcation:"<<doc.object().toVariantMap();
        }
    }
}
