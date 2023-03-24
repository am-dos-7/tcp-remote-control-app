#ifndef TCP_LIBZ_HPP
#define TCP_LIBZ_HPP

/* 
 * Début: 07/05/2021
 * Auteur: Am_Zirée
 * 
 * Implémentation d'un client TCP à partir des 'C socket'
 */

#include <Arduino.h>
#include <lwip/sockets.h>
#include <esp_log.h>
#include <string.h>
#include <errno.h>
#include "sdkconfig.h"

class TcpClient
{
public:
    TcpClient();
    ~TcpClient();
    int setAddress();       // Définir l'adresse de la socket
    void setPortNumber();   // Définir le numéro de port de la socket
    int connect();          // Se connecter à la socket
    int disconnect();       // Se déconnecter de la socket
    int sendData(char *data, uint32_t size);  // Ecrire des données sur la socket
    int readData(char *buffer, uint32_t size);// Lire des données à partir de la socket
    int sendAsByteArray(uint32_t);   // Envoyer un 'Unsigned Integer' comme une suite d'octet, LSB First: pratique pour envoyer la taille d'un message

private:
    int socket_describer;   // Descripteur de la socket
    uint16_t port_number;
    char *address;
};

TcpClient::TcpClient(/* args */)
{
}

TcpClient::~TcpClient()
{
}


#endif