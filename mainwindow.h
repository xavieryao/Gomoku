#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QTabWidget>
#include "gomokuserver.h"
#include "gomokuclient.h"
#include "gomokuwidget.h"

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
};

#endif // MAINWINDOW_H
