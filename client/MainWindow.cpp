#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QCommonStyle>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //--------------------按钮信号------------------------
    connect(ui->eyeBtn, &QPushButton::clicked, this, &MainWindow::showPassword);
    connect(ui->pencilBtn, &PencilButton::reflash, this, &MainWindow::reflashPassword);
    connect(ui->pencilBtn, &PencilButton::replace, this, &MainWindow::replacePassword);

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
    QString remoteId = ui->remoteIdEdit->text().remove(QRegExp("\\s"));
    // 无法连接自己
    
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
    device = new DeviceInfo(this);
    // 加载配置文件
    QSettings settings("config.ini", QSettings::IniFormat);
    settings.beginGroup("REMOTE_DESKTOP_SERVER");
    // <1> 加载远程服务器信息
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
    // <2> 加载密码
    QString password = settings.value("password").toString();
    if(password.isEmpty())
    {
        QString uuid = QUuid::createUuid().toString().remove("{").remove("}").remove("-");
        password = uuid.right(6);
        QByteArray npw = password.toLatin1();
        DeviceInfo::xorData(npw);
        settings.setValue("password", npw);
    }else
    {
        QByteArray npw = password.toLatin1();
        DeviceInfo::xorData(npw);
        password = npw;
    }
    device->setPassword(password);
    settings.endGroup();
    settings.sync();

    device->setHost(remoteHost);
    device->setPort(remotePort);
    device->setPassword(password);
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
    //      <1> 关闭socket连接
    connect(this, &MainWindow::closed, passiveNetworkHander, &PassiveHandler::removeSocket);
    //      <2> socket连接关闭后，执行后续处理
    connect(passiveNetworkHander, &PassiveHandler::finished, this, &MainWindow::quit);
    // 被控网络登记注册成功
    connect(passiveNetworkHander, &PassiveHandler::registered, this, &MainWindow::registerEvent);
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

void MainWindow::registerEvent(QString id)
{
    id = id.mid(0, 3) + " " + id.mid(3, 3) + " " + id.mid(6, 3);
    ui->idLabel->setText(id);
    ui->connectBtn->setEnabled(true);
}

void MainWindow::setPassword(const QString & password)
{
    QSettings setting("config.ini", QSettings::IniFormat);
    setting.beginGroup("REMOTE_DESKTOP_SERVER");

    device->setPassword(password);
    QByteArray npw = password.toLatin1();
    DeviceInfo::xorData(npw);
    setting.setValue("password", npw);

    setting.endGroup();
    setting.sync();
}

void MainWindow::showPassword(bool checked)
{
    bool show = ui->eyeBtn->changeState();
    QString pw = show ? device->getPassword() : "******";
    ui->passwordLabel->setText(pw);
}

QString MainWindow::reflashPassword()
{
    QString uuid = QUuid::createUuid().toString().remove("{").remove("}").remove("-");
    QString password = uuid.right(6);
    setPassword(password);
    bool show = ui->eyeBtn->getState();
    QString pw = show ? device->getPassword() : "******";
    ui->passwordLabel->setText(pw);
    return password;
}

void MainWindow::replacePassword()
{

}
