#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H
#include <QString>
#include <QNetworkConfigurationManager>
#include <QHostInfo>

/*! 该类主要获取本地网络管理类
 * \brief The NetworkManager class
 */
class NetworkManager : public QObject
{
    Q_OBJECT
public:
    NetworkManager(QObject*w=0);
    /*! 是否有可以用网络链接
     * \brief NetIsOnline
     * \return
     */
    bool NetIsOnline();
    /*! 是否能够链接到某个主机域名
     * \brief checkNetWork
     * \param hostName
     */
    void checkNetWork(QString hostName);
signals:
    void sigLookUpHostResult(QString hostName,bool ok);
private slots:
    void onLookupHost(QHostInfo host);
private:
    QNetworkConfigurationManager mgr;
};

#endif // NETWORKMANAGER_H
