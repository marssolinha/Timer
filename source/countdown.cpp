#include "countdown.h"

Countdown::Countdown(QObject *parent) :
    QObject(parent), timer(new QTimer()), realtime(new QTimer())
{
    connect(this, SIGNAL(timeChanged()), this, SLOT(TimerStart()));
    connect(timer, SIGNAL(timeout()), this, SLOT(Timer()));
    //connect(realtime, SIGNAL(timeout()), this, SLOT(realtTime()));
    //realtime->start(15);
}

void Countdown::setTimeString(QString get_time)
{
    m_time = QDateTime::fromString(QString("1970-01-01 %1 -00").arg(get_time), Qt::ISODate).toTime_t();
    m_time_start = QDateTime::currentDateTimeUtc().toTime_t();
    m_time_end = m_time + m_time_start;
    timer->setInterval(40);
    prepareStartTime();
}

void Countdown::getCurrentTimeController(QJsonObject obj_time)
{
    m_current_time = QDateTime::currentDateTimeUtc().toTime_t();
    m_current_time_controller = obj_time.value("time").toInt();
    m_diff = m_current_time - m_current_time_controller;
}

void Countdown::setTimeFromController(QJsonObject get_timer)
{
    m_time = get_timer.value("time").toInt();
    m_time_start = get_timer.value("time_start").toInt();
    m_time_end = get_timer.value("time_end").toInt();
    m_time_alert = get_timer.value("time_alert").toInt();

    emit timeChanged();
    emit time_alertChanged();
}

void Countdown::setTime_alert(qint32 quint_time)
{
    m_time_alert = quint_time;
    emit time_alertChanged();
}

void Countdown::prepareStartTime()
{
    QJsonObject obj;
    m_object_time = QJsonObject{};
    m_object_time.insert("time", (int) m_time);
    m_object_time.insert("time_start", (int) m_time_start);
    m_object_time.insert("time_end", (int) m_time_end);
    m_object_time.insert("time_alert", (int) m_time_alert);
    obj.insert("start", m_object_time);
    m_object_time = obj;

    emit send_timerChanged();
    emit timeChanged();
}

void Countdown::prepareRequestTimerIfRunning()
{
    if (!m_status_timer)
        return;
    emit send_timerIfRunningChanged();
}

void Countdown::prepareStopTime()
{
    m_object_command = {};
    m_object_command.insert("action", "stop");
    emit send_commandChanged();
    TimerStop();
}

void Countdown::preparePauseTime()
{
    m_object_command = {};
    m_object_command.insert("action", "pause");
    emit send_commandChanged();
    TimerPause();
}

void Countdown::prepareResumeTime()
{
    m_object_command = {};
    m_object_command.insert("action", "resume");
    emit send_commandChanged();
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

void Countdown::timeToString(qint32 get_time)
{
    m_convert_hours = QDateTime::fromTime_t(get_time).toUTC().toString("hh");
    m_convert_minutes = QDateTime::fromTime_t(get_time).toUTC().toString("mm");
    m_convert_seconds = QDateTime::fromTime_t(get_time).toUTC().toString("ss");

    emit convertHoursChanged();
    emit convertMinutesChanged();
    emit convertSecondsChanged();
}

void Countdown::Timer()
{
    //m_timer = m_time - (qint32)((qint64)m_time_end - QDateTime::currentDateTimeUtc().toTime_t() - m_diff);
    m_timer = (QDateTime::currentDateTimeUtc().toTime_t() - m_diff) - m_time_start;
    if ((m_time - m_timer) < m_time_alert) {
        m_alert = true;
        emit alertChanged();
    }
    emit hoursChanged();
    emit minutesChanged();
    emit secondsChanged();
}

void Countdown::TimerStart()
{
    timer->start();
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
    m_alert = false;

    emit alertChanged();
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

void Countdown::realtTime()
{
    m_real_time = QDateTime::currentDateTimeUtc().toString("dd/MM/yyyy hh:mm:ss:zzz");
    emit getRealTimeChanged();
}
