#ifndef DEVICE_INFO_H_
#define DEVICE_INFO_H_

#include <QObject>

class DeviceInfo : public QObject
{
    Q_OBJECT
public:
    explicit DeviceInfo(QObject * parent = nullptr) : QObject(parent) {}
    ~DeviceInfo() {}

    void setRemoteInfo(const QString & host, quint16 port)
    {
        this->host = host;
        this->port = port;
    }

    QString getRemoteHost()
    {
        return host;
    }

    quint16 getRemotePort()
    {
        return port;
    } 
private:
    QString host;
    quint16 port;
};

#endif