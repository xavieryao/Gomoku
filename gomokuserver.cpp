#include "gomokuserver.h"

GomokuServer::GomokuServer(QObject* parent) :
    GomokuAbsHost(parent)
{
}


void GomokuServer::start()
{
    mServer = new QTcpServer();
    if (!mServer.data()->listen(QHostAddress::Any, 8888)){
        emit error("Can not listen.");
        deleteLater();
    }

//    mServer.data()->waitForNewConnection();
    connect(mServer.data(), &QTcpServer::newConnection, this, &GomokuServer::onNewConnection);

    connect(mServer.data(), &QTcpServer::acceptError, [=]{
       emit error(mServer.data()->errorString());
       deleteLater();
    });

}


GomokuServer::~GomokuServer()
{
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
    emit connected(mSocket->peerAddress().toString());
    mSerializer->setSocket(mSocket);
    connect(mSocket, &QTcpSocket::readyRead, mSerializer, &ProtocolSerializer::readyToRead);
    connect(mSocket.data(), &QTcpSocket::disconnected, [=]{
        emit disconnected();
        deleteLater();
    });


}
