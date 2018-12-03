#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QObject>
#include <QtNetwork>
#include "service_global.h"
class SERVICESHARED_EXPORT Notification : public QObject
{
    Q_OBJECT
public:
    explicit Notification(QObject *parent = 0);
    void Listen(quint16 prot);
    static NoticeInfo NoticeformJson(QJsonObject obj);
    static void HeartBeat(const QJsonObject para, bool &ok, QString hostname, QString hostport);
signals:
    void newNOtice(QJsonObject &obj);
public slots:
    void readPendingDatagrams();
private:
    QUdpSocket *m_recev;
    qint16 port;
};

#endif // NOTIFICATION_H
