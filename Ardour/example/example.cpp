#include <ardour/ardour.h>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml/qqmlextensionplugin.h>

Q_IMPORT_QML_PLUGIN(ArdourPlugin)

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    if (!ARDOUR::init (true, NULL, true))
    {
        return EXIT_FAILURE;
    }

    QQmlApplicationEngine engine;
    // The first subfolder is the libraryName followed by the regular
    // folder structure: LibraryName/Subfolder
    const QUrl url(QStringLiteral("qrc:/ExampleProjectApp/example/example.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
