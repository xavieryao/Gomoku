#ifndef PROTOCOLPARSER_H
#define PROTOCOLPARSER_H

#include <QObject>
#include <QtNetwork>
#include <QJsonObject>

class ProtocolSerializer : public QObject
{
    Q_OBJECT
public:
    explicit ProtocolSerializer(QObject *parent = 0);

    QTcpSocket *getSocket() const;
    void setSocket(QTcpSocket *value);
    QByteArray serialize(QJsonObject msg);

signals:
    void moveParsed(QPoint point);
    void nextGame();

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
