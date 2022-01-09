#ifndef PASSIVE_HANDLER_H_
#define PASSIVE_HANDLER_H_
#include "common/NetworkHandler.h"

class PassiveHandler : public NetworkHandler
{
    Q_OBJECT
public:
    explicit PassiveHandler(DeviceInfo * device, QObject * parent = nullptr)
        : NetworkHandler(device, NetworkHandler::PASSIVE, parent)
    {
        
    }
    virtual void dealProto(int type, BigPack::Exchange & ex)
    {
        switch(type)
        {
            case BigPack::Exchange::TypeReplyRegisterDetails:
            {
                if(ex.replyinfo().success())
                {
                    QString registerId = QString::fromStdString(ex.replyinfo().registerid());
                    emit registered(registerId);
                }
                break;
            }
            default: break;
        }
    }
signals:
    void registered(QString registerId);
};

#endif
