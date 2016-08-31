#include "gomokuserver.h"

GomokuServer::GomokuServer(QObject* parent) :
    QObject(parent)
{

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
       quit();
       deleteLater();
    });

    connect(this, &QObject::destroyed, this, &GomokuServer::quit);

}

void GomokuServer::quit()
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
    mServer.data()->close();
    emit socketCreated(mSocket.data()->peerAddress().toString());
    qInfo() << "new connection from " << mSocket.data()->peerAddress().toString();
    connect(mSocket.data(), &QTcpSocket::disconnected, [=]{
        emit disconnected();
        qInfo() << "disconnected.";
        quit();
        deleteLater();
    });
}
