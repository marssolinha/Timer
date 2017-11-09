#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "./source/statusbar.h"
#include "./source/hostinfo.h"
#include "./source/networkdiscovery.h"
#include "./source/tcpconnect.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    qmlRegisterType<StatusBar>("StatusBar", 0, 1, "StatusBar");
    qmlRegisterType<HostInfo>("HostInfo", 0, 1, "HostInfo");
    qmlRegisterType<NetworkDiscovery>("NetworkDiscovery", 0, 1, "NetworkDiscovery");
    qmlRegisterType<TcpConnect>("TcpConnect", 0, 1, "TcpConnect");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
