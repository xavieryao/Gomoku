#ifndef GOMOKUCLIENT_H
#define GOMOKUCLIENT_H

#include <QtNetwork>
#include "protocolserializer.h"

class GomokuClient : public QObject
{
    Q_OBJECT
public:
    GomokuClient(QString server, QObject* parent = 0);
    ~GomokuClient();
    void start();

    QString& getServer();
    void setServer(const QString &value);

private:
    QString server;
    QPointer<QTcpSocket> socket;
    ProtocolSerializer* serializer;

signals:
    void connected();
    void error(const QString errorStr);
    void disconnected();
    void newMove(QPoint position);

public slots:
    void sendMove(const QPoint& position);
};

#endif // GOMOKUCLIENT_H
