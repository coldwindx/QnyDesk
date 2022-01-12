#ifndef DEVICE_INFO_H_
#define DEVICE_INFO_H_

#include <QObject>

class DeviceInfo : public QObject
{
    Q_OBJECT
public:
     explicit DeviceInfo(QObject * parent = nullptr) : QObject(parent) {}
    ~DeviceInfo() {}
    void setHost(QString host);
    void setPort(quint16 port);
    void setId(QString id);
    QString getHost();
    quint16 getPort();
    QString getId();
    // 获取本机网络IP
    QString netWorkIp();
    // 获取本机MAC地址
    QString macAddress();
    // 获取硬盘ID
    QString diskDeviceID();
    // 获取平台CPU的ID
    QString platformCpuId();
    // 获取密码
    QString getPassword();
    // 设置密码
    void setPassword(const QString & password);

    static QString hash(const QString & str1, const QString & str2);
    static void xorData(QByteArray & data);
private:
    QString host;
    quint16 port;
    QString password;
    QString id;

    void getcpuid(unsigned int CPUInfo[], unsigned int InfoType);
    void getcpuidex(unsigned int CPUInfo[], unsigned int InfoType, unsigned int ECXValue);
};

#endif
