#ifndef HOSTINFO_H
#define HOSTINFO_H

#include <QObject>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QDebug>
#include <QList>
#include <QJsonObject>
#include <QTime>

class HostInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int pin READ getPin WRITE generatePin NOTIFY pinChanged)
    Q_PROPERTY(QString address READ getAddress NOTIFY addressChanged)

public:
    explicit HostInfo(QObject *parent = nullptr);

Q_SIGNALS:
    void pinChanged();
    void addressChanged();

public slots:
    void generatePin(int time);
    int getPin();
    QString getAddress();

protected:

private:
    int l_pin;
    QString interface;

};

#endif // HOSTINFO_H
