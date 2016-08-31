#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QTabWidget>
#include <QLabel>
#include "gomokuserver.h"
#include "gomokuclient.h"
#include "gomokuwidget.h"
#include "protocolserializer.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

signals:

public slots:

private:
    void setupServer();
    void setupClient();

    QHostAddress localAddress();
    QPointer<GomokuServer> mServer;
    QPointer<GomokuClient> mClient;

    GomokuWidget* gomoku;
    QWidget* mServerWidget;
    QWidget* mClientWidget;
    QTabWidget* mTab;

    QLabel* mPlayer;
    QLabel* mTurn;

    ProtocolSerializer* mClientSerializer;
    ProtocolSerializer* mServerSerializer;
};

#endif // MAINWINDOW_H
