#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QtNetwork>
#include "gomokuwidget.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle(tr("Gomoku"));
    QWidget* centralWidget = new QWidget(this);
    QHBoxLayout* centralLayout = new QHBoxLayout(this);
    centralWidget->setLayout(centralLayout);

    gomoku = new GomokuWidget(this);

    QWidget* sideBar = new QWidget(this);
    QVBoxLayout* lay = new QVBoxLayout(this);
    mTab = new QTabWidget(this);
    lay->addWidget(mTab);
    sideBar->setLayout(lay);

    mServerSerializer = new ProtocolSerializer(this);
    mClientSerializer = new ProtocolSerializer(this);

    setupServer();
    setupClient();

    connect(gomoku, &GomokuWidget::win, [=](bool you) {
        if (you) {
            QMessageBox::information(this, tr("Game finished"), "You are the WINNER!");
        } else {
            QMessageBox::information(this, tr("Game finished"), "You are a loser.");
        }
    });

    centralLayout->addWidget(gomoku, 12);
    centralLayout->addWidget(sideBar, 9);
    setCentralWidget(centralWidget);
    resize(650, 360);
}

void MainWindow::setupServer()
{
    QWidget* serverWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(serverWidget);
    serverWidget->setLayout(layout);
    QLabel* ipLabel = new QLabel(serverWidget);
    QPointer<QLabel> statusLabel = new QLabel(serverWidget);
//    QLineEdit* nick = new QLabel(this);
//    nick->setPlaceholderText(tr("Nickname"));
    ipLabel->setText(tr("Local IP:%1").arg(localAddress().toString()));
    QPointer<QPushButton> cancel = new QPushButton(tr("Stop Server"), serverWidget);
    cancel->setEnabled(false);
    QPointer<QPushButton> startServer = new QPushButton(tr("Start Server"), serverWidget);
    connect(startServer, &QPushButton::clicked, [=]{

        statusLabel->setText("");
        if (mServer) {
            mServer.data()->deleteLater();
        }
        mServer = new GomokuServer();
        mServer->setSerializer(mServerSerializer);
        connect(mServer.data(), &GomokuServer::error, [=](const QString error){
//            cancel->setEnabled(true);
            statusLabel->setText(error);
            startServer->setText(tr("Start Server"));
            startServer->setEnabled(true);
            cancel->setEnabled(false);
        });
        connect(mServer.data(), &GomokuServer::connected, [=](const QString peer){
            statusLabel->setText(tr("Client %1 Connected.").arg(peer));
            startServer->setText(tr("Connected"));
            gomoku->setInitColor(Pawn::BLACK);
            connect(gomoku, &GomokuWidget::move, mServer, &GomokuServer::sendMove);
            connect(mServer, &GomokuServer::newMove, gomoku, &GomokuWidget::positionPawn);
        });

        connect(mServer.data(), &GomokuServer::disconnected, [=]{
            if (statusLabel) {
                statusLabel->setText(tr("Disconnected."));
            }
            if (startServer) {
                startServer->setText(tr("Start Server"));
                startServer->setEnabled(true);
            }
            if (cancel) {
                cancel->setEnabled(false);
            }
        });
        startServer->setText(tr("Listening..."));
        startServer->setEnabled(false);
        cancel->setEnabled(true);
        mServer.data()->start();

    });

    connect(cancel, &QPushButton::clicked, [=]{
        if (mServer) {
            mServer.data()->deleteLater();
        }
        startServer->setText(tr("Start Server"));
        startServer->setEnabled(true);
        statusLabel->setText("");
        cancel->setEnabled(false);
    });

    layout->addWidget(ipLabel);
    layout->addWidget(statusLabel);
//    layout->addWidget(nick);
    layout->addWidget(startServer);
    layout->addWidget(cancel);

    mTab->addTab(serverWidget, tr("Server"));
}

void MainWindow::setupClient()
{
    QWidget* clientWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(clientWidget);
    QLabel* ipLabel = new QLabel(clientWidget);
    ipLabel->setText(tr("Local IP:%1").arg(localAddress().toString()));
    QLineEdit* serverIp = new QLineEdit(tr("127.0.0.1"), clientWidget);
    QPointer<QLabel> statusLabel = new QLabel(clientWidget);
    QPointer<QPushButton> connectToServer = new QPushButton(tr("Connect To Server"), clientWidget);
    QPointer<QPushButton> cancel = new QPushButton(tr("Disconnect"), clientWidget);
    cancel->setEnabled(false);
    connect(connectToServer, &QPushButton::clicked, [=]{
        if (mClient) {
            mClient.data()->deleteLater();
        }
        statusLabel->setText("");
        mClient = new GomokuClient(serverIp->text());
        mClient->setSerializer(mClientSerializer);

        connect(mClient.data(), &GomokuClient::connected, [=]{
            connectToServer->setText(tr("Connected"));
            gomoku->setInitColor(Pawn::WHITE);
            connect(gomoku, &GomokuWidget::move, mClient, &GomokuAbsHost::sendMove);
            connect(mClient, &GomokuClient::newMove, gomoku, &GomokuWidget::positionPawn);
        });

        connect(mClient.data(), &GomokuClient::error, [=](const QString err){
            statusLabel->setText(err);
            cancel->setEnabled(false);
            connectToServer->setEnabled(true);
            connectToServer->setText(tr("Connect To Server"));
        });

        connect(mClient.data(), &GomokuClient::disconnected, [=]{
            qDebug() << "disconn";
           if (!statusLabel || !cancel || !connectToServer) {
               return;
           }
           statusLabel->setText("Disconnected.");
           cancel->setEnabled(false);
           connectToServer->setEnabled(true);
           connectToServer->setText(tr("Connect To Server"));
        });

        connectToServer->setText(tr("Connecting..."));
        connectToServer->setEnabled(false);
        cancel->setEnabled(true);
        mClient.data()->start();
    });

    connect(cancel, &QPushButton::clicked, [=]{
        if (mClient) {
            mClient.data()->deleteLater();
        }
        connectToServer->setText(tr("Connect To Server"));
        connectToServer->setEnabled(true);
        cancel->setEnabled(false);
    });

    layout->addWidget(ipLabel);
    layout->addWidget(statusLabel);
    layout->addWidget(serverIp);
    layout->addWidget(connectToServer);
    layout->addWidget(cancel);

    mTab->addTab(clientWidget, tr("Client"));

}

QHostAddress MainWindow::localAddress()
{
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol
                && address != QHostAddress(QHostAddress::LocalHost)) {
            return address;
        }
    }
    return QHostAddress(QHostAddress::LocalHost);
}
