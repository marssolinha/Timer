#ifndef TCPCONNECT_H
#define TCPCONNECT_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QList>
#include <QDebug>

class TcpConnect : public QObject
{
    Q_OBJECT
public:
    explicit TcpConnect(QObject *parent = nullptr);

signals:

public slots:

protected slots:
    void server_incomingConnect();
    void server_readyRead();
    void server_disconnectSocket();

private slots:
    QTcpServer *server;
    QList<QTcpSocket *> clients;

    const quint16 controller_port = 4191;
    const quint16 receiver_port = 8191;
};

#endif // TCPCONNECT_H
