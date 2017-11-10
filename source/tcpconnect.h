#ifndef TCPCONNECT_H
#define TCPCONNECT_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QList>
#include <QDebug>

class TcpConnect : public QObject
{
    Q_OBJECT
    Q_PROPERTY(quint16 serviceType READ serviceType WRITE setServiceType NOTIFY serviceTypeChanged)
    Q_PROPERTY(QString addressController READ addressController WRITE setAddressController NOTIFY addressControllerChanged)
    Q_PROPERTY(bool receiver_connect READ receiver_connect NOTIFY receiver_connectChanged)

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
    /**
     * @brief addressControllerChanged
     */
    void addressControllerChanged();
    /**
     * @brief receiver_connectChanged
     */
    void receiver_connectChanged();

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
    /**
     * @brief client_connectController
     */
    void client_connectController();
    /**
     * @brief client_connectedController
     */
    void client_connectedController();
    /**
     * @brief client_disconnectController
     */
    void client_disconnectController();
    /**
     * @brief client_disconnected
     */
    void client_disconnected();

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
    /**
     * @brief server_disconnectAllSockets
     */
    void server_disconnectAllSockets();
    /**
     * @brief client_readyRead
     */
    void client_readySocket();

    /**
     * @brief setAddressController
     * @param address
     */
    void setAddressController(QString address);

private:
    QTcpServer *server;
    QTcpSocket *client;
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

    bool server_state = false, client_state = false;
    bool receiver_connect() { return client_state; }

    QString _addressController;
    QString addressController() { return _addressController; }
};

#endif // TCPCONNECT_H
