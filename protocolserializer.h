#ifndef PROTOCOLPARSER_H
#define PROTOCOLPARSER_H

#include <QObject>
#include <QtNetwork>

class ProtocolSerializer : public QObject
{
    Q_OBJECT
public:
    explicit ProtocolSerializer(QObject *parent = 0);

    QTcpSocket *getSocket() const;
    void setSocket(QTcpSocket *value);

signals:
    void moveParsed(QPoint point);

public slots:
    void readyToRead();

private:
    QByteArray buffer;
    QTcpSocket* socket;
    QString HEADER = "YaoProtocol\n";
    int HEADER_LEN = HEADER.length();

    void parseMsg(QString& msg);
};

#endif // PROTOCOLPARSER_H
