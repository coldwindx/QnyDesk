#include "NetworkHandler.h"

NetworkHandler::NetworkHandler(const QString & host, quint16 port, TransferType type, QObject * parent)
    : QObject(parent)
{
    this->host = host;
    this->port = port;
    this->type = type;
    // 重连定时器
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &NetworkHandler::reconnect);
}

NetworkHandler::~NetworkHandler()
{

}

void NetworkHandler::createSocket()
{
    socket = new QTcpSocket(this);
    // 当连接成功时，先stateChanged，后connected
    // connect(socket, &QTcpSocket::connected, this, &NetworkHandler::connectedEvent);
    // <1> 监听连接状态
    connect(socket, &QTcpSocket::stateChanged, this, &NetworkHandler::stateChangedEvent);
    // <2> 读取消息
    connect(socket, &QTcpSocket::readyRead, this, &NetworkHandler::readEvent);
    // <3> 错误信息处理
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errorEvent(QAbstractSocket::SocketError)));
    // connect
    socket->abort();
    socket->connectToHost(host, port);
}

void NetworkHandler::removeSocket()
{
    qDebug() << "Start to remove socket!";
    if(timer)
    {
        if(timer->isActive())
            timer->stop();
        timer->deleteLater();   // 不能delete timer，因为timer指定了Qt的父亲，可能会有问题
        timer = Q_NULLPTR;
    }
    if(socket)
    {
        if(QAbstractSocket::UnconnectedState != socket->state())
            socket->close(); 
        socket->disconnectFromHost();
        socket->deleteLater();
        socket = Q_NULLPTR;
    }
    qDebug() << "End to remove socket!";
    emit finished();
}

void NetworkHandler::connectedEvent()
{
    qDebug() << "connect success!";
}

void NetworkHandler::stateChangedEvent(QAbstractSocket::SocketState socketState)
{
    // 一般服务器没有开转这个状态
    if(QAbstractSocket::UnconnectedState == socketState)
    {
        qDebug() << "Fail to connect remote server[" << host << ":" << port << "]!";
        // 发送连接失败信号
        emit connectStateChanged(false);
        // 启动重连定时器
        // 程序退出时，这里会接收到信号，此时timer已经释放，需要提前判断timer
        if(timer && !timer->isActive())
            timer->start(5000);
        return ;
    }
    // 一般服务器开启连接端口号没有开转这个状态
    if(QAbstractSocket::ConnectingState == socketState)
    {
        qDebug() << "Try to connect remote server[" << host << ":" << port << "]!";
        // 启动重连定时器
        if(!timer->isActive())
            timer->start(5000);
        return ;
    }
    // 连接成功
    if(QAbstractSocket::ConnectedState == socketState)
    {
        emit connectStateChanged(true);
        return ;
    }
    // 连接关闭
    if(QAbstractSocket::ClosingState == socketState)
    {
        emit connectStateChanged(false);
        // 启动重连定时器
        if(!timer->isActive())
            timer->start(5000);
        return ;
    }
}

    
void NetworkHandler::reconnect()
{
    qDebug() << "Start to reconnect server!";
    if(socket->state() == QAbstractSocket::ConnectedState)
    {
        return timer->stop();
    }
    if(socket->state() == QAbstractSocket::ConnectingState)
    {
        socket->abort();
        return socket->connectToHost(host, port);
    }
    if(socket->state() == QAbstractSocket::UnconnectedState)
    {
        socket->abort();
        return socket->connectToHost(host, port);
    }
    qDebug() << "Unexpected socket state!";
}

void NetworkHandler::readEvent()
{
    qDebug() << "Start to read data from socket!";
    QByteArray array = socket->readAll();
    qDebug() << array;
    qDebug() << "Stop to read data!";
}

void NetworkHandler::errorEvent(QAbstractSocket::SocketError error)
{
    qDebug() << "Socket Error:" << socket->errorString();
}