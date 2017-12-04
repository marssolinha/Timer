#include "tcpconnect.h"

TcpConnect::TcpConnect(QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);
    client = new QTcpSocket(this);

    connect(this, SIGNAL(serviceTypeChanged()), this, SLOT(interpreterService()));
    connect(server, SIGNAL(newConnection()), this, SLOT(server_incomingConnect()));
    connect(client, SIGNAL(connected()), this, SLOT(client_connectedController()));
    connect(client, SIGNAL(readyRead()), this, SLOT(client_readSocket()));
    connect(client, SIGNAL(disconnected()), this, SLOT(client_disconnectController()));
}

void TcpConnect::setServiceType(quint16 _type)
{
    m_service_type = _type;
    emit serviceTypeChanged();
}

void TcpConnect::interpreterService()
{
    switch (m_service_type) {
    case RECEIVER:
        server_disconnectAllSockets();
        server_stop();
        break;
    case CONTROLLER:
        client_disconnectController();
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
    connect(_client, SIGNAL(readyRead()), this, SLOT(server_readSocket()));
    connect(_client, SIGNAL(disconnected()), this, SLOT(server_disconnectSocket()));

    prepare_current_time_toSend(_client);
    clients.append(std::move(_client));
    emit devicesChanged();
    emit list_devicesChanged();
    m_list_send_timerIfRunning.append(clients.length() -1);
    emit send_TimerIfRunning();
}

void TcpConnect::prepare_current_time_toSend(QTcpSocket *&_client)
{
    _client->write(QString("[{\"current_time\":{\"time\":%1}}]").arg(QDateTime::currentDateTimeUtc().toTime_t()).toUtf8());
    _client->flush();
}

void TcpConnect::server_readSocket()
{
    for (auto v_client : clients) {
        if (v_client->bytesAvailable()) {
            v_client->flush();
            QByteArray data = v_client->readAll();
            QJsonParseError parseError;
            QJsonDocument doc_tcp = QJsonDocument::fromJson(data, &parseError);

            if (parseError.error == 0) {
                for (const QJsonValue &json_object : doc_tcp.array()) {
                    server_parseCommand(json_object.toObject());
                }
            }
        }
    }
}

void TcpConnect::server_sendIfTimerRunning(const QJsonArray data)
{
    QJsonDocument document(data);
    for (const qint32 it : m_list_send_timerIfRunning) {
        clients[it]->flush();
        clients[it]->write(document.toJson(QJsonDocument::Compact));
    }
    m_list_send_timerIfRunning.clear();
}

void TcpConnect::server_parseCommand(const QJsonObject obj)
{
    for (const QString &key : obj.keys()) {
        qDebug() << key;
    }
}

void TcpConnect::server_writeSocket(const QJsonArray data)
{
    QJsonDocument document(data);
    for (auto v_client : clients) {
        v_client->flush();
        v_client->write(document.toJson(QJsonDocument::Compact));
    }
}

void TcpConnect::setData_send(QJsonObject obj)
{
    QJsonArray array_send;
    array_send.append(obj);
    server_writeSocket(array_send);
}

void TcpConnect::setData_sendIfRunning(QJsonObject obj)
{
    QJsonArray array_send;
    array_send.append(obj);
    server_sendIfTimerRunning(array_send);
}

void TcpConnect::server_disconnectClient(qint32 index)
{
    qint32 i = 0;
    for (auto v_client : clients) {
        if (v_client->state() == QTcpSocket::ConnectedState && i == index) {
            v_client->close();
            clients.removeAt(i);
        }
        ++i;
    }
}

void TcpConnect::connectToController(QVariantMap variant)
{
    QString address = variant.value("address").toString();
    if (address == m_local_addr)
        return;
    setAddressController(address);
    setNameController(variant.value("device").toString());
    client_connectController();
}

void TcpConnect::server_disconnectSocket()
{
    qint32 i = 0;
    for (auto v_client : clients) {
        if (v_client->state() != QTcpSocket::ConnectedState) {
            v_client->close();
            v_client->deleteLater();
            clients.removeAt(i);
        }
        ++i;
    }
    emit devicesChanged();
    emit list_devicesChanged();
}

void TcpConnect::server_disconnectAllSockets()
{
    for (auto v_client : clients) {
        if (v_client->state() == QTcpSocket::ConnectedState) {
            v_client->close();
            v_client->deleteLater();
        }
    }
    clients.clear();
    emit devicesChanged();
    emit list_devicesChanged();
}

/*
 * Receiver functions
 */

void TcpConnect::client_connectController()
{
    if (m_addressController != "" && m_service_type == RECEIVER)
        client->connectToHost(m_addressController, controller_port);
}

void TcpConnect::client_connectedController()
{
    client_state = true;
    emit receiver_connectChanged();
}

void TcpConnect::client_disconnectController()
{
    if (client->isOpen())
        client->close();
    client_state = false;
    emit receiver_connectChanged();
}

void TcpConnect::client_disconnected()
{
    client->deleteLater();
}

void TcpConnect::client_readSocket()
{
    client->flush();
    while (client->bytesAvailable()) {
        QByteArray data = client->readLine();
        QJsonParseError parserError;
        QJsonDocument doc_tcp = QJsonDocument::fromJson(data, &parserError);

        if (parserError.error == 0) {
            for (const QJsonValue &json_object : doc_tcp.array()) {
                client_parseCommand(json_object.toObject());
            }
        }
    }
}

void TcpConnect::client_parseCommand(const QJsonObject obj)
{
    for (const QString &key : obj.keys()) {
        if (key == "current_time") {
            m_receive_current_time_controller = obj.value(key).toObject();
            emit receive_current_time_controllerChanged();
        } else if (key == "start") {
            m_receive_timer = obj.value(key).toObject();
            emit receive_timerChanged();
        } else if (key == "action") {
            m_receive_command = obj;
            emit receive_commandChanged();
        }
    }
}

void TcpConnect::client_writeSocket(const QJsonArray data)
{
    QJsonDocument document(data);
    if (client->waitForConnected())
        client->write(document.toJson(QJsonDocument::Compact));
}

void TcpConnect::setAddressController(const QString address)
{
    m_addressController = address;
    emit addressControllerChanged();
}

void TcpConnect::setNameController(const QString name)
{
    m_nameController = name;
    emit nameControllerChanged();
}

void TcpConnect::setLocal_addr(const QString address)
{
    m_local_addr = address;
    emit local_addrChanged();
}

QList<QVariant> TcpConnect::list_devices()
{
    m_list_devices.clear();
    for (const auto &v_client : clients) {
        QJsonObject obj;
        obj.insert("ip", v_client->peerAddress().toString());
        m_list_devices.append(obj.toVariantMap());
    }
    return m_list_devices;
}


