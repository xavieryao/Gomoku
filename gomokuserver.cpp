#include "gomokuserver.h"

GomokuServer::GomokuServer(QObject* parent) :
    QObject(parent),
    mSerializer(new ProtocolSerializer(this))
{
    connect(mSerializer, &ProtocolSerializer::moveParsed, this, &GomokuServer::newMove);
}

void GomokuServer::start()
{
    mServer = new QTcpServer();
    qInfo() << "listen" << mServer.data()->listen(QHostAddress::Any, 8888);

//    mServer.data()->waitForNewConnection();
    connect(mServer.data(), &QTcpServer::newConnection, this, &GomokuServer::onNewConnection);

    connect(mServer.data(), &QTcpServer::acceptError, [=]{
       qInfo() << "accept error:" << mServer.data()->errorString();
       emit error(mServer.data()->errorString());
       deleteLater();
    });

}

GomokuServer::~GomokuServer()
{
    qInfo() << "clean up server";
    if (mServer) {
        mServer.data()->close();
        mServer.data()->deleteLater();
    }

    if (mSocket) {
        mSocket.data()->disconnectFromHost();
        mSocket.data()->close();
        mSocket.data()->deleteLater();
    }

//    this->deleteLater();
}

void GomokuServer::onNewConnection()
{
    mSocket = mServer.data()->nextPendingConnection();
    mServer->pauseAccepting();
    emit socketCreated(mSocket.data()->peerAddress().toString());
    qInfo() << "new connection from " << mSocket.data()->peerAddress().toString();
    qInfo() << "local port" << mSocket->localPort();
    mSerializer->setSocket(mSocket);
    connect(mSocket, &QTcpSocket::readyRead, mSerializer, &ProtocolSerializer::readyToRead);
    connect(mSocket.data(), &QTcpSocket::disconnected, [=]{
        emit disconnected();
        qInfo() << "disconnected.";
        deleteLater();
    });


}

void GomokuServer::sendMove(const QPoint& position)
{
    if (mSocket) {
        qInfo() << "send move data";
        QJsonObject obj;
        obj["msgType"] = "move";
        obj["x"] = position.x();
        obj["y"] = position.y();
        mSocket.data()->write(mSerializer->serialize(obj));
    }

}
