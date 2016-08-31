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


protected:
    ProtocolSerializer* mSerializer;
    QPointer<QTcpSocket> mSocket;

public slots:
    void sendMove(const QPoint& position);

signals:
    void connected(const QString peer);
    void error(const QString errorStr);
    void disconnected();
    void newMove(QPoint position);
};

#endif // GOMOKUABSHOST_H
