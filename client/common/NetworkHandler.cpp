#include "NetworkHandler.h"

NetworkHandler::NetworkHandler(QObject * parent) : QObject(parent)
{
    this->type = ACTIVE;
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
    connect(socket, &QTcpSocket::connected, this, &NetworkHandler::afterConnect);
    connect(socket, &QTcpSocket::stateChanged, this, &NetworkHandler::afterStateChange);
    // connect
    socket->abort();
    socket->connectToHost(remoteHost, remotePort);
}

void NetworkHandler::afterConnect()
{
    qDebug() << "connect success!";
}

void NetworkHandler::init(DeviceInfo * deviceInfo, TransferType type)
{
    if(deviceInfo != nullptr)
    {
        remoteHost = deviceInfo->getRemoteHost();
        remotePort = deviceInfo->getRemotePort();
    }
    this->type = type;
}

void NetworkHandler::afterStateChange(QAbstractSocket::SocketState socketState)
{
    // 一般服务器没有开转这个状态
    if(QAbstractSocket::UnconnectedState == socketState)
    {
        qDebug() << "Fail to connect remote server[" << remoteHost << ":" << remotePort << "]!";
        // 发送连接失败信号
        emit connectStateChanged(false);
        // 启动重连定时器
        if(!timer->isActive())
            timer->start(5000);
        return ;
    }
    // 一般服务器开启连接端口号没有开转这个状态
    if(QAbstractSocket::ConnectingState == socketState)
    {
        qDebug() << "Try to connect remote server[" << remoteHost << ":" << remotePort << "]!";
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
}

    
void NetworkHandler::reconnect()
{
    if(socket->state() == QAbstractSocket::ConnectedState)
    {
        qDebug() << "Success to connect remote server!";
        qDebug() << "Stop to reconnect!";
        return timer->stop();
    }
    if(socket->state() == QAbstractSocket::ConnectingState)
    {
        socket->abort();
        return socket->connectToHost(remoteHost, remotePort);
    }
}
