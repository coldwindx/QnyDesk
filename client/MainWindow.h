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
    void afterTrigger(QAction * action);
signals:
    void closed();
private:
    Ui::MainWindow *ui;
    PassiveHandler * passiveNetworkHander;  // 被控网络处理
    DeviceInfo * deviceInfo;                // 设备信息
    QSystemTrayIcon * trayIcon;             // 托盘
    QMenu * trayMenu;                       // 托盘菜单


    void setTrayMenu();                                 // 设置系统托盘
    void loadSettings();
    void startPassiveConnect();

    void afterConnectStateChanged(bool flag);           // 连接成功后序操作
    void afterSocketFinish();

};

#endif // MAINWINDOW_H
