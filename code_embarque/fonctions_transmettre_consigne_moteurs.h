#ifndef FONCTIONS_TRANSMETTRE_CONSIGNE_MOTEURS_H
#define FONCTIONS_TRANSMETTRE_CONSIGNE_MOTEURS_H
#include <Servo.h>
#include "structures.h"
#include "constantes.h"
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

extern unsigned long temps;

void transmettreConsigneMoteurs(DONNEES &message);
void testFunction();
void initESC();
void arretUrgence();

#endif