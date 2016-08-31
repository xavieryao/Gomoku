#include "gomokuclient.h"
#include <QDebug>

GomokuClient::GomokuClient(QString server, QObject* parent):
    QObject(parent),
    server(server),
    serializer(new ProtocolSerializer(this))
{
    connect(serializer, &ProtocolSerializer::moveParsed, this, &GomokuClient::newMove);
}

void GomokuClient::start()
{
    socket = new QTcpSocket();
    socket.data()->connectToHost(QHostAddress(server), 8888);
    connect(socket.data(), &QTcpSocket::connected, [=]{
        serializer->setSocket(socket);
    });

    connect(socket.data(), &QTcpSocket::connected, this, &GomokuClient::connected);

    connect(socket.data(), static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),
          [=](QAbstractSocket::SocketError socketError){
        emit error(socket.data()->errorString());
        qDebug() << "socket error:" << socket.data()->errorString();
        deleteLater();
    });

    connect(socket.data(), &QTcpSocket::disconnected, [=]{
       qInfo() << "disconnected";
       emit disconnected();
       deleteLater();
    });

    connect(socket.data(), &QTcpSocket::readyRead, serializer, &ProtocolSerializer::readyToRead);
}

QString& GomokuClient::getServer()
{
    return server;
}

void GomokuClient::setServer(const QString &value)
{
    server = value;
}

GomokuClient::~GomokuClient()
{
    qDebug() << "quit";
    if (socket) {
        socket.data()->disconnectFromHost();
        socket.data()->close();
        socket->deleteLater();
    }
//    deleteLater();
}

void GomokuClient::sendMove(const QPoint& position)
{
    if (socket) {
        qInfo() << "send move data";
        QJsonObject obj;
        obj["msgType"] = "move";
        obj["x"] = position.x();
        obj["y"] = position.y();
        socket.data()->write(serializer->serialize(obj));
    }

}
