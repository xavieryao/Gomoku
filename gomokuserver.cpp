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
    mServer.data()->close();
    emit socketCreated(mSocket.data()->peerAddress().toString());
    qInfo() << "new connection from " << mSocket.data()->peerAddress().toString();
    connect(mSocket.data(), &QTcpSocket::disconnected, [=]{
        emit disconnected();
        qInfo() << "disconnected.";
        deleteLater();
    });

    // DEBUG
    qInfo() << "send debug data";
    QString header = "YaoProtocol\n%1\n";
    QString msg = "{\"msgType\":\"move\", \"x\":3, \"y\":5}\n";
    header = header.arg(msg.toUtf8().size());
    QString towrite = header + msg;
    mSocket->write(towrite.toUtf8());
}
