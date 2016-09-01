#ifndef GOMOKUSERVER_H
#define GOMOKUSERVER_H

#include <QtNetwork>
#include "protocolserializer.h"
#include <QJsonObject>
#include "gomokuabshost.h"

class GomokuServer : public GomokuAbsHost
{
    Q_OBJECT
public:
    GomokuServer(int port, QObject* parent = 0);
    ~GomokuServer();
    void start() override;

private:
    QPointer<QTcpServer> mServer;
    int port;

private slots:
    void onNewConnection();


};

#endif // GOMOKUSERVER_H
