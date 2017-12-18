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

void TcpConnect::setPinPass(qint16 pin_code)
{
    m_pinPass = pin_code;
    emit pinPassChanged();
}

void TcpConnect::interpreterService()
{
    busy_changed(true);
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
    busy_changed(false);
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
    clients.append(qMakePair(std::move(_client), false));

    emit devicesChanged();
    emit list_devicesChanged();
    notification_changed(QString("Um novo dispositivo conectado: %1").arg(_client->peerAddress().toString()));
}

void TcpConnect::prepare_current_time_toSend(QTcpSocket *&_client)
{
    _client->write(QString("[{\"current_time\":{\"time\":%1}}]").arg(QDateTime::currentDateTimeUtc().toTime_t()).toUtf8());
    _client->flush();
}

void TcpConnect::server_readSocket()
{
    for (qint32 it = 0; it < clients.length(); ++it) {
        if (clients[it].first->bytesAvailable()) {
            clients[it].first->flush();
            QByteArray data = clients[it].first->readAll();
            QJsonParseError parseError;
            QJsonDocument doc_tcp = QJsonDocument::fromJson(data, &parseError);

            if (parseError.error == 0) {
                for (const QJsonValue &json_object : doc_tcp.array()) {
                    server_parseCommand(json_object.toObject(), it);
                }
            }
        }
    }
}

void TcpConnect::server_sendIfTimerRunning(const QJsonArray data)
{
    QJsonDocument document(data);
    for (const qint32 it : m_list_send_timerIfRunning) {
        if (clients[it].second) {
            clients[it].first->flush();
            clients[it].first->write(document.toJson(QJsonDocument::Compact));
            m_list_send_timerIfRunning.removeAt(it);
        }
    }
}

void TcpConnect::server_parseCommand(const QJsonObject obj, qint32 it)
{
    for (const QString &key : obj.keys()) {
        if (key == "connect") {
            server_validateUser(obj.value(key).toObject(), it);
        }
    }
}

void TcpConnect::server_validateUser(const QJsonObject obj, qint32 it)
{
    if (obj.value("pin").toInt() == m_pinPass) {
        clients[it].second = true;
        m_list_send_timerIfRunning.append(it);
        emit send_TimerIfRunning();
    } else
        clients[it].first->close();
}

void TcpConnect::server_writeSocket(const QJsonArray data)
{
    QJsonDocument document(data);
    for (auto v_client : clients) {
        if (v_client.second == true) {
            v_client.first->flush();
            v_client.first->write(document.toJson(QJsonDocument::Compact));
        }
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
        if (v_client.first->state() == QTcpSocket::ConnectedState && i == index) {
            v_client.first->close();
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
        if (v_client.first->state() != QTcpSocket::ConnectedState) {
            notification_changed(QString("Dispositivo desconectado: %1").arg(v_client.first->peerAddress().toString()));
            v_client.first->close();
            v_client.first->deleteLater();
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
        if (v_client.first->state() == QTcpSocket::ConnectedState) {
            v_client.first->close();
            v_client.first->deleteLater();
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
    busy_changed(true);
    if (m_addressController != "" && m_service_type == RECEIVER)
        client->connectToHost(m_addressController, controller_port);
    busy_changed(false);
}

void TcpConnect::client_connectedController()
{
    client_state = true;
    emit receiver_connectChanged();
    client->write(QString("[{\"connect\":{\"pin\":%1}}]").arg(m_pinController).toUtf8());
    client->flush();
}

void TcpConnect::client_disconnectController()
{
    busy_changed(true);
    if (client->isOpen()) {
        notification_changed("Este dispositivo está desconectado");
        client->close();
    }
    client_state = false;
    emit receiver_connectChanged();
    busy_changed(false);
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

void TcpConnect::setPinController(const qint16 pin)
{
    m_pinController = pin;
    emit pinControllerChanged();
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

void TcpConnect::busy_changed(bool _busy)
{
    m_busy = _busy;
    emit busyChanged();
}

void TcpConnect::notification_changed(QString _notification)
{
    m_notification = _notification;
    emit notificationChanged();
}

QList<QVariant> TcpConnect::list_devices()
{
    m_list_devices.clear();
    for (const auto &v_client : clients) {
        QJsonObject obj;
        obj.insert("ip", v_client.first->peerAddress().toString());
        m_list_devices.append(obj.toVariantMap());
    }
    return m_list_devices;
}


