#ifndef NETWORK_HANDLER_H_
#define NETWORK_HANDLER_H_
#include <QObject>
#include <QTcpSocket>

class NetworkHandler : public QObject
{
    Q_OBJECT
private:
    QTcpSocket * socket;
public:
    explicit NetworkHandler(QObject * parent = nullptr) : QObject(parent)
    {

    }
    ~NetworkHandler()
    {

    }

    void createSocket()
    {
        socket = new QTcpSocket(this);
        connect(socket, &QTcpSocket::connected, this, &NetworkHandler::handlerConnected);
        // connect
        socket->connectToHost("www.baidu.com", 80);
    }

    void handlerConnected()
    {
        
    }
};

#endif