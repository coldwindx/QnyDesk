#ifndef NETWORK_HANDLER_H_
#define NETWORK_HANDLER_H_
#include <QObject>
#include <QTcpSocket>
#include <QTimer>

class NetworkHandler : public QObject
{
    Q_OBJECT
public:
    enum TransferType
    {
        PASSIVE, ACTIVE
    };

    explicit NetworkHandler(const QString & host, quint16 port, TransferType type = ACTIVE, QObject * parent = nullptr);
    ~NetworkHandler();

    void createSocket();

    void removeSocket();

    void connectedEvent();

    void stateChangedEvent(QAbstractSocket::SocketState socketState);
    
    void reconnect();

    void readEvent();                               // 从网络套接字读取数据

    void errorEvent(QAbstractSocket::SocketError socketError);       // 错误消息处理
signals:
    void connectStateChanged(bool flag);
    void finished();
private:
    QTcpSocket * socket;
    QString host;   
    quint16 port;
    TransferType type;
    QTimer * timer;                     // 重连定时器
};

#endif