#ifndef GOMOKUCLIENT_H
#define GOMOKUCLIENT_H

#include <QtNetwork>
#include "protocolserializer.h"
#include "gomokuabshost.h"

class GomokuClient : public GomokuAbsHost
{
    Q_OBJECT
public:
    GomokuClient(QString server, QObject* parent = 0);
    ~GomokuClient();

    QString& getServer();
    void setServer(const QString &value);

    void start() override;

private:
    QString server;
};

#endif // GOMOKUCLIENT_H
