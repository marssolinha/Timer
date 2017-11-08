#include "tcpconnect.h"

TcpConnect::TcpConnect(QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(server_incomingConnect()));

    if (!server->listen(QHostAddress::AnyIPv4, controller_port)) {
        qDebug() << "Server error:" << server->errorString();
    }
}

void TcpConnect::server_incomingConnect()
{
    QTcpSocket *client = server->nextPendingConnection();
    connect(client, SIGNAL(readyRead()), this, SLOT(server_readyRead()));
    connect(client, SIGNAL(disconnected()), this, SLOT(server_disconnectSocket()));

    clients.append(client);
}

void TcpConnect::server_readyRead()
{
    for (auto client : clients) {
        if (client->bytesAvailable()) {
            client->flush();
            QByteArray data = client->readAll();
            QJsonParseError parseError;
            QJsonDocument doc_tcp = QJsonDocument::fromJson(data, &parseError);
        }
    }
}

void TcpConnect::server_disconnectSocket()
{
    int i = 0;
    for (auto client : clients) {
        if (client->state() != QTcpSocket::ConnectedState) {
            client->close();
            clients.removeAt(i);
        }
        ++i;
    }
}
