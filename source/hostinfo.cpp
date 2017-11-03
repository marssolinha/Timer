#include "hostinfo.h"

HostInfo::HostInfo(QObject *parent) : QObject(parent)
{

}

QString HostInfo::getHostname()
{
    return QHostInfo::localHostName();
}

int HostInfo::getPin()
{
    return qrand() % 10000;
}

QString HostInfo::getAddress()
{
    return "";
}
