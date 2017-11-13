#include "countdown.h"

Countdown::Countdown(QObject *parent) : QObject(parent)
{

}

void Countdown::setTime(QString get_time)
{
    QDateTime time = QDateTime::fromString(QString("1970-01-01 %1 -00").arg(get_time), Qt::ISODate);
    qDebug() << time.toUTC().toTime_t();

    m_time_string = get_time;
    emit timeChanged();
}
