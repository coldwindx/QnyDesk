#ifndef DEVICE_INFO_H_
#define DEVICE_INFO_H_

#include <QObject>

class DeviceInfo : public QObject
{
    Q_OBJECT
public:
    // explicit DeviceInfo(QObject * parent = nullptr) : QObject(parent) {}
    explicit DeviceInfo(const QString & host, quint16 port, QObject * parent = nullptr) : QObject(parent) 
    {
        this->host = host;
        this->port = port;
    }

    ~DeviceInfo() {}
    
    QString getHost()
    {
        return host;
    }

    quint16 getPort()
    {
        return port;
    } 
private:
    QString host;
    quint16 port;
};

#endif