#include "gomokuabshost.h"

GomokuAbsHost::GomokuAbsHost(QObject* parent):
    QObject(parent)
{

}

ProtocolSerializer *GomokuAbsHost::serializer() const
{
    return mSerializer;
}

void GomokuAbsHost::setSerializer(ProtocolSerializer *serializer)
{
    mSerializer = serializer;
    connect(mSerializer, &ProtocolSerializer::moveParsed, this, &GomokuAbsHost::newMove);
    connect(mSerializer, &ProtocolSerializer::nextGame, this, &GomokuAbsHost::nextGame);
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

void GomokuAbsHost::requestNextGame()
{
    if (mSocket) {
        QJsonObject obj;
        obj["msgType"] = "nextGame";
        mSocket->write(mSerializer->serialize(obj));
    }
}
