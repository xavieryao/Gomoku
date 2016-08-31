#include "protocolserializer.h"
#include "QJsonDocument"
#include "QJsonObject"

ProtocolSerializer::ProtocolSerializer(QObject *parent) : QObject(parent)
{

}

QTcpSocket *ProtocolSerializer::getSocket() const
{
    return socket;
}

void ProtocolSerializer::setSocket(QTcpSocket *value)
{
    socket = value;
}

void ProtocolSerializer::readyToRead()
{
    buffer.append(socket->readAll());
    QString str = buffer;
    int size = 0;
    while(str.contains(HEADER)) {
        str.remove(0, HEADER_LEN);
//        size += HEADER_LEN;
        QStringList list = str.split('\n');
        if (list.size()==0) {
            break;
        }
        int lengthSize = list.at(1).size();
        str.remove(0, lengthSize); // remove '[contentLength]\n'
        int contentSize = list.at(0).toInt();
        if (str.size() < contentSize) {
            break;
        }
        QString content = str.remove(contentSize);
        parseMsg(content);
        size += HEADER_LEN;
        size += lengthSize;
        size += contentSize;
    }
    buffer.remove(0, size);
}

void ProtocolSerializer::parseMsg(QString &msg)
{
    QJsonDocument doc = QJsonDocument::fromJson(msg.toUtf8());
    QJsonObject obj = doc.object();
    QString msgType = obj["msgType"].toString();
    if (msgType == "move") {
        int x = obj["x"].toInt();
        int y = obj["y"].toInt();
        QPoint point(x, y);
        qInfo() << "move parsed " << point;
        emit moveParsed(point);
        return;
    }
}
