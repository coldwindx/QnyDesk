#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QCommonStyle>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    deviceInfo = new DeviceInfo(this);
    trayIcon = new QSystemTrayIcon(this);
    trayMenu = new QMenu(this);
    // 系统托盘显示
    setTrayMenu();
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

void MainWindow::setTrayMenu()
{
    QCommonStyle style;     // 加载系统图标
    trayMenu->addAction(QIcon(style.standardPixmap(QStyle::SP_ComputerIcon)), "打开主界面");
    trayMenu->addAction(QIcon(style.standardPixmap(QStyle::SP_DialogCancelButton)), "退出");
    trayIcon->setContextMenu(trayMenu);
    trayIcon->setIcon(QIcon(":/src/img/favicon.ico"));
    trayIcon->setToolTip("QnyDesk");
    trayIcon->show();
    // 绑定槽函数
    connect(trayMenu, SIGNAL(triggered(QAction *)), this, SLOT(afterTrigger(QAction *)));
}

void MainWindow::afterTrigger(QAction * action)
{
    if("打开主界面" == action->text())
    {
        return this->show();
    }
    if("退出" == action->text())
    {
        emit closed();
    }
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
    // 连接成功
    connect(passiveNetworkHander, &PassiveHandler::connectStateChanged, this, &MainWindow::afterConnectStateChanged);
    // 窗口关闭
    connect(this, &MainWindow::closed, passiveNetworkHander, &PassiveHandler::removeSocket);
    connect(passiveNetworkHander, &PassiveHandler::finished, thread, &QThread::quit);
    connect(thread, &QThread::finished, passiveNetworkHander, &PassiveHandler::deleteLater);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(passiveNetworkHander, &PassiveHandler::finished, this, &MainWindow::afterSocketFinish);
    
    
    

    passiveNetworkHander->moveToThread(thread);
    thread->start();
}

void MainWindow::afterConnectStateChanged(bool flag)
{
    if(flag)
    {
        ui->stateLamp->setStyleSheet("background-color:rgb(50,190,166);border-radius:15px");
        ui->stateLabel->setText("就绪");
        return ;
    }
    ui->stateLamp->setStyleSheet("background-color:rgb(185,54,54);border-radius:15px");
    ui->stateLabel->setText("连接失败，启动重启。。。");
}

void MainWindow::afterSocketFinish()
{
    passiveNetworkHander = Q_NULLPTR;
    if(!passiveNetworkHander)
        QApplication::quit();       // 退出程序
}