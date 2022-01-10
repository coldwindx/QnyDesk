#include "deviceinfo.h"
#include <QProcess>
#include <QHostInfo>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QCryptographicHash>

#ifdef __GNUC__
#include <cpuid.h>
#elif defined(_MSC_VER)
#if _MSC_VER >= 1400
#include <intrin.h>
#endif
#else
#error Only supports MSVC or GCC
#endif

void DeviceInfo::setHost(QString host)
{
    this->host = host;
}

void DeviceInfo::setPort(quint16 port)
{
    this->port = port;
}

QString  DeviceInfo::getHost(){
    return this->host ;
}

quint16 DeviceInfo::getPort(){
    return this->port;
}

//得到网络地址
QString DeviceInfo::netWorkIp()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach(QHostAddress address, list)
    {
        if (address.protocol() == QAbstractSocket::IPv4Protocol)
            //我们使用IPv4地址
            return address.toString();
    }
    return 0;
}
//得到MAC地址
QString DeviceInfo::macAddress()
{
    QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();// 获取所有网络接口列表
    int nCnt = nets.count();
    QString strMacAddr = "";
    for (int i = 0; i < nCnt; i++)
    {
        // 如果此网络接口被激活并且正在运行并且不是回环地址，则就是我们需要找的Mac地址
        if (nets[i].flags().testFlag(QNetworkInterface::IsUp) && nets[i].flags().testFlag(QNetworkInterface::IsRunning) && !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack))
        {
            strMacAddr = nets[i].hardwareAddress();
            break;
        }
    }
    return strMacAddr;
}
//获取硬盘设备（linux平台待实现）
QString DeviceInfo::diskDeviceID()
{
#ifdef Q_OS_WIN
    QProcess p(0);
    p.start("wmic diskdrive get serialnumber");
    p.waitForStarted();
    p.waitForFinished();
    QString result = QString::fromLocal8Bit(p.readAllStandardOutput());
    QStringList list = result.split("\r\n");
    if (list.size() < 1)
        return "error";
    return list.at(1).trimmed();
#endif
#ifdef Q_OS_UNIX
    int fd;
    struct hd_driveid hid;
    FILE *fp;
    char line[0x100], *disk, *root, *p;

    fp = fopen ("/etc/mtab", "r");
    if (fp == NULL)
    {
        fprintf (stderr, "No /etc/mtab file.\n");
        return -1;
    }

    fd = -1;
    while (fgets (line, sizeof line, fp) != NULL)
    {
        disk = strtok (line, " ");
        if (disk == NULL)
        {
            continue;
        }

        root = strtok (NULL, " ");
        if (root == NULL)
        {
            continue;
        }

        if (strcmp (root, "/") == 0)
        {
            for (p = disk + strlen (disk) - 1; isdigit (*p); p --)
            {
                *p = '\0';
            }
            fd = open (disk, O_RDONLY);
            break;
        }
    }

    fclose(fp);

    if(fd < 0)
    {
        fprintf(stderr, "open hard disk device failed.\n");
        return -1;
    }

    if(ioctl(fd, HDIO_GET_IDENTITY, &hid) < 0)
    {
        fprintf (stderr, "ioctl error.\n");
        return -1;
    }

    close (fd);

    snprintf (id, max, "%s", hid.serial_no  );
    fprintf (stdout, "get hard disk serial number: %s/n", id);

    return 0;
#endif
}

//获取机器cpuId(全平台）
QString DeviceInfo::platformCpuId()
{
    QString cpu_id = "";
    unsigned int dwBuf[4];
    unsigned long long ret;
    getcpuid(dwBuf, 1);
    ret = dwBuf[3];
    ret = ret << 32;
    cpu_id = QString::number(dwBuf[3], 16).toUpper();
    int clenght = 8 - cpu_id.length() ;
    for(int i=0;i<clenght;i++){
        cpu_id.prepend("0");
    }
    QString cpu_id2 = QString::number(dwBuf[0], 16).toUpper();
    clenght = 8 - cpu_id2.length() ;
    for(int i=0;i<clenght;i++){
        cpu_id2.prepend("0");
    }
    return cpu_id+cpu_id2;
}

QString DeviceInfo::getPassword()
{
    return password;
}

void DeviceInfo::setPassword(const QString &password)
{
    this->password = password;
}


void DeviceInfo::getcpuid(unsigned int CPUInfo[4], unsigned int InfoType)
{
#if defined(__GNUC__)// GCC
    __cpuid(InfoType, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
#elif defined(_MSC_VER)// MSVC
    #if _MSC_VER >= 1400 //VC2005才支持__cpuid
        __cpuid((int*)(void*)CPUInfo, (int)(InfoType));
    #else //其他使用getcpuidex
        getcpuidex(CPUInfo, InfoType, 0);
    #endif
#endif
}

void DeviceInfo::getcpuidex(unsigned int CPUInfo[4], unsigned int InfoType, unsigned int ECXValue)
{
#if defined(_MSC_VER) // MSVC
    #if defined(_WIN64)    // 64位下不支持内联汇编. 1600: VS2010, 据说VC2008 SP1之后才支持__cpuidex.
        __cpuidex((int*)(void*)CPUInfo, (int)InfoType, (int)ECXValue);
    #else
        if (NULL==CPUInfo)    return;
        _asm{
            // load. 读取参数到寄存器.
            mov edi, CPUInfo;
            mov eax, InfoType;
            mov ecx, ECXValue;
            // CPUID
            cpuid;
            // save. 将寄存器保存到CPUInfo
            mov    [edi], eax;
            mov    [edi+4], ebx;
            mov    [edi+8], ecx;
            mov    [edi+12], edx;
        }
    #endif
#endif
}

QString DeviceInfo::hash(const QString & str1, const QString & str2)
{
    QString sum = str1 + str2;
    QByteArray array = QCryptographicHash::hash(sum.toUtf8(), QCryptographicHash::Md5);
    return array.toBase64();
}

void DeviceInfo::xorData(QByteArray &data)
{
    static QByteArray key1 = "privatekey";
    static QByteArray key2 = "helloworld";
    for(int i = 0, size = data.size(); i < size; ++i)
        data[i] = data[i] ^ key1[i % key1.size()] ^ key2[i % key2.size()];
}

