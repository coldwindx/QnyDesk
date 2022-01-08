#ifndef PASSIVE_HANDLER_H_
#define PASSIVE_HANDLER_H_
#include "common/NetworkHandler.h"

class PassiveHandler : public NetworkHandler
{
    Q_OBJECT
public:
    explicit PassiveHandler(const QString & host, quint16 port, QObject * parent = nullptr) 
        : NetworkHandler(host, port, NetworkHandler::PASSIVE, parent)
    {
        
    }
};

#endif