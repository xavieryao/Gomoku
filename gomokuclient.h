#ifndef GOMOKUCLIENT_H
#define GOMOKUCLIENT_H

#include <QThread>
#include <QtNetwork>

class GomokuClient : public QThread
{
    Q_OBJECT
public:
    GomokuClient(QString server);
    void run() Q_DECL_OVERRIDE;

    QString& getServer();
    void setServer(const QString &value);

private:
    QString server;

signals:
    void connected();
    void connectionError(const QString errorStr);
    void error(const QString errorStr);
};

#endif // GOMOKUCLIENT_H
