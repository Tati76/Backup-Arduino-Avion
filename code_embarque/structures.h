// *********************************** STRUCTURE RADIO ************************************
#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <Arduino.h>
#include <RF24.h>

struct DONNEES {
  uint8_t Acc; //moteur
  uint8_t LUn; // servoEmpLacet 
  uint8_t RUn; // servoEmpLacet 
  uint8_t XRoulis; // servoAileGauche et servoAileDroite
  uint8_t YRoulis; // servoDroiteStab et servoGaucheStab
  bool decollage; // si train d'atterissage alors aide à l'atterissage/decollage
};

#endif