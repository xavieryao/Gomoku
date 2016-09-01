#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QTabWidget>
#include <QLabel>
#include <QPushButton>
#include "gomokuserver.h"
#include "gomokuclient.h"
#include "gomokuwidget.h"
#include "protocolserializer.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    bool eventFilter(QObject *watched, QEvent *event) override;

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
    QPushButton* mHint;

    ProtocolSerializer* mClientSerializer;
    ProtocolSerializer* mServerSerializer;

    int lockTab = 0;
};

#endif // MAINWINDOW_H
