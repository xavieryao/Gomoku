#include "gomokuserver.h"

GomokuServer::GomokuServer(QObject* parent) :
    QObject(parent)
{

}

void GomokuServer::start()
{
    mServer = new QTcpServer();
    qDebug() << "listen" << mServer.data()->listen(QHostAddress::Any, 8888);

//    mServer.data()->waitForNewConnection();
    connect(mServer.data(), &QTcpServer::newConnection, this, &GomokuServer::onNewConnection);

    connect(mServer.data(), &QTcpServer::acceptError, [=]{
       qDebug() << "accept error:" << mServer.data()->errorString();
       emit error(mServer.data()->errorString());
       quit();
    });
}

void GomokuServer::quit()
{
    qDebug() << "onThreadFinished";
    if (mServer) {
        mServer.data()->close();
        mServer.data()->deleteLater();
    }

    if (mSocket) {
        mSocket.data()->close();
        mSocket.data()->deleteLater();
    }
}

void GomokuServer::onNewConnection()
{
    mSocket = mServer.data()->nextPendingConnection();
    mServer.data()->close();
    emit socketCreated(mSocket.data()->peerAddress().toString());
    qDebug() << "new connection from " << mSocket.data()->peerAddress().toString();
}
