#ifndef FONCTIONS_PRINT_H
#define FONCTIONS_PRINT_H
#include <SPI.h>
#include <RF24.h>
#include <Servo.h>
#include "structures.h"
#include <Arduino.h>

extern Servo servoEmpLacet; // angle Lacet
extern Servo servoAileDroite;
extern Servo servoAileGauche;
extern Servo servoAideDecollageGauche;
extern Servo servoAideDecollageDroite;
extern Servo moteurExtDroite;
extern Servo moteurIntDroite;
extern Servo moteurIntGauche;
extern Servo moteurExtGauche;
extern Servo servoDroiteStab; // angle vertical joystick tanguage
extern Servo servoGaucheStab; // angle vertical joystick tanguage
extern Servo trainAtterissage;


void printEtatSorties();
void printMessageRadio(DONNEES &message);

#endif