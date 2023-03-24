#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QStandardPaths>
#include <fstream>
#include <backend.h>
#define ANDROID_DEPLOY        // A commenter pour les versions Desktop

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);


    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
#ifdef ANDROID_DEPLOY
    BackEnd::setAndroidBuild(true);
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    engine.rootContext()->setContextProperty("applicationDirPath", path);
#else
    BackEnd::setAndroidBuild(false);
    engine.rootContext()->setContextProperty("applicationDirPath", QGuiApplication::applicationDirPath());
#endif
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

//    QString path = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
//    qDebug() << "Pictures Location: " << path;

//    path.append("/myPict.txt");

//    std::ofstream file(qPrintable(path), std::ios::binary);
//    file << "Hello! It's me";
//    file.close();

//    std::ifstream file_read(qPrintable(path), std::ios::binary);
//    std::string text;
//    file_read >> text;

//    qDebug() << "Read back: " << QString::fromStdString(text);
    qDebug() << "Application Data Dir: " << QGuiApplication::applicationDirPath();

    return app.exec();
}
