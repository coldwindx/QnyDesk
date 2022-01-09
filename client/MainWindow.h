#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include <QThread>
#include <QSettings>
#include <QSystemTrayIcon>
#include "button/SettingButton.h"
#include "qny_passive/PassiveHandler.h"
#include "common/DeviceInfo.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void link();
signals:
    void closed();
private:
    Ui::MainWindow *ui;

    QThread * thread;                       // 处理网络连接的线程          
    PassiveHandler * passiveNetworkHander;  // 被控网络处理
    DeviceInfo * device;                    // 设备信息
    QSystemTrayIcon * tray;                 // 托盘

    void setTrayMenu();                                 // 设置系统托盘
    void loadSettings();                                // 加载配置信息
    void startPassiveConnect();                         // 连接远端服务器
    void quit();                                        // 程序退出
    void registerEvent(QString registerId);             // 被控网络注册事件

    void afterConnectStateChanged(bool flag);           // 连接成功后序操作
    void afterSocketFinish();

};

#endif // MAINWINDOW_H
