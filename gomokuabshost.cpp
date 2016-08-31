#include "gomokuabshost.h"

GomokuAbsHost::GomokuAbsHost(QObject* parent):
    QObject(parent),
    mSerializer(new ProtocolSerializer(this))
{

}

ProtocolSerializer *GomokuAbsHost::serializer() const
{
    return mSerializer;
}

void GomokuAbsHost::setSerializer(ProtocolSerializer *serializer)
{
    mSerializer = serializer;
}

void GomokuAbsHost::sendMove(const QPoint& position)
{
    if (mSocket) {
//        qInfo() << "send move data";
        QJsonObject obj;
        obj["msgType"] = "move";
        obj["x"] = position.x();
        obj["y"] = position.y();
        mSocket.data()->write(mSerializer->serialize(obj));
    }

}
