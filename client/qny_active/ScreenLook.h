#ifndef SCREENLOOK_H
#define SCREENLOOK_H

#include <QThread>
#include <QCloseEvent>
#include "common/DeviceInfo.h"
#include "codec/VPxDecoder.h"
#include "ActiveHandler.h"
#include "PasswordDialog.h"


class ScreenLook : public VPxDecoder
{
    Q_OBJECT
public:
    ScreenLook(DeviceInfo * device, QWidget * parent = nullptr);
private:
    DeviceInfo * device;

    QThread * thread;
    ActiveHandler * activeHandler;
    PasswordDialog pwDialog;                    // 密码对话框

    void closeEvent(QCloseEvent *e);            // 窗口关闭事件
    void linkEvent(QString password);           // 连接被控主机
};

#endif // SCREENLOOK_H
