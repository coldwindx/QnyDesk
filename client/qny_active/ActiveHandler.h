#ifndef ACTIVE_HANDLER_H_
#define ACTIVE_HANDLER_H_
#include "common/NetworkHandler.h"

class ActiveHandler : public NetworkHandler
{
    Q_OBJECT
public:
    explicit ActiveHandler(DeviceInfo * device, QObject * parent = nullptr)
        : NetworkHandler(device, NetworkHandler::PASSIVE, parent)
    {
    }

};

#endif
