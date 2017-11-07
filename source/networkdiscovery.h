#ifndef NETWORKDISCOVERY_H
#define NETWORKDISCOVERY_H

#include <QObject>
#include <QDebug>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QVariant>
#include <QJsonObject>

class NetworkDiscovery : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString device READ device WRITE setDevice NOTIFY deviceChanged)
    Q_PROPERTY(quint8 type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QList<QVariant> controller READ controller WRITE clearController NOTIFY controllerChanged)

public:
    explicit NetworkDiscovery(QObject *parent = nullptr);

Q_SIGNALS:
    void typeChanged();
    void controllerChanged();
    void deviceChanged();

public slots:
    void sendRequestController();
    void setType(quint8 l_type);
    void setDevice(QString l_device);
    void clearController(QList<QVariant>);

private slots:
    void readSocketController();
    void readSocketClient();
    void sendReponseClient(QHostAddress addr);

private:
    QUdpSocket *udpClient, *udpServer;
    const quint16 controller_port = 4191;
    const quint16 receiver_port = 8191;

    quint8 _type;
    quint8 type() { return _type; }

    QString _device = "";
    QString device() { return _device; }

    QList<QVariant> _controller;
    QList<QVariant> controller() { return _controller; }
};

#endif // NETWORKDISCOVERY_H
