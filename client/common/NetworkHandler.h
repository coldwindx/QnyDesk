#ifndef NETWORK_HANDLER_H_
#define NETWORK_HANDLER_H_
#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QCryptographicHash>
#include "protocol/Exchange.pb.h"
#include "DeviceInfo.h"

class NetworkHandler : public QObject
{
    Q_OBJECT
public:
    enum TransferType
    {
        PASSIVE, ACTIVE
    };

    explicit NetworkHandler(DeviceInfo * device, TransferType type = ACTIVE, QObject * parent = nullptr);
    ~NetworkHandler();

    void createSocket();

    void removeSocket();

    void connectedEvent();

    void stateChangedEvent(QAbstractSocket::SocketState socketState);
    
    void reconnect();

    void readEvent();                                               // 从网络套接字读取数据

    void errorEvent(QAbstractSocket::SocketError socketError);      // 错误消息处理
signals:
    void connectStateChanged(bool flag);
    void finished();
private:
    QTcpSocket * socket;
    TransferType type;
    DeviceInfo * device;
    QTimer * timer;                         // 重连定时器
};

#endif
