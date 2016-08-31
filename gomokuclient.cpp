#include "gomokuclient.h"
#include <QDebug>

GomokuClient::GomokuClient(QString server, QObject* parent):
    GomokuAbsHost(parent),
    server(server)
{
    connect(mSerializer, &ProtocolSerializer::moveParsed, this, &GomokuClient::newMove);
}


void GomokuClient::start()
{
    mSocket = new QTcpSocket();
    mSocket.data()->connectToHost(QHostAddress(server), 8888);
    connect(mSocket.data(), &QTcpSocket::connected, [=]{
        mSerializer->setSocket(mSocket);
    });

    connect(mSocket.data(), &QTcpSocket::connected, [=]{
        emit connected(mSocket->peerAddress().toString());
    });

    connect(mSocket.data(), static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),
          [=](QAbstractSocket::SocketError socketError){
        emit error(mSocket.data()->errorString());
        qDebug() << "socket error:" << mSocket.data()->errorString();
        deleteLater();
    });

    connect(mSocket.data(), &QTcpSocket::disconnected, [=]{
       qInfo() << "disconnected";
       emit disconnected();
       deleteLater();
    });

    connect(mSocket.data(), &QTcpSocket::readyRead, mSerializer, &ProtocolSerializer::readyToRead);
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
    if (mSocket) {
        mSocket.data()->disconnectFromHost();
        mSocket.data()->close();
        mSocket->deleteLater();
    }
//    deleteLater();
}


