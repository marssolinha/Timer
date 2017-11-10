#include "tcpconnect.h"

TcpConnect::TcpConnect(QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);
    client = new QTcpSocket(this);

    connect(this, SIGNAL(serviceTypeChanged()), this, SLOT(interpreterService()));
    connect(server, SIGNAL(newConnection()), this, SLOT(server_incomingConnect()));
    connect(client, SIGNAL(disconnected()), this, SLOT(client_disconnected()));
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

/*
 * Controller functions
 */

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

    qDebug() << "New connection";
    clients.append(_client);
}

void TcpConnect::server_readyRead()
{
    for (auto v_client : clients) {
        if (v_client->bytesAvailable()) {
            v_client->flush();
            QByteArray data = v_client->readAll();
            QJsonParseError parseError;
            QJsonDocument doc_tcp = QJsonDocument::fromJson(data, &parseError);

            if (parseError.error == 0) {
                for (const QJsonValue &json_object : doc_tcp.array()) {
                    qDebug() << json_object.toObject();
                }
            }
        }
    }
}

void TcpConnect::server_disconnectSocket()
{
    int i = 0;
    for (auto v_client : clients) {
        if (v_client->state() != QTcpSocket::ConnectedState) {
            v_client->close();
            clients.removeAt(i);
        }
        ++i;
    }
}

/*
 * Receiver functions
 */

void TcpConnect::client_connectController()
{
    if (_addressController != "") {
        client->connectToHost(_addressController, controller_port);
    }
}

void TcpConnect::client_disconnectController()
{
    if (client->isOpen()) {
        client->close();
    }
}

void TcpConnect::client_disconnected()
{
    qDebug() << "Disconnect";
    client->deleteLater();
}

void TcpConnect::client_readySocket()
{
    client->flush();
    while (client->canReadLine()) {
        QByteArray data = client->readLine();
        QJsonParseError parserError;
        QJsonDocument doc_tcp = QJsonDocument::fromJson(data, &parserError);

        if (parserError.error == 0) {
            for (const QJsonValue &json_object : doc_tcp.array()) {
                qDebug() << json_object.toObject();
            }
        }
    }
}

void TcpConnect::setAddressController(QString address)
{
    _addressController = address;
    emit addressControllerChanged();
}
