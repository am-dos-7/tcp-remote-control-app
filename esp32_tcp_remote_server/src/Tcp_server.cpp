#include "Tcp_server.hpp"

TcpServer::TcpServer()
{
    server_descriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    server_address.sin_family = AF_INET;
    clientConnected = false;

    /*if((server_descriptor < 0) || (client_descriptor < 0)){

    }*/

    clientAdressLength = sizeof(struct sockaddr_in);    // Initialisation de la cette variable qui est un value-result: nécessaire pour la fonction accept
}

TcpServer::~TcpServer()
{   /* Nothing to do */
}

int TcpServer::start(char address[], uint16_t port_number)
{
    server_address.sin_port = htons(port_number);

    if(!strcmp(address, "")){    /* Si la chaine est vide, on démarre le serveur sur n'importe quelle adresse */
        Serial.println("Adresse serveur:     ANY");
        server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    else
        inet_aton(address, &server_address.sin_addr);
    
    /* Démarrage du serveur */
    if(bind(server_descriptor, (struct sockaddr *)&server_address, sizeof(server_address)) < 0){
        Serial.println("Echec du démarrage du serveur");
        return -1;
    }

    if(listen(server_descriptor, 5) < 0)
        return -2;

    startAcceptThread();

    return 1;   // Le code résultat à envoyer est à gérer plus tard.
}

void TcpServer::startAcceptThread()
{
    pthread_create(&accept_thread, NULL, TcpServer::sta_accept_thread_fn, this);
}

/* La fonction suivante va boucler tant qu'il n'y a pas de client connecté et s'arrêter dès qu'un client se connecte */
void * TcpServer::sta_accept_thread_fn(void * param)
{   
    TcpServer *server = (TcpServer*) param;
    server->client_descriptor = accept(server->server_descriptor, (struct sockaddr*) &server->client_address, &server->clientAdressLength);
    while(server->client_descriptor < 0){   /* On boucle tant qu'on n'a pas de client */
        delay(1000);
        server->client_descriptor = accept(server->server_descriptor, (struct sockaddr*) &server->client_address, &server->clientAdressLength);
    }

    // Si on on sort c'est qu'un client s'est connecté
    server->setClientConnected(true);       // on actualise le flag clientConnected: client connecté
    Serial.printf("Nous avons un client à l'adresse %s et de numéro de port %u\n", inet_ntoa(server->client_address.sin_addr), ntohs(server->client_address.sin_port));

    /* On lance le thread qui s'occupe de la réception des données */
    server->startDataThread();
    Serial.println("Accept thread closed");
    pthread_exit(NULL);
    //return ;
}

void TcpServer::startDataThread()
{
    pthread_create(&data_thread, NULL, TcpServer::sta_data_thread_fn, this);
}

void * TcpServer::sta_data_thread_fn(void * param)
{
    TcpServer *server = (TcpServer *) param;
    char data[2];
    int lus;
    memset(data, '\0', 2);
    while(server->isClientConnected()){ /* La lecture de données tourne tant que le client est connecté, et se termine dès que ce dernier se déconnecte */
        lus = read(server->client_descriptor, data, 1);   // lire un octet à chaque fois
        switch(lus){
            case -1:
                Serial.println("Erreur de lecture socket client");
                goto END_DATA_THREAD;
                break;
            case  0:
                Serial.println("Client déconnecté");
                goto END_DATA_THREAD;  break;
            default: 
                server->dataHandler((uint8_t) data[0]); //server->dataHandler((uint8_t) data[0]);
        }
    }
    END_DATA_THREAD :   // Etiquette référençant la déconnexion du client
    close(server->client_descriptor);
    server->setClientConnected(false);  // On actualise le flag clientConnected: client déconnecté
    Serial.println("Data thread closed");
    server->startAcceptThread();   // On lance la thread d'écoute à nouveau
    pthread_exit(NULL);
    //return ;
}

void TcpServer::setDataHandler(void (*func) (uint8_t))
{
     dataHandler = func;
}

int TcpServer::sendAsByteArray(uint32_t param)
{
    char* temp = NULL;
    int n = sizeof(param);
    temp = (char*) malloc((n + 1) * sizeof(char)); // le +1 pour '\0'
    if(temp == NULL){
        perror("malloc envoyerInt");
        //exit(EXIT_FAILURE);
    }

    for(int i = 0; i < n; i++)
        temp[i] = (param >> (8*i)); // On aura >> 0, >> 8, >> 16, >> 32, ... LSB First

    uint32_t nombre_recup = 0;
    for(int i = 0; i < n; i++)
        nombre_recup += (unsigned char)temp[i] << 8*i;

    return write(client_descriptor, temp, n);
}

int TcpServer::sendData(char *data, uint32_t length)
{
    return write(client_descriptor, data, length);
}

int TcpServer::sendFrame(char *frame, uint32_t size)
{
    sendAsByteArray(size);  // On envoie d'abord la taille
    sendData(frame, size);        // et ensuite la trame complète
}

void TcpServer::setClientConnected(bool state)
{
    clientConnected = state;
}

bool TcpServer::isClientConnected()
{
    return clientConnected;
}

int TcpServer::disconnectCurrentClient()
{
    return close(client_descriptor);    // la fermeture du descripteur de la socket entraine la déconnexion
}