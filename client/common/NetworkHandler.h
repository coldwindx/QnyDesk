#ifndef NETWORK_HANDLER_H_
#define NETWORK_HANDLER_H_
#include <QObject>
#include <QTcpSocket>
#include "DeviceInfo.h"

class NetworkHandler : public QObject
{
    Q_OBJECT
public:
    enum TransferType
    {
        PASSIVE, ACTIVE
    };
    explicit NetworkHandler(QObject * parent = nullptr) : QObject(parent)
    {
        this->type = PASSIVE;
    }
    ~NetworkHandler()
    {

    }

    void createSocket()
    {
        QString remoteHost = "localhost";
        quint16 remotePort = 443;
        if(deviceInfo != nullptr)
        {
            remoteHost = deviceInfo->getRemoteHost();
            remotePort = deviceInfo->getRemotePort();
        }
        socket = new QTcpSocket(this);
        connect(socket, &QTcpSocket::connected, this, &NetworkHandler::handlerConnected);
        // connect
        socket->connectToHost("www.baidu.com", 80);
    }

    void handlerConnected()
    {
        qDebug() << "connect success!";
    }

    void init(DeviceInfo * deviceInfo, TransferType type)
    {
        this->deviceInfo = deviceInfo;
        this->type = type;
    }

private:
    QTcpSocket * socket;
    DeviceInfo * deviceInfo;
    TransferType type;
};

#endif