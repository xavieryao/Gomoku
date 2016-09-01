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
    qDebug() << "ready to read";
    buffer.append(socket->readAll());
//    qInfo("read from socket");
//    qInfo() << "read " << buffer.size() << "bytes.";
//    qInfo() << QString(buffer);
    QString str = buffer;
    int size = 0;
    while(str.contains(HEADER)) {
        str.remove(0, HEADER_LEN);
//        qInfo() << "has header";
//        size += HEADER_LEN;
        QStringList list = str.split('\n');
        if (list.size()==0) {
            break;
        }
        int lengthSize = list.at(0).size() + 1;
//        qInfo() << "has length. field size: " << lengthSize;
        str.remove(0, lengthSize); // remove '[contentLength]\n'
        int contentSize = list.at(0).toInt();
//        qInfo() << "read content size:" << contentSize;
        if (str.size() < contentSize) {
            break;
        }
        QString content = str.remove(contentSize);
//        qInfo() << "read content:" << content;
        parseMsg(content);
        size += HEADER_LEN;
        size += lengthSize;
        size += contentSize;
    }
    buffer.remove(0, size);
    qDebug() << "done";
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
//        qInfo() << "move parsed " << point;
        emit moveParsed(point);
        return;
    }
    if (msgType == "nextGame") {
        emit nextGame();
        return;
    }
}

QByteArray ProtocolSerializer::serialize(QJsonObject msg)
{
    QJsonDocument doc(msg);
    QByteArray json = doc.toJson(QJsonDocument::Compact);
    QByteArray bytes = HEADER.toUtf8();
    bytes += QString::number(json.length());
    bytes += '\n';
    bytes += json;
    qDebug() << QString(bytes);
    return bytes;
}
