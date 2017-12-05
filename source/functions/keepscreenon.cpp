#include "keepscreenon.h"
#include <QDebug>

KeepScreenOn::KeepScreenOn(QObject *parent) : QObject(parent)
{
    qDebug() << "KeepScreen desktop";
}

void KeepScreenOn::on()
{

}

void KeepScreenOn::off()
{

}
