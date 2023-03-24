#ifndef VEHICLE_HPP
#define VEHICLE_HPP

#define FREQ 5000
#define LEFT_CHANNEL 2
#define RIGHT_CHANNEL 3
#define RESOLUTION 8


#include <Arduino.h>

class Vehicle
{
public:
    Vehicle(/* args */);
    ~Vehicle();

    void begin();   // Fonction d'initialisation: à appeler après avoir défini les différentes lignes/broches (par les fonctions set)

    void setLeftFrontLine(uint8_t);    // Définir leftFrontLine
    void setLeftBackLine(uint8_t);     // Définir leftBackLine
    void setLeftMotorPWMLine(uint8_t); // Définir la ligne PWM pour le moteur gauche

    void setRightFrontLine(uint8_t);
    void setRightBackLine(uint8_t);
    void setRightMotorPWMLine(uint8_t);

    void rideFront();                  // Avancer
    void rideBack();                   // Reculer
    void turnLeft();                   // Tourner à gauche
    void turnRight();                  // Tourner à droite
    void brake();                      // Freiner
    
    void setLeftMotorVelocity(uint8_t);       // Définir la vitesse du motor gauche
    void setRightMotorVelocity(uint8_t);      // Définir la vitesse du motor droit
    void setVelocity(uint8_t);                // Définir une vitesse commune pour les moteurs des deux cotés
    uint8_t getLeftMotorVelocity();           // Retourne la vitesse moteur gauche
    uint8_t getRightMotorVelocity();
    

private:
    void rideLeftMotorFront();         // Faire avancer le moteur gauche
    void rideLeftMotorBack();          // Faire reculer le moteur droit
    void stopLeftMotor();
    void rideRightMotorFront();
    void rideRightMotorBack();
    void stopRightMotor();
    

private:
    uint8_t leftFrontLine;              // Ligne à mettre à HIGH pour faire avancer les moteurs gauches
    uint8_t leftBackLine;               // Ligne à mettre à HIGH pour faire reculer les moteurs gauches
    uint8_t rightFrontLine;              // Ligne à mettre à HIGH pour faire avancer les moteurs droits
    uint8_t rightBackLine;              // Ligne à mettre à HIGH pour faire reculer les moteurs droits

    uint8_t leftMotorPWMLine;                // Ligne de PWM pour définir la vitesse du moteur gauche
    uint8_t rightMotorPWMLine;               // Ligne de PWM pour définir la vitesse du moteur droit

    uint8_t leftMotorVelocity = 0;      // Vitesse du moteur gauche
    uint8_t rightMotorVelocity = 0;     // Vitesse du moteur droit
};

//extern Vehicle myCar;

//Vehicle myCar;     // Instantiation d'un objet unique de la classe Vehicle à utiliser dans le main

#endif