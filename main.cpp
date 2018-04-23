#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "connectionlistmodel.h"
#include "vlccontroller.h"
#include <QAnimationDriver>
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    app.setApplicationDisplayName("vlc-remote");
    app.setApplicationName("vlc-remote");

    qmlRegisterType<ConnectionListModel>("app.model.ConnectionList",1,0,"ConnectionList");
    qmlRegisterType<VLCController>("app.model.VLC",1,0,"VLC");

    QQmlApplicationEngine engine;

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}


//TODO use QSetting for storing information and QAndroidPermission for authorization
