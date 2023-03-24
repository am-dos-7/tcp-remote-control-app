/* En vue de disposer de plus de GPIO, on a décidé de se passer des messages de debug
 * via le port série. On a donc commenter toutes les instructions d'affichage 
 *
 * 
 */

#include <Arduino.h>

#include "esp_camera.h"
#include <WiFi.h>
#include <Wire.h>

#include <lwip/sockets.h>
#include <esp_log.h>
#include <string.h>
#include <errno.h>
#include "sdkconfig.h"
#include "esp_cam_libZ.hpp"
#include <Tcp_server.hpp>
//#include "vehicle.hpp"

 const char* ssid = "ALPHA224";
 const char* password = "CROYANCE214@@";

// const char* ssid = "UnivCasa";
// const char* password = "";

#define PORT_NUMBER 50885
#define WIDTH 320
#define HEIGHT 240

/* Paramètres pour l'I2C */
#define SCL 12
#define SDA 13
#define I2C_SLAVE_ADDRESS 12

void printData(uint8_t data);

EspCamLib camera;

TcpServer *server;

void setup() {
  
  Serial.begin(9600);
  Serial.setDebugOutput(true);
  Serial.println();

  Wire.begin(SDA, SCL);
    // Test de la liaision I2C
  Wire.beginTransmission(I2C_SLAVE_ADDRESS);
  Wire.write((uint8_t) 12);                   // On envoie le code 12 pour montrer que la carte a bien démarré.
  Wire.endTransmission();
  
  camera.begin();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  

  Serial.print("Camera Ready at ");
  Serial.print(WiFi.localIP());
  Serial.println();
  

  server = new TcpServer;
  server->setDataHandler(printData);
  server->start((char*)"", PORT_NUMBER);
  delay(2000);

  camera_fb_t *pic = NULL; 

  Serial.printf("En attente de connexion d'un client!\n");
  while(!server->isClientConnected());

  /* Si on arrive ici, c'est qu'un client s'est connecté */
  
  camera.setFrameSize(taille::VGA);
  while(1){
    //pic = esp_camera_fb_get();
    camera.getFrame(&pic);
    if(pic != NULL){
          if(server->isClientConnected()){
            server->sendFrame((char*) pic->buf, pic->len);  // Envoie de la capture au client
          }
          else{
            Serial.printf("Client non connecté!\n");
            delay(1000);
          }
          
          esp_camera_fb_return(pic);
          //delay(50);
        }
    }
    //delay(100);
  }
// }

void loop() {
  // put your main code here, to run repeatedly:
  delay(10000);
  
}

void printData(uint8_t data){
  Serial.printf("Byte received: %u\n", data);

  // Envoi de la donnée au périphérique à l'adresse 12
  Wire.beginTransmission(I2C_SLAVE_ADDRESS);
  Wire.write((uint8_t) data);
  Wire.endTransmission();
}