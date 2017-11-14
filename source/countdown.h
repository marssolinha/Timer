#ifndef TIMER_H
#define TIMER_H

#include <QObject>
#include <QDateTime>
#include <QTimer>
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

public slots:
    inline QString timeString() { return m_time_string; }
    inline quint32 time() { return m_time; }
    inline bool status_timer() { return m_status_timer; }

    void setTimeString(QString get_time);
    void TimerStop();
    void TimerPause();
    void TimerResume();

private slots:
    void Timer();
    void TimerStart();

private:
    QString m_time_string;
    quint32 m_time;
    bool m_status_timer = false;
    bool m_timer_pause = false;

    inline QString hours() { return QDateTime::fromTime_t(m_timer).toUTC().toString("hh"); }
    inline QString minutes() { return QDateTime::fromTime_t(m_timer).toUTC().toString("mm"); }
    inline QString seconds() { return QDateTime::fromTime_t(m_timer).toUTC().toString("ss"); }
    inline bool timerPause() { return m_timer_pause; }

    QTimer *timer;
    quint32 m_timer = 0;
};

#endif // TIMER_H
