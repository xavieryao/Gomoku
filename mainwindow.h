#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include "gomokuserver.h"
#include "gomokuclient.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

signals:

public slots:

private:
    void setupServer(QWidget* serverWidget);
    void setupClient(QWidget* clientWidget);

    QHostAddress localAddress();
    QPointer<GomokuServer> server;
    GomokuClient* client = 0;
};

#endif // MAINWINDOW_H
