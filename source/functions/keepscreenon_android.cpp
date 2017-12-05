#include "keepscreenon.h"
#include <QDebug>

KeepScreenOn::KeepScreenOn(QObject *parent) : QObject(parent)
{

}

void KeepScreenOn::on()
{
    QtAndroid::runOnAndroidThread([]() {
        QAndroidJniObject window = QtAndroid::androidActivity().callObjectMethod("getWindow", "()Landroid/view/Window;");
        const int FLAG_KEEP_SCREEN_ON = 128;
        window.callMethod<void>("addFlags", "(I)V", FLAG_KEEP_SCREEN_ON);
    });
}

void KeepScreenOn::off()
{
    QtAndroid::runOnAndroidThread([]() {
        QAndroidJniObject window = QtAndroid::androidActivity().callObjectMethod("getWindow", "()Landroid/view/Window;");
        const int FLAG_KEEP_SCREEN_ON = 128;
        window.callMethod<void>("clearFlags", "(I)V", FLAG_KEEP_SCREEN_ON);
    });
}
