#include "notification.h"
#include <QHostAddress>
#include <QJsonObject>
#include <QDebug>
#include <QJsonDocument>
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

void Notification::readPendingDatagrams()
{
    while(m_recev!=NULL&&m_recev->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(m_recev->pendingDatagramSize());
        m_recev->readDatagram(datagram.data(), datagram.size());
        QJsonDocument doc = QJsonDocument::fromJson(datagram);
        if(!doc.isNull()){
            emit newNOtice(doc.object());
            qDebug()<<"notifcation:"<<doc.object().toVariantMap();
        }
    }
}
