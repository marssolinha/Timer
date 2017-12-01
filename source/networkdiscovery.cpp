#include "networkdiscovery.h"

NetworkDiscovery::NetworkDiscovery(QObject *parent) : QObject(parent)
{
    connect(this, SIGNAL(typeChanged()), this, SLOT(serviceChange()));
}

void NetworkDiscovery::readSocketController()
{
    if (m_type == 0)
        return;
    QByteArray buffer;
    QHostAddress client_addr;
    quint16 client_port;

    buffer.resize(udpServer->pendingDatagramSize());
    udpServer->readDatagram(buffer.data(), buffer.size(), &client_addr, &client_port);

    if (client_addr.toString().size() > 0)
        this->sendReponseClient(client_addr);
}

void NetworkDiscovery::readSocketClient()
{
    qDebug() << "Read socket client";
    QByteArray buffer;
    QHostAddress _controller_addr;
    quint16 _controller_port;

    buffer.resize(udpClient->pendingDatagramSize());
    udpClient->readDatagram(buffer.data(), buffer.size(), &_controller_addr, &_controller_port);

    QJsonObject obj;
    obj.insert("address", _controller_addr.toString());
    obj.insert("text", QString("%1 (%2)").arg(buffer.data()).arg(_controller_addr.toString()));

    QRegularExpression expression("connect:");
    QRegularExpressionMatch expression_match = expression.match(buffer.data());
    if (expression_match.hasMatch()) {
        obj.insert("device", QString(buffer.data()).replace(QString("connect:"), QString("")));
        m_connect_controller = obj.toVariantMap();
        emit connect_controllerChanged();
    } else {
        obj.insert("device", buffer.data());
        m_controller.append(obj.toVariantMap());
        emit controllerChanged();
    }
}

void NetworkDiscovery::sendReponseClient(QHostAddress addr)
{
    qDebug() << "Send response";
    QByteArray datagram;
    datagram.append(m_device);
    udpClient->writeDatagram(datagram.data(), datagram.size(), addr, receiver_port);
    qDebug() << datagram << addr.toString() << receiver_port;
}

void NetworkDiscovery::sendRequestController()
{
    qDebug() << "Send request";
    QByteArray datagram = "";
    udpClient->writeDatagram(datagram.data(), datagram.size(), QHostAddress::Broadcast, controller_port);
}

void NetworkDiscovery::setType(quint8 _type)
{
    m_type = _type;
    emit typeChanged();
}

void NetworkDiscovery::setDevice(QString _device)
{
    m_device = _device;
    emit deviceChanged();
}

void NetworkDiscovery::clearController(QList<QVariant>)
{
    m_controller.clear();
    emit controllerChanged();
}

void NetworkDiscovery::sendSignalToConnection()
{
    qDebug() << "send signal to receivers connection";
    QByteArray datagram = "connect:";
    datagram.append(m_device);
    udpServer->writeDatagram(datagram.data(), datagram.size(), QHostAddress::Broadcast, receiver_port);
}

void NetworkDiscovery::serviceChange()
{
    switch (m_type) {
    case RECEIVER:
        controller_stop();
        receiver_start();
        break;
    case CONTROLLER:
        receiver_stop();
        controller_start();
        break;
    }
}

void NetworkDiscovery::controller_start()
{
    udpServer = new QUdpSocket(this);
    if (!udpServer->bind(QHostAddress::AnyIPv4, controller_port))
        qDebug() << "UDP Server error:" << udpServer->errorString();
    else
        server_state = true;
    connect(udpServer, SIGNAL(readyRead()), this, SLOT(readSocketController()));
}

void NetworkDiscovery::controller_stop()
{
    if (server_state) {
        udpServer->close();
        delete udpServer;
    }
}

void NetworkDiscovery::receiver_start()
{
    udpClient = new QUdpSocket(this);
    if (!udpClient->bind(QHostAddress::AnyIPv4, receiver_port))
        qDebug() << "UDP Client error: " << udpClient->error() << udpClient->errorString();
    else
        client_state = true;
    connect(udpClient, SIGNAL(readyRead()), this, SLOT(readSocketClient()));
}

void NetworkDiscovery::receiver_stop()
{
    if (client_state) {
        udpClient->close();
        delete udpClient;
    }
}
