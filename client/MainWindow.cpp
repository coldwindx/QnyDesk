#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QCommonStyle>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setTrayMenu();          // 系统托盘显示
    loadSettings();         // 加载配置文件
    startPassiveConnect();  // 启动被控网络
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
    tray = new QSystemTrayIcon(this);
    QMenu * menu = new QMenu(this);
    // 加载系统图标
    QCommonStyle style; 
    // <1> 打开主界面
    QAction * open = new QAction(QIcon(style.standardPixmap(QStyle::SP_ComputerIcon)), "打开主界面");
    connect(open, &QAction::triggered, this, [&](){ this->show(); });
    menu->addAction(open);
    // <2> 退出程序
    QAction * close = new QAction(QIcon(style.standardPixmap(QStyle::SP_DialogCancelButton)), "退出");
    connect(close, &QAction::triggered, this, [&](){ emit closed(); });
    menu->addAction(close);
    // 系统托盘图标
    tray->setContextMenu(menu);
    tray->setIcon(QIcon(":/src/img/favicon.ico"));
    tray->setToolTip("QnyDesk");
    tray->show();
}

void MainWindow::loadSettings()
{
    // 加载配置文件
    QSettings settings("config.ini", QSettings::IniFormat);
    settings.beginGroup("REMOTE_DESKTOP_SERVER");

    QString remoteHost = settings.value("remoteHost").toString();
    int remotePort = settings.value("remotePort").toInt();
    if(remoteHost.isEmpty())
    {
        remoteHost = "localhost";
        settings.setValue("remoteHost", remoteHost);
    }
    if(0 == remotePort)
    {
        remotePort = 443;
        settings.setValue("remotePort", 443);
    }

    settings.endGroup();
    settings.sync();
    // 保存设备信息
    device = new DeviceInfo(remoteHost, remotePort, this);
}

void MainWindow::startPassiveConnect()
{
    this->thread = new QThread();
    passiveNetworkHander = new PassiveHandler(device);
    // 线程启动时，开始处理
    connect(thread, &QThread::started, passiveNetworkHander, &PassiveHandler::createSocket);
    // 连接成功
    connect(passiveNetworkHander, &PassiveHandler::connectStateChanged, this, &MainWindow::afterConnectStateChanged);
    // 窗口关闭
    // <1> 关闭socket连接
    connect(this, &MainWindow::closed, passiveNetworkHander, &PassiveHandler::removeSocket);
    // <2> socket连接关闭后，执行后续处理
    connect(passiveNetworkHander, &PassiveHandler::finished, this, &MainWindow::quit);

    passiveNetworkHander->moveToThread(thread);
    thread->start();
}

void MainWindow::quit()
{
    passiveNetworkHander->deleteLater();
    thread->quit();
    thread->wait();
    thread->deleteLater();
    QApplication::quit();       // 退出程序
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
