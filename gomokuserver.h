#ifndef GOMOKUSERVER_H
#define GOMOKUSERVER_H

#include <QThread>
#include <QtNetwork>

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

signals:
    void socketCreated(QString clientAddr);
    void error(const QString errorString);
    void disconnected();

private slots:
    void onNewConnection();


};

#endif // GOMOKUSERVER_H
