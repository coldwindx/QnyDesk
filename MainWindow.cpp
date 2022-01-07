#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    deviceInfo = new DeviceInfo(this);

    // 加载配置文件
    loadSettings();
    // 启动被控网络
    startPassiveConnect();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::link()
{
    qDebug() << "This button is clicked!";
}

void MainWindow::loadSettings()
{
    // 加载配置文件
    QSettings settings("config.ini", QSettings::IniFormat);
    settings.beginGroup("REMOTE_DESKTOP_SERVER");

    QString remoteHost = settings.value("remoteHost").toString();
    if(remoteHost.isEmpty())
    {
        remoteHost = "localhost";
        settings.setValue("remoteHost", remoteHost);
    }
    int remotePort = settings.value("remotePort").toInt();
    if(0 == remotePort)
    {
        remotePort = 443;
        settings.setValue("remotePort", 443);
    }

    settings.endGroup();
    settings.sync();
    // 保存设备信息
    deviceInfo->setRemoteInfo(remoteHost, remotePort);
}

void MainWindow::startPassiveConnect()
{
    QThread * thread = new QThread();
    passiveNetworkHander = new PassiveHandler();
    passiveNetworkHander->init(deviceInfo, NetworkHandler::PASSIVE);
    // 线程启动时，开始处理
    connect(thread, &QThread::started, passiveNetworkHander, &PassiveHandler::createSocket);

    passiveNetworkHander->moveToThread(thread);
    thread->start();
}