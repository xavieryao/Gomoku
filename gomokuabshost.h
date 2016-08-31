#ifndef GOMOKUABSHOST_H
#define GOMOKUABSHOST_H

#include <QObject>
#include "protocolserializer.h"
#include <QtNetwork>

class GomokuAbsHost : public QObject
{
    Q_OBJECT
public:
    GomokuAbsHost(QObject* parent);
    ProtocolSerializer *serializer() const;
    void setSerializer(ProtocolSerializer *serializer);

    virtual void start() = 0;

    bool alive() {
        return (mSocket && mSocket->isOpen());
    }


protected:
    ProtocolSerializer* mSerializer;
    QPointer<QTcpSocket> mSocket;

public slots:
    void sendMove(const QPoint& position);
    void requestNextGame();

signals:
    void connected(const QString peer);
    void error(const QString errorStr);
    void disconnected();
    void newMove(QPoint position);
    void nextGame();
};

#endif // GOMOKUABSHOST_H
