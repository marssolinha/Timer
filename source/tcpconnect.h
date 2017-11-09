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
    Q_PROPERTY(quint16 serviceType READ serviceType WRITE setServiceType NOTIFY serviceTypeChanged)

public:
    /**
     * @brief TcpConnect
     * @param parent
     */
    explicit TcpConnect(QObject *parent = nullptr);

Q_SIGNALS:
    /**
     * @brief serviceTypeChanged
     */
    void serviceTypeChanged();

public slots:

protected slots:
    /**
     * @brief server_start
     */
    void server_start();
    /**
     * @brief server_stop
     */
    void server_stop();

private slots:
    /**
     * @brief setServiceType
     * @param _type
     */
    void setServiceType(quint16 _type);
    /**
     * @brief interpreterService
     */
    void interpreterService();

    /**
     * @brief server_incomingConnect
     */
    void server_incomingConnect();
    /**
     * @brief server_readyRead
     */
    void server_readyRead();
    /**
     * @brief server_disconnectSocket
     */
    void server_disconnectSocket();

private:
    QTcpServer *server;
    QList<QTcpSocket *> clients;

    const quint16 controller_port = 4191;
    const quint16 receiver_port = 8191;

    quint16 l_service_type;
    /**
     * @brief serviceType
     * @return
     */
    quint16 serviceType() { return l_service_type; }

    enum {
        RECEIVER = 0,
        CONTROLLER
    };

    bool server_state;
};

#endif // TCPCONNECT_H
