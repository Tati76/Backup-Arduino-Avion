#ifndef FONCTIONS_RADIO_H
#define FONCTIONS_RADIO_H

#include "structures.h"
#include <RF24.h>
#include <Arduino.h>

void envoyerMessage(uint8_t nombre);
DONNEES ecouterRadio();

#endif