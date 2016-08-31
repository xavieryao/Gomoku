#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QtNetwork>
#include "gomokuwidget.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle(tr("Gomoku"));
    QWidget* centralWidget = new QWidget(this);
    QHBoxLayout* centralLayout = new QHBoxLayout(this);
    centralWidget->setLayout(centralLayout);

    GomokuWidget* gomoku = new GomokuWidget(this);

    QWidget* sideBar = new QWidget(this);
    QVBoxLayout* lay = new QVBoxLayout(this);
    QTabWidget* tab = new QTabWidget(this);
    lay->addWidget(tab);
    sideBar->setLayout(lay);

    QWidget* serverWidget = new QWidget(this);
    QVBoxLayout* serverLayout = new QVBoxLayout(this);
    serverWidget->setLayout(serverLayout);

    QWidget* clientWidget = new QWidget(this);
    QVBoxLayout* clientLayout = new QVBoxLayout(this);
    clientWidget->setLayout(clientLayout);

    setupServer(serverWidget);
    setupClient(clientWidget);

    tab->addTab(serverWidget, tr("Server"));
    tab->addTab(clientWidget, tr("Client"));

    centralLayout->addWidget(gomoku, 6);
    centralLayout->addWidget(sideBar, 4);
    setCentralWidget(centralWidget);
    resize(600, 360);
}

void MainWindow::setupServer(QWidget *serverWidget)
{
    QVBoxLayout* layout = static_cast<QVBoxLayout *>(serverWidget->layout());
    QLabel* ipLabel = new QLabel(this);
    QLabel* statusLabel = new QLabel(this);
    ipLabel->setText(tr("Local IP:%1").arg(localAddress().toString()));
    QPushButton* cancel = new QPushButton(tr("Stop Server"), this);
    cancel->setEnabled(false);
    QPushButton* startServer = new QPushButton(tr("Start Server"), this);
    connect(startServer, &QPushButton::clicked, [=]{

        statusLabel->setText("");
        if (server) {
            server.data()->quit();
        }
        server = new GomokuServer();
        connect(server.data(), &GomokuServer::error, [=](const QString error){
//            cancel->setEnabled(true);
            statusLabel->setText(error);
            startServer->setText(tr("Start Server"));
            startServer->setEnabled(true);
            cancel->setEnabled(false);
        });
        connect(server.data(), &GomokuServer::socketCreated, [=](const QString client){
            statusLabel->setText(tr("Client %1 Connected.").arg(client));
            startServer->setText(tr("Connected"));
        });
        startServer->setText(tr("Listening..."));
        startServer->setEnabled(false);
        cancel->setEnabled(true);
        server.data()->start();

    });

    connect(cancel, &QPushButton::clicked, [=]{
        if (server) {
            server.data()->quit();
        }
        startServer->setText(tr("Start Server"));
        startServer->setEnabled(true);
        statusLabel->setText("");
        cancel->setEnabled(false);
    });

    layout->addWidget(ipLabel);
    layout->addWidget(statusLabel);
    layout->addWidget(startServer);
    layout->addWidget(cancel);
}

void MainWindow::setupClient(QWidget* clientWidget)
{
    QVBoxLayout* layout = static_cast<QVBoxLayout *>(clientWidget->layout());
    QLabel* ipLabel = new QLabel(this);
    ipLabel->setText(tr("Local IP:%1").arg(localAddress().toString()));
    QLineEdit* serverIp = new QLineEdit(tr("127.0.0.1"), this);
    QPushButton* connectToServer = new QPushButton(tr("Connect To Server"), this);
    connect(connectToServer, &QPushButton::clicked, [=]{
        if (!client) {
            client = new GomokuClient(serverIp->text());
        }
        client->start();
    });
    layout->addWidget(ipLabel);
    layout->addWidget(serverIp);
    layout->addWidget(connectToServer);
}

QHostAddress MainWindow::localAddress()
{
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol
                && address != QHostAddress(QHostAddress::LocalHost)) {
            qDebug() << address.toString();
            return address;
        }
    }
    return QHostAddress(QHostAddress::LocalHost);
}
