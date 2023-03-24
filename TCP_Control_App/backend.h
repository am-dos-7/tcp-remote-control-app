#ifndef BACKEND_H
#define BACKEND_H

#include <QHostAddress>
#include <QTcpSocket>
#include <QObject>
#include <QString>
#include <QDebug>
//#include <QImage>
#include <fstream>
#include <qqml.h>
#include <QQuickImageProvider>
#include <QQuickView>
#include <QQmlEngine>
#include <QImage>
#include <QPixmap>
#include <QLabel>

#include <QGuiApplication>
#include <QStandardPaths>

#define WIDTH 320
#define HEIGHT 240

class BackEnd : public QObject//, QQuickImageProvider /* This last needed to send capture frame to the QML interface*/
{
    Q_OBJECT
    Q_PROPERTY(QString serverAddress READ serverAddress WRITE setServerAddress /*NOTIFY serverAddressChanged*/)
    Q_PROPERTY(int serverPort READ serverPort WRITE setServerPort /*NOTIFY serverPortChanged*/)
    Q_PROPERTY(bool connectToServer /*READ getConnectionState*/ WRITE connectToServer NOTIFY connectionStateChanged)
    Q_PROPERTY(int data WRITE sendData) // used to send data when a touche is pressed
    Q_PROPERTY(NOTIFY frameAvailable)
    Q_PROPERTY(bool image_index READ getImageIndex)
    Q_PROPERTY(bool android_build READ isAndroidBuild)
    QML_ELEMENT

public:
    explicit BackEnd(QObject *parent = nullptr);
    ~BackEnd();
    QString serverAddress();                    // return the server address
    int serverPort();                      // return the server port
    void setServerAddress(const QString address);
    void setServerPort(const int port);
    void connectToServer(const bool state);     // connect to server if state == 1, disconnect if state == 0
    void sendData(int);
    void setDimensions(uint16_t w, uint16_t h); // configure width and height
    bool getImageIndex();
    bool isAndroidBuild(){ // return the build for Android value
        return android_build;
    }

    void static setAndroidBuild(bool arg){  // Définit la valeur de android_build
            android_build = arg;
    }

public slots:
    void on_connected();
    void on_disconnected();
    void donnees_recues();

signals:
    void connectionStateChanged(bool connection_state);  // emitted on connection successful or broken connection; state = 1 if connected, 0 else
    void frameAvailable();                               // emitted when a frame is available to be loaded to GUI

private:
    QString m_serverAddress;
    int m_serverPort;
    QTcpSocket *m_serverSocket;
    uint32_t tailleMessage;
    uint16_t width;
    uint16_t height;
    QLabel *capture;
    QImage *image;
    bool image_index;                                   // will be used when saving image
    QString pictures_path; /*QGuiApplication::applicationDirPath()*/;
    static bool android_build;     // = 1 for Android builds; = 0 else. Specify whether the build is for Android or not, to the QML part
};

#endif // BACKEND_H
