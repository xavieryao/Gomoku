#ifndef GOMOKUSERVER_H
#define GOMOKUSERVER_H

#include <QtNetwork>
#include "protocolserializer.h"
#include <QJsonObject>

class GomokuServer : public QObject
{
    Q_OBJECT
public:
    GomokuServer(QObject* parent = 0);
    ~GomokuServer();
    void start();

private:
    QPointer<QTcpServer> mServer;
    QPointer<QTcpSocket> mSocket;

    ProtocolSerializer* mSerializer;

signals:
    void socketCreated(QString clientAddr);
    void error(const QString errorString);
    void disconnected();
    void newMove(QPoint position);

public slots:
    void sendMove(const QPoint& position);

private slots:
    void onNewConnection();


};

#endif // GOMOKUSERVER_H
