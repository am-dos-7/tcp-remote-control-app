#include "backend.h"

bool BackEnd::android_build;

BackEnd::BackEnd(QObject *parent) :
    QObject(parent)//, QQuickImageProvider(QQuickImageProvider::Pixmap)
{
    m_serverSocket = new QTcpSocket(this);
    //connect() // connect all necessary signals related to the TCP socket
    connect(m_serverSocket, SIGNAL(connected()), this, SLOT(on_connected()));
    connect(m_serverSocket, SIGNAL(disconnected()), this, SLOT(on_disconnected()));
    connect(m_serverSocket, SIGNAL(readyRead()), this, SLOT(donnees_recues()));

    /* Configurer les dimensions de l'image par défaut */
    setDimensions(WIDTH, HEIGHT);

    image = new QImage;
    tailleMessage = 0;

    if(android_build){
        pictures_path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/";
    }
    else{
        pictures_path = "";
    }
}

QString BackEnd::serverAddress()
{
    return m_serverAddress;
}

int BackEnd::serverPort()
{
    return m_serverPort;
}

void BackEnd::setServerAddress(const QString address)
{
    m_serverAddress = address;
}

void BackEnd::setServerPort(const int port)
{
    m_serverPort = port;
}

void BackEnd::connectToServer(const bool state)
{
    if(state){
        qDebug()<< "Trying to connect to server at " + m_serverAddress + " on Port number " << m_serverPort;
        m_serverSocket->connectToHost(QHostAddress(m_serverAddress), m_serverPort);
    }
    else {
        m_serverSocket->disconnectFromHost();   // On se déconnecte
        qDebug() << "Disconnected from server";
        qDebug() << "Pending data: " << m_serverSocket->bytesAvailable();
        tailleMessage = 0;                          // On remet la taille de la frame en cours de réception à zéro.
    }
}

void BackEnd::sendData(int data)
{
    char _data[1];
    _data[0] = (char) data;
    m_serverSocket->write(_data, 1);
}

void BackEnd::setDimensions(uint16_t w, uint16_t h)
{
    width = w;
    height = h;
}

void BackEnd::on_connected()            // connected to server
{
    emit connectionStateChanged(true);
}

void BackEnd::on_disconnected()
{
    emit connectionStateChanged(false); // disconnected from server, or connection close by server
}


void BackEnd::donnees_recues()
{
//    qDebug() << "Recu";
    if(m_serverSocket == 0)
        return;

    // Si on reçoit un nouveau message. Sinon on arrête de suite la méthode
    if(tailleMessage == 0)
    {// On récupère la taille du message si c'est possible
        if(m_serverSocket->bytesAvailable() < (int)sizeof(uint32_t))
            return;
        char *temp = NULL;
        temp = new char [sizeof (uint32_t) * sizeof (char)];
        if(temp == NULL){
            perror("alloc");
            return;
        }
        m_serverSocket->read(temp, sizeof (uint32_t));

        for (int i(0); i< (int)sizeof (uint32_t); i++) {
            tailleMessage += (unsigned char) temp[i] << 8*i;
        }
        delete temp;
//        qDebug()<< "Taille: "<< tailleMessage;
        //in >> tailleMessage;
    }
    // Si on connait la taille du message, on essaie de récupérer le message en entier
    if(m_serverSocket->bytesAvailable() < tailleMessage)
        return;
    // On a récupéré tout le message
    char *message = NULL;
    message = new char[(tailleMessage) * sizeof (char)];
    if(message == NULL){
        perror("alloc message");
        exit(EXIT_FAILURE);
    }

    m_serverSocket->read(message, tailleMessage);

    *image = QImage::fromData((uchar *)message, tailleMessage);
    //capture->setPixmap(QPixmap::fromImage(*image));

    /* We use alternatively two files to save and load the frames*/
    if(!image_index){
        image->save(pictures_path + "image0.jpg");
        image_index = true;
    }
    else{
        image->save(pictures_path + "image1.jpg");
        image_index = false;
    }

    emit frameAvailable();
    delete message;
    tailleMessage = 0;
}

bool BackEnd::getImageIndex()
{
    return image_index;
}

BackEnd::~BackEnd()
{
    qDebug() << "Image 0 delete state: " << QFile::remove(pictures_path + "image0.jpg");
    qDebug() << "Image 1 delete state: " << QFile::remove(pictures_path + "image1.jpg");
}

//QPixmap BackEnd::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
//{
//    int width = 100;
//   int height = 50;

//   if (size)
//      *size = QSize(width, height);
//   QPixmap pixmap(requestedSize.width() > 0 ? requestedSize.width() : width,
//                  requestedSize.height() > 0 ? requestedSize.height() : height);
//   pixmap.fill(QColor(id).rgba());
//   return pixmap;
//}


//void BackEnd::makeProviderAccessible() ///* Provider has 'video as identifier */
//{
//    QQuickView view;
//    QQmlEngine *engine = view.engine();
//    engine->addImageProvider(QLatin1String("video"), new BackEnd);
//    view.setSource(QUrl(QStringLiteral("MainFrame.qml"))/*::fromLocalFile(QStringLiteral("MainFrame.qml"))*/);
//    view.show();
//}

