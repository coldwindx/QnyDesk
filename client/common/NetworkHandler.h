#ifndef NETWORK_HANDLER_H_
#define NETWORK_HANDLER_H_
#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include "DeviceInfo.h"

class NetworkHandler : public QObject
{
    Q_OBJECT
public:
    enum TransferType
    {
        PASSIVE, ACTIVE
    };
    explicit NetworkHandler(QObject * parent = nullptr);
    ~NetworkHandler();

    void createSocket();

    void removeSocket();

    void afterConnect();

    void init(DeviceInfo * deviceInfo, TransferType type);

    void afterStateChange(QAbstractSocket::SocketState socketState);
    
    void reconnect();

    void read();                    // 从网络套接字读取数据
signals:
    void connectStateChanged(bool flag);
    void finished();
private:
    QTcpSocket * socket;
    QString remoteHost = "localhost";   
    quint16 remotePort = 443;
    TransferType type;
    QTimer * timer;                     // 重连定时器
};

#endif