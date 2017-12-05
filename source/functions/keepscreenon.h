#ifndef KEEPSCREENON_H
#define KEEPSCREENON_H

#include <QObject>

#ifdef Q_OS_ANDROID
#include <QAndroidJniObject>
#include <QtAndroid>
#include <QtAndroidExtras/QAndroidJniObject>
#endif

class KeepScreenOn : public QObject
{
    Q_OBJECT
public:
    explicit KeepScreenOn(QObject *parent = nullptr);
    void on();
    void off();

signals:

public slots:
};

#endif // KEEPSCREENON_H
