#include "hostinfo.h"

HostInfo::HostInfo(QObject *parent) : QObject(parent)
{
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    for (auto &iface : interfaces) {
        if (iface.name().indexOf("wlan") >= 0 || iface.name().indexOf("wlp") >= 0)
            interface = iface.name();
    }
}

QString HostInfo::getAddress()
{
    QNetworkInterface iface = QNetworkInterface::interfaceFromName(interface);
    QList<QNetworkAddressEntry> entries = iface.addressEntries();
    if (!entries.isEmpty())
        return iface.addressEntries().first().ip().toString();
    return QString("");
}

int HostInfo::getPin()
{
    return m_pin;
}

void HostInfo::generatePin(int time)
{
    qsrand(time);
    m_pin = qrand() % 10000;
    emit pinChanged();
}
