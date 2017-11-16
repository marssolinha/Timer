#ifndef TIMER_H
#define TIMER_H

#include <QObject>
#include <QDateTime>
#include <QTimer>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

class Countdown : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString timeString READ time WRITE setTimeString NOTIFY timeStringChanged)
    Q_PROPERTY(quint32 time READ time NOTIFY timeChanged)
    Q_PROPERTY(bool status_timer READ status_timer NOTIFY status_timerChanged)
    Q_PROPERTY(bool timerPause READ timerPause NOTIFY timerPauseChanged)
    Q_PROPERTY(QString hours READ hours NOTIFY hoursChanged)
    Q_PROPERTY(QString minutes READ minutes NOTIFY minutesChanged)
    Q_PROPERTY(QString seconds READ seconds NOTIFY secondsChanged)
    Q_PROPERTY(QJsonObject send_timer READ send_timer NOTIFY send_timerChanged)
    Q_PROPERTY(QJsonObject send_command READ send_command NOTIFY send_commandChanged)

public:
    explicit Countdown(QObject *parent = nullptr);

signals:
    void timeChanged();
    void timeStringChanged();
    void status_timerChanged();
    void timerPauseChanged();
    void hoursChanged();
    void minutesChanged();
    void secondsChanged();
    void send_timerChanged();
    void send_commandChanged();

public slots:
    inline QString timeString() { return m_time_string; }
    inline quint32 time() { return m_time; }
    inline bool status_timer() { return m_status_timer; }

    void setTimeString(QString get_time);
    void setTimeFromController(QJsonObject get_timer);
    void prepareStopTime();
    void preparePauseTime();
    void prepareResumeTime();
    void getCommand(QJsonObject get_command);

private slots:
    void Timer();
    void prepareStartTime();
    void TimerStart();
    void TimerStop();
    void TimerPause();
    void TimerResume();

private:
    QString m_time_string;
    quint32 m_time;
    quint32 m_time_start;
    bool m_status_timer = false;
    bool m_timer_pause = false;

    inline QString hours() { return QDateTime::fromTime_t(m_timer).toUTC().toString("hh"); }
    inline QString minutes() { return QDateTime::fromTime_t(m_timer).toUTC().toString("mm"); }
    inline QString seconds() { return QDateTime::fromTime_t(m_timer).toUTC().toString("ss"); }
    inline bool timerPause() { return m_timer_pause; }
    inline QJsonObject send_timer() { return m_object_time; }
    inline QJsonObject send_command() { return m_object_command; }

    QTimer *timer;
    quint32 m_timer = 0, m_timer_start = 0, m_time_end = 0;

    QJsonObject m_object_time, m_object_command;
};

#endif // TIMER_H
