#ifndef GOMOKUCLIENT_H
#define GOMOKUCLIENT_H

#include <QtNetwork>

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

signals:
    void connected();
    void error(const QString errorStr);
    void disconnected();

};

#endif // GOMOKUCLIENT_H
