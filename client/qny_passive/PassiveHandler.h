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
};

#endif
