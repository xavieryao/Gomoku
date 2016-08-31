#include "gomokuclient.h"
#include <QDebug>

GomokuClient::GomokuClient(QString server, QObject* parent):
    QObject(parent),
    server(server)
{

}

void GomokuClient::start()
{
    QTcpSocket* socket = new QTcpSocket();
    socket->connectToHost(QHostAddress(server), 8888);
    connect(socket, &QTcpSocket::connected, this, &GomokuClient::connected);

    connect(socket, static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),
          [=](QAbstractSocket::SocketError socketError){
        emit error(socket->errorString());
        qDebug() << "socket error:" << socket->errorString();
        quit();
    });

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
    // TODO need some clean up.
}
