#ifndef TIMER_H
#define TIMER_H

#include <QObject>
#include <QDateTime>
#include <QDebug>

class Countdown : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString time READ time WRITE setTime NOTIFY timeChanged)

public:
    explicit Countdown(QObject *parent = nullptr);

signals:
    void timeChanged();

public slots:
    inline QString time() { return m_time_string; }
    void setTime(QString get_time);

private:
    QString m_time_string;
};

#endif // TIMER_H
