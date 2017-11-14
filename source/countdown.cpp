#include "countdown.h"

Countdown::Countdown(QObject *parent) :
    QObject(parent), timer(new QTimer())
{
    connect(this, SIGNAL(timeChanged()), this, SLOT(TimerStart()));
    connect(timer, SIGNAL(timeout()), this, SLOT(Timer()));
}

void Countdown::setTimeString(QString get_time)
{
    m_time = QDateTime::fromString(QString("1970-01-01 %1 -00").arg(get_time), Qt::ISODate).toTime_t();
    timeChanged();
}

void Countdown::Timer()
{
    if (m_timer >= m_time) {
        TimerStop();
        return;
    }
    ++m_timer;

    emit hoursChanged();
    emit minutesChanged();
    emit secondsChanged();
}

void Countdown::TimerStart()
{
    timer->start(1000);
    m_status_timer = true;
    status_timerChanged();
}

void Countdown::TimerStop()
{
    timer->stop();
    m_timer = 0;
    m_time = 0;
    m_status_timer = false;

    emit status_timerChanged();
    emit hoursChanged();
    emit minutesChanged();
    emit secondsChanged();
}

void Countdown::TimerPause()
{
    timer->stop();
    m_timer_pause = true;
    emit timerPauseChanged();
}

void Countdown::TimerResume()
{
    TimerStart();
    m_timer_pause = false;
    emit timerPauseChanged();
}
