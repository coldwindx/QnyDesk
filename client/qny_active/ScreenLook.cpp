#include "ScreenLook.h"

ScreenLook::ScreenLook(DeviceInfo * device, QWidget * parent) : VPxDecoder (parent)
{
    this->device = device;
    // 监听鼠标
    setMouseTracking(true);
    // 初始化
    thread = new QThread();
    activeHandler = new ActiveHandler(device, this);
    // 线程开始时，创建Socket
    connect(thread, &QThread::start, activeHandler, &ActiveHandler::createSocket);
    // 密码对话框请求认证
    connect(&pwDialog, &PasswordDialog::link, this, &ScreenLook::linkEvent);
}

void ScreenLook::closeEvent(QCloseEvent * e)
{
    // 关闭Socket
    activeHandler->removeSocket();
    activeHandler->deleteLater();
    // 关闭线程
    thread->quit();
    thread->wait();
    thread->deleteLater();
    // 接受信息，退出窗口
    e->accept();
}

void ScreenLook::linkEvent(QString password)
{
    activeHandler->request(device->getId(), password);
}
