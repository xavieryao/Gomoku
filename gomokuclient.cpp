#include "gomokuclient.h"
#include <QDebug>

GomokuClient::GomokuClient(QString server):
    server(server)
{

}

void GomokuClient::run()
{
    QTcpSocket* socket = new QTcpSocket();
    socket->connectToHost(QHostAddress(server), 8888);
    if (socket->waitForConnected()) {
        qDebug() << "connected.";
        emit connected();
    } else {
        emit connectionError(socket->errorString());
        qDebug() << "connection error " << socket->errorString();
        quit();
    }
    connect(socket, static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),
          [=](QAbstractSocket::SocketError socketError){
        emit error(socket->errorString());
        qDebug() << "socket error:" << socket->errorString();
        quit();
    });
    exec();

}

QString& GomokuClient::getServer()
{
    return server;
}

void GomokuClient::setServer(const QString &value)
{
    server = value;
}
