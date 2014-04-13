#include <QApplication>
#include <QQmlApplicationEngine>
#include "mjpegstreamerclient.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<MjpegStreamerClient>("FancyControls", 1, 0, "MjpegStreamerClient");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:///qml/main.qml")));

    return app.exec();
}
