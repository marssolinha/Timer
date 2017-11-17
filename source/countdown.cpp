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
    m_time_start = QDateTime::currentDateTime().toTime_t();
    m_time_end = m_time + m_time_start;
    prepareStartTime();
}

void Countdown::setTimeFromController(QJsonObject get_timer)
{
    m_time = get_timer.value("time").toInt();
    //m_time_start = QDateTime::currentDateTime().toTime_t();
    m_time_start = get_timer.value("time_start").toInt();
    m_time_end = get_timer.value("time_end").toInt();
    timeChanged();
}

void Countdown::prepareStartTime()
{
    QJsonObject obj;
    m_object_time = QJsonObject{};
    m_object_time.insert("time", (int) m_time);
    m_object_time.insert("time_start", (int) m_time_start);
    m_object_time.insert("time_end", (int) m_time_end);
    obj.insert("start", m_object_time);
    m_object_time = obj;

    send_timerChanged();
    timeChanged();
}

void Countdown::prepareStopTime()
{
    m_object_command = {};
    m_object_command.insert("action", "stop");
    send_commandChanged();
    TimerStop();
}

void Countdown::preparePauseTime()
{
    m_object_command = {};
    m_object_command.insert("action", "pause");
    send_commandChanged();
    TimerPause();
}

void Countdown::prepareResumeTime()
{
    m_object_command = {};
    m_object_command.insert("action", "resume");
    send_commandChanged();
    TimerResume();
}

void Countdown::getCommand(QJsonObject get_command)
{
    if (get_command.value("action") == "stop")
        TimerStop();
    if (get_command.value("action") == "pause")
        TimerPause();
    if (get_command.value("action") == "resume")
        TimerResume();
}

void Countdown::timeToString(quint32 get_time)
{
    m_convert_hours = QDateTime::fromTime_t(get_time).toUTC().toString("hh");
    m_convert_minutes = QDateTime::fromTime_t(get_time).toUTC().toString("mm");
    m_convert_seconds = QDateTime::fromTime_t(get_time).toUTC().toString("ss");

    qDebug() << m_convert_hours << m_convert_minutes << m_convert_seconds;

    emit convertHoursChanged();
    emit convertMinutesChanged();
    emit convertSecondsChanged();
}

void Countdown::Timer()
{
    /*if (m_timer >= m_time) {
        TimerStop();
        return;
    }*/
    m_timer = m_time - (m_time_end - QDateTime::currentDateTime().toTime_t());

    emit hoursChanged();
    emit minutesChanged();
    emit secondsChanged();
}

void Countdown::TimerStart()
{
    timer->start(999);
    m_status_timer = true;
    status_timerChanged();
}

void Countdown::TimerStop()
{
    timer->stop();
    m_timer = 0;
    m_time = 0;
    m_status_timer = false;
    m_timer_pause = false;

    emit timerPauseChanged();
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
