#include "fonctions_print.h"

void printEtatSorties()
{
  Serial.print('[');
  Serial.print(servoEmpLacet.read());
  Serial.print(',');
  Serial.print(servoAileDroite.read());
  Serial.print(',');
  Serial.print(servoAileGauche.read());
  Serial.print(',');
  Serial.print(servoAideDecollageGauche.read());
  Serial.print(',');
  Serial.print(servoAideDecollageDroite.read()); 
  Serial.print(',');
  Serial.print(moteurExtDroite.read());
  Serial.print(',');
  Serial.print(moteurIntDroite.read());
  Serial.print(',');
  Serial.print(moteurIntGauche.read());
  Serial.print(',');
  Serial.print(moteurExtGauche.read());
  Serial.print(',');
  Serial.print(servoDroiteStab.read());
  Serial.print(',');
  Serial.print(servoGaucheStab.read());
  Serial.print(',');
  Serial.print(trainAtterissage.read());
  Serial.println(']');
}

void printMessageRadio(DONNEES &message)
{
  Serial.print('[');
  Serial.print(message.Acc);
  Serial.print(',');
  Serial.print(message.LUn);
  Serial.print(',');
  Serial.print(message.RUn);
  Serial.print(',');
  Serial.print(message.XRoulis);
  Serial.print(',');
  Serial.print(message.YRoulis);
  Serial.print(',');
  Serial.print(message.decollage);
  Serial.println(']');
}