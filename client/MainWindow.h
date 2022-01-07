#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include <QThread>
#include <QSettings>
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
private slots:
    void link();
private:
    Ui::MainWindow *ui;
    PassiveHandler * passiveNetworkHander;  // 被控网络处理
    DeviceInfo * deviceInfo;                // 设备信息

    void loadSettings();
    void startPassiveConnect();
    void afterConnectStateChanged(bool flag);           // 连接成功后序操作

};

#endif // MAINWINDOW_H
