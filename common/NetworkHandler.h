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
        UNINIT, PASSIVE, ACTIVE
    };
    explicit NetworkHandler(QObject * parent = nullptr) : QObject(parent)
    {

    }
    ~NetworkHandler()
    {

    }

    void createSocket()
    {
        if(UNINIT == type)
        {
            qDebug() << "没有初始化！";
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
    TransferType type = UNINIT;
};

#endif