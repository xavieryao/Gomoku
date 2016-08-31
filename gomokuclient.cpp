#include "gomokuclient.h"
#include <QDebug>

GomokuClient::GomokuClient(QString server, QObject* parent):
    QObject(parent),
    server(server)
{

}

void GomokuClient::start()
{
    socket = new QTcpSocket();
    socket.data()->connectToHost(QHostAddress(server), 8888);
    connect(socket.data(), &QTcpSocket::connected, this, &GomokuClient::connected);

    connect(socket.data(), static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),
          [=](QAbstractSocket::SocketError socketError){
        emit error(socket.data()->errorString());
        qDebug() << "socket error:" << socket.data()->errorString();
        quit();
    });

    connect(this, &QObject::destroyed, this, &GomokuClient::quit);
}

QString& GomokuClient::getServer()
{
    return server;
}

void GomokuClient::setServer(const QString &value)
{
    server = value;
}

void GomokuClient::quit()
{
    if (socket) {
        socket.data()->disconnectFromHost();
        socket.data()->close();
        socket.data()->deleteLater();
    }
}
