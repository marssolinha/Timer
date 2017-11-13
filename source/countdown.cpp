#include "countdown.h"

Countdown::Countdown(QObject *parent) : QObject(parent)
{

}

void Countdown::setTime(QString get_time)
{
    //QDateTime time = QDateTime::fromString(QString("1970-01-01T%1").arg(get_time), Qt::ISODate);
    QDateTime time;
    time = QDateTime::fromString(QString("1970-01-01T00:00:00"), Qt::ISODate);
    qDebug() << time.toUTC().toTime_t();

    m_time_string = get_time;
    emit timeChanged();
}
