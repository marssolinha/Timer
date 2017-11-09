#include "tcpconnect.h"

TcpConnect::TcpConnect(QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(server_incomingConnect()));

    /*if (!server->listen(QHostAddress::AnyIPv4, controller_port)) {
        qDebug() << "Server error:" << server->errorString();
    } else {
        server_state = true;
    }*/

    connect(this, SIGNAL(serviceTypeChanged()), this, SLOT(interpreterService()));
}

void TcpConnect::setServiceType(quint16 _type)
{
    l_service_type = _type;
    emit serviceTypeChanged();
}

void TcpConnect::interpreterService()
{
    switch (l_service_type) {
    case RECEIVER:
        server_stop();
        break;
    case CONTROLLER:
        server_start();
        break;
    }
}

void TcpConnect::server_start()
{
    server = new QTcpServer();
    connect(server, SIGNAL(newConnection()), this, SLOT(server_incomingConnect()));

    if (!server->listen(QHostAddress::AnyIPv4, controller_port)) {
        qDebug() << "Server error:" << server->errorString();
    } else {
        server_state = true;
    }
}

void TcpConnect::server_stop()
{
    if (server_state) {
        disconnect(server, SIGNAL(newConnection()), this, SLOT(server_incomingConnect()));
        server->close();
        server_state = false;
    }
    clients.clear();
}

void TcpConnect::server_incomingConnect()
{
    QTcpSocket *_client = server->nextPendingConnection();
    connect(_client, SIGNAL(readyRead()), this, SLOT(server_readyRead()));
    connect(_client, SIGNAL(disconnected()), this, SLOT(server_disconnectSocket()));

    clients.append(_client);
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
