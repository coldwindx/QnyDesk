#ifndef DEVICE_INFO_H_
#define DEVICE_INFO_H_

#include <QObject>

class DeviceInfo : public QObject
{
    Q_OBJECT
public:
    // explicit DeviceInfo(QObject * parent = nullptr) : QObject(parent) {}
    explicit DeviceInfo(const QString & host, quint16 port, QObject * parent = nullptr);

    ~DeviceInfo() {}
    
    QString getHost();
    quint16 getPort();
    // 获取本机网络IP
    QString netWorkIp();
    // 获取本机MAC地址
    QString macAddress();
    // 获取硬盘ID
    QString diskDeviceID();
    // 获取平台CPU的ID
    QString platformCpuId();

    static QString hash(const QString & str1, const QString & str2);
private:
    QString host;
    quint16 port;

    void getcpuid(unsigned int CPUInfo[], unsigned int InfoType);
    void getcpuidex(unsigned int CPUInfo[], unsigned int InfoType, unsigned int ECXValue);
};

#endif
