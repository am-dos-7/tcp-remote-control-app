#include "vehicle.hpp"


Vehicle::Vehicle()
{}

Vehicle::~Vehicle()
{}

void Vehicle::begin()
{
    // On met les lignes de commande en sortie
    pinMode(leftFrontLine, OUTPUT);
    pinMode(leftBackLine, OUTPUT);
    pinMode(leftMotorPWMLine, OUTPUT);

    pinMode(rightFrontLine, OUTPUT);
    pinMode(rightBackLine, OUTPUT);
    pinMode(rightMotorPWMLine, OUTPUT);

    /* et on les force à LOW */
    /*
    digitalWrite(leftFrontLine, LOW);
    digitalWrite(leftBackLine, LOW);
    digitalWrite(leftMotorPWMLine, LOW);

    digitalWrite(rightFrontLine, LOW);
    digitalWrite(rightBackLine, LOW);
    digitalWrite(rightMotorPWMLine, LOW);
    */

    // Configuration du PWM pour l'ESP32
    ledcSetup(LEFT_CHANNEL, FREQ, RESOLUTION);     // Configuration de la voie 0
    ledcSetup(RIGHT_CHANNEL, FREQ, RESOLUTION);     // Configuration de la voie 1

    ledcAttachPin(leftMotorPWMLine, LEFT_CHANNEL);     // Rattachement de la ligne PWM gauche à la voie 0
    ledcAttachPin(rightMotorPWMLine, RIGHT_CHANNEL);    // Rattachement de la ligne PWM droite à la voie 1
}

void Vehicle::setLeftFrontLine(uint8_t line)
{
    leftFrontLine = line;
}

void Vehicle::setLeftBackLine(uint8_t line)
{
    leftBackLine = line;
}

void Vehicle::setLeftMotorPWMLine(uint8_t line)
{
    leftMotorPWMLine = line;
}

void Vehicle::setRightFrontLine(uint8_t line)
{
    rightFrontLine = line;
}

void Vehicle::setRightBackLine(uint8_t line)
{
    rightBackLine = line;
}

void Vehicle::setRightMotorPWMLine(uint8_t line)
{
    rightMotorPWMLine = line;
}

void Vehicle::rideLeftMotorFront()
{
    digitalWrite(leftFrontLine, HIGH);
    digitalWrite(leftBackLine, LOW);
}

void Vehicle::rideLeftMotorBack()
{
    digitalWrite(leftBackLine, HIGH);
    digitalWrite(leftFrontLine, LOW);
}

void Vehicle::stopLeftMotor()
{
    digitalWrite(leftFrontLine, LOW);
    digitalWrite(leftBackLine, LOW);
}

void Vehicle::rideRightMotorFront()
{
    digitalWrite(rightFrontLine, HIGH);
    digitalWrite(rightBackLine, LOW);
}

void Vehicle::rideRightMotorBack()
{
    digitalWrite(rightBackLine, HIGH);
    digitalWrite(rightFrontLine, LOW);
}

void Vehicle::stopRightMotor()
{
    digitalWrite(rightFrontLine, LOW);
    digitalWrite(rightBackLine, LOW);
}

void Vehicle::rideFront()
{
    rideLeftMotorFront();
    rideRightMotorFront();
}

void Vehicle::rideBack()
{
    rideLeftMotorBack();
    rideRightMotorBack();
}

void Vehicle::turnLeft()
{
    stopLeftMotor();
    rideRightMotorFront();
}

void Vehicle::turnRight()
{
    stopRightMotor();
    rideLeftMotorFront();
}

void Vehicle::brake()
{
    stopLeftMotor();
    stopRightMotor();
}


void Vehicle::setLeftMotorVelocity(uint8_t v)
{
    leftMotorVelocity = v;      // On mémorise la vitesse, pour pouvoir la retourner avec get
    ledcWrite(LEFT_CHANNEL, leftMotorVelocity);     // puis on la change
}

void Vehicle::setRightMotorVelocity(uint8_t v)
{
    rightMotorVelocity = v;     // On mémorise la vitesse, pour pouvoir la retourner avec get
    ledcWrite(RIGHT_CHANNEL, rightMotorVelocity);   // puis on la change
}

void Vehicle::setVelocity(uint8_t v)
{
    setLeftMotorVelocity(v);
    setRightMotorVelocity(v);
}

uint8_t Vehicle::getLeftMotorVelocity()
{
    return leftMotorVelocity;
}

uint8_t Vehicle::getRightMotorVelocity()
{
    return rightMotorVelocity;
}