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
    Q_PROPERTY(QString local_addr READ local_addr WRITE setLocal_addr NOTIFY local_addrChanged)
    Q_PROPERTY(bool receiver_connect READ receiver_connect NOTIFY receiver_connectChanged)
    Q_PROPERTY(QJsonObject receive_timer READ receive_timer NOTIFY receive_timerChanged)
    Q_PROPERTY(QJsonObject receive_command READ receive_command NOTIFY receive_commandChanged)
    Q_PROPERTY(qint32 devices READ devices NOTIFY devicesChanged)
    Q_PROPERTY(QList<QVariant> list_devices READ list_devices NOTIFY list_devicesChanged)

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
    /**
     * @brief data_sendChanged
     */
    void data_sendChanged();
    /**
     * @brief receive_timerChanged
     */
    void receive_timerChanged();
    /**
     * @brief receiver_commandChanged
     */
    void receive_commandChanged();
    /**
     * @brief devicesChanged
     */
    void devicesChanged();
    /**
     * @brief list_devicesChanged
     */
    void list_devicesChanged();
    /**
     * @brief local_addrChanged
     */
    void local_addrChanged();

public slots:
    /**
     * @brief setData_send
     * @param obj
     */
    void setData_send(QJsonObject obj);
    /**
     * @brief server_disconnectClient
     * @param index
     */
    void server_disconnectClient(qint32 index);
    /**
     * @brief connectToController
     * @param variant
     */
    void connectToController(QVariantMap variant);

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
     * @brief server_readSocket
     */
    void server_readSocket();
    /**
     * @brief server_parseCommand
     */
    void server_parseCommand(const QJsonObject obj);
    /**
     * @brief server_writeSocket
     */
    void server_writeSocket(const QJsonArray data);
    /**
     * @brief server_disconnectSocket
     */
    void server_disconnectSocket();
    /**
     * @brief server_disconnectAllSockets
     */
    void server_disconnectAllSockets();
    /**
     * @brief client_readSocket
     */
    void client_readSocket();
    /**
     * @brief client_parseCommand
     */
    void client_parseCommand(const QJsonObject obj);
    /**
     * @brief client_writeSocket
     */
    void client_writeSocket(const QJsonArray data);
    /**
     * @brief setAddressController
     * @param address
     */
    void setAddressController(QString address);
    void setLocal_addr(QString address);

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

    QString m_addressController;
    QString addressController() { return m_addressController; }

    QJsonObject m_receive_timer = {};
    inline QJsonObject receive_timer() { return m_receive_timer; }

    QJsonObject m_receive_command = {};
    inline QJsonObject receive_command () { return m_receive_command; }

    qint32 devices() { return clients.size(); }

    QList<QVariant> m_list_devices;
    QList<QVariant> list_devices();

    QString m_local_addr;
    QString local_addr() { return m_local_addr; }
};

#endif // TCPCONNECT_H
