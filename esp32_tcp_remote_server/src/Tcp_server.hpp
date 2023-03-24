#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP

/* 
 * Début: 07/05/2021
 * Auteur: egne_Zirée
 * 
 * Implémentation d'un serveur TCP à partir des 'C socket'
 */

#include <Arduino.h>
#include <lwip/sockets.h>
#include <esp_log.h>
#include <string.h>
#include <errno.h>
#include "sdkconfig.h"
#include <pthread.h>

class TcpServer
{
public:
    TcpServer();
    ~TcpServer();
    int start(char * address, uint16_t port_number);          // démarrer le serveur à l'addresse et sur le port spécifiés
    int disconnectCurrentClient();       // Déconnecter la socket client en cours
    int sendFrame(char *frame, uint32_t size);
    void setDataHandler(void (*function) (uint8_t)); // Définit la fonction à appeler s'il y a une donnée
    bool isClientConnected();


private:        /* Private functions */
    static void * sta_accept_thread_fn(void *); // static accept thread function: Sera lancée dans un thread qui tourne en boucle et se termine à la connexion d'un client: 'sta' prefix for static
    static void * sta_data_thread_fn(void *);   // static data thread function

    void startAcceptThread();           // Lancera le thread d'écoute
    void startDataThread();            // Lance le thread d'où seront reçues les données du client
    void setClientConnected(bool);      // Copie le paramètre dans clientConnected, appelée à la connexion et à la déconnexion

    int sendAsByteArray(uint32_t);   // Envoyer un 'Unsigned Integer' comme une suite d'octet, LSB First: pratique pour envoyer la taille d'un message
    int sendData(char *data, uint32_t size);  // Ecrire des données sur la socket
    

private:    /* Private attributes */
    void (*dataHandler)(uint8_t);          // pointeur sur fonction
    int server_descriptor;   // Descripteur de la socket
    int client_descriptor;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    pthread_t accept_thread;
    pthread_t data_thread;
    socklen_t clientAdressLength;
    bool clientConnected;                       // Indique si un client est connecté: est mis à 1 à une connexion et à 0 à une déconnexion
                                                // Penser à associer une mutex à cet attribut     
};

#endif


/*
 *  NOTES GENERALES
 * L'utilisation des Threads POSIX a exigé de rendre statique la fonction passé au thread à sa création
 * (le C ne reconnait pas le mot-clé this du C++ qui accompagne les classes).
 * Cela entraine dans certains cas la nécessité de rendre également statiques plusieurs autres méthodes 
 * de la classe sinon toutes. Pour contourner ce problème, on peut passer en paramètre à la fonction statique
 * en question, un objet dont on va appeler la méthode non statique à l'intérieur de la fonction statique.
 * De cette manière, la méthode statique va juste servir comme coquille juste pour contourner le problème.
 * 
 */