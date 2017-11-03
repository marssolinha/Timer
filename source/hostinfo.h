#ifndef HOSTINFO_H
#define HOSTINFO_H

#include <QObject>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QDebug>

class HostInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString hostname READ getHostname NOTIFY hostnameChanged)
    Q_PROPERTY(int pin READ getPin NOTIFY pinChanged)
    Q_PROPERTY(QString address READ getAddress NOTIFY addressChanged)

public:
    explicit HostInfo(QObject *parent = nullptr);

Q_SIGNALS:
    void hostnameChanged();
    void pinChanged();
    void addressChanged();

public slots:
    QString getHostname();
    int getPin();
    QString getAddress();

protected:

};

#endif // HOSTINFO_H
