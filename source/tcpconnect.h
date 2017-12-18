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
#include <QDateTime>

class TcpConnect : public QObject
{
    Q_OBJECT
    Q_PROPERTY(quint16 serviceType READ serviceType WRITE setServiceType NOTIFY serviceTypeChanged)
    Q_PROPERTY(qint16 pinPass READ pinPass WRITE setPinPass NOTIFY pinPassChanged)
    Q_PROPERTY(QString addressController READ addressController WRITE setAddressController NOTIFY addressControllerChanged)
    Q_PROPERTY(qint16 pinController READ pinController WRITE setPinController NOTIFY pinControllerChanged)
    Q_PROPERTY(QString nameController READ nameController WRITE setNameController NOTIFY nameControllerChanged)
    Q_PROPERTY(QString local_addr READ local_addr WRITE setLocal_addr NOTIFY local_addrChanged)
    Q_PROPERTY(bool receiver_connect READ receiver_connect NOTIFY receiver_connectChanged)
    Q_PROPERTY(QJsonObject receive_current_time_controller READ receive_current_time_controller NOTIFY receive_current_time_controllerChanged)
    Q_PROPERTY(QJsonObject receive_timer READ receive_timer NOTIFY receive_timerChanged)
    Q_PROPERTY(QJsonObject receive_command READ receive_command NOTIFY receive_commandChanged)
    Q_PROPERTY(qint32 devices READ devices NOTIFY devicesChanged)
    Q_PROPERTY(QList<QVariant> list_devices READ list_devices NOTIFY list_devicesChanged)

    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged)
    Q_PROPERTY(QString notification READ notification NOTIFY notificationChanged)

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
     * @brief pinPassChanged
     */
    void pinPassChanged();
    /**
     * @brief addressControllerChanged
     */
    void addressControllerChanged();
    /**
     * @brief pinControllerChanged
     */
    void pinControllerChanged();
    /**
     * @brief nameControllerChanged
     */
    void nameControllerChanged();
    /**
     * @brief receiver_connectChanged
     */
    void receiver_connectChanged();
    /**
     * @brief data_sendChanged
     */
    void data_sendChanged();
    /**
     * @brief send_TimerIfRunning
     */
    void send_TimerIfRunning();
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
    /**
     * @brief receive_current_time_controllerChanged
     */
    void receive_current_time_controllerChanged();
    /**
     * @brief busyChanged
     */
    void busyChanged();
    /**
     * @brief notificationChanged
     */
    void notificationChanged();

public slots:
    /**
     * @brief setData_send
     * @param obj
     */
    void setData_send(QJsonObject obj);
    /**
     * @brief setData_sendIfRunning
     * @param obj
     */
    void setData_sendIfRunning(QJsonObject obj);
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
     * @brief setPinPass
     * @param pin_code
     */
    void setPinPass(qint16 pin_code);
    /**
     * @brief interpreterService
     */
    void interpreterService();
    /**
     * @brief server_incomingConnect
     */
    void server_incomingConnect();
    /**
     * @brief prepare_current_time_toSend
     * @param _client
     */
    void prepare_current_time_toSend(QTcpSocket *&_client);
    /**
     * @brief server_readSocket
     */
    void server_readSocket();
    /**
     * @brief server_sendIfTimerRunning
     * @param _client
     */
    void server_sendIfTimerRunning(const QJsonArray data);
    /**
     * @brief server_parseCommand
     */
    void server_parseCommand(const QJsonObject obj, qint32 it);
    /**
     * @brief server_validateUser
     * @param obj
     * @param it
     */
    void server_validateUser(const QJsonObject obj, qint32 it);
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
    void setAddressController(const QString address);
    /**
     * @brief setPinController
     * @param pin
     */
    void setPinController(const qint16 pin);
    /**
     * @brief setNameController
     * @param name
     */
    void setNameController(const QString name);
    /**
     * @brief setLocal_addr
     * @param address
     */
    void setLocal_addr(const QString address);

    /**
     * @brief busy_changed
     * @param _busy
     */
    void busy_changed(bool _busy);
    /**
     * @brief notification_changed
     * @param _notification
     */
    void notification_changed(QString _notification);

private:
    QTcpServer *server;
    QTcpSocket *client;
    QList<QPair<QTcpSocket *, bool>> clients;
    QList<qint32> m_list_send_timerIfRunning;

    const quint16 controller_port = 4191;
    const quint16 receiver_port = 8191;

    quint16 m_service_type;
    quint16 serviceType() { return m_service_type; }

    qint16 m_pinPass = 0;
    qint16 pinPass() { return m_pinPass; }

    enum {
        RECEIVER = 0,
        CONTROLLER
    };

    bool server_state = false, client_state = false;
    bool receiver_connect() { return client_state; }

    QString m_addressController;
    QString addressController() { return m_addressController; }

    qint16 m_pinController;
    qint16 pinController() { return m_pinController; }

    QString m_nameController;
    QString nameController() { return m_nameController; }

    QJsonObject m_receive_current_time_controller = {};
    inline QJsonObject receive_current_time_controller() { return m_receive_current_time_controller; }

    QJsonObject m_receive_timer = {};
    inline QJsonObject receive_timer() { return m_receive_timer; }

    QJsonObject m_receive_command = {};
    inline QJsonObject receive_command () { return m_receive_command; }

    qint32 devices() { return clients.size(); }

    QList<QVariant> m_list_devices;
    QList<QVariant> list_devices();

    QString m_local_addr;
    QString local_addr() { return m_local_addr; }

    bool m_busy = false;
    bool busy() { return m_busy; }

    QString m_notification = "";
    QString notification() { return m_notification; }
};

#endif // TCPCONNECT_H
