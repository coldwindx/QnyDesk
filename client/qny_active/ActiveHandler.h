#ifndef ACTIVE_HANDLER_H_
#define ACTIVE_HANDLER_H_
#include "common/NetworkHandler.h"

class ActiveHandler : public NetworkHandler
{
    Q_OBJECT
public:
    explicit ActiveHandler(DeviceInfo * device, QObject * parent = nullptr)
        : NetworkHandler(device, NetworkHandler::ACTIVE, parent)
    {
    }

    void request(const QString& remoteId, const QString & password)
    {
        BigPack::Exchange ex;
        ex.set_type(BigPack::Exchange::TypeRequestAuth);
        ex.set_resourceid(this->registerId);
        ex.set_targetid(remoteId.toStdString());

        BigPack::CsRequestAuth * auth = new BigPack::CsRequestAuth();
        auth->set_password(password.toStdString());
        ex.set_allocated_requestauth(auth);

        std::string msg;
        ex.SerializeToString(&msg);
        if(socket->isOpen())
        {
            socket->write(intToBytes(ex.ByteSizeLong()), sizeof(qint32));
            socket->write(msg.c_str(), ex.ByteSizeLong());
            socket->flush();
            return ;
        }
        qDebug() << "设备没有打开！";
    }
};

#endif
