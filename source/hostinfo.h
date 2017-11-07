#ifndef HOSTINFO_H
#define HOSTINFO_H

#include <QObject>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QDebug>
#include <QList>
#include <QJsonObject>

class HostInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int pin READ getPin NOTIFY pinChanged)
    Q_PROPERTY(QString address READ getAddress NOTIFY addressChanged)

public:
    explicit HostInfo(QObject *parent = nullptr);

Q_SIGNALS:
    void pinChanged();
    void addressChanged();

public slots:
    int getPin();
    QString getAddress();

protected:

private:
    QString interface;

};

#endif // HOSTINFO_H
