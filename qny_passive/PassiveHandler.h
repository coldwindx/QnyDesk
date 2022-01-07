#ifndef PASSIVE_HANDLER_H_
#define PASSIVE_HANDLER_H_
#include "common/NetworkHandler.h"

class PassiveHandler : public NetworkHandler
{
    Q_OBJECT
public:
    explicit PassiveHandler(QObject * parent = nullptr) : NetworkHandler(parent)
    {
        
    }
};

#endif