// ************* La Radio *************
#include <SPI.h>
#include <RF24.h> // voir http://tmrh20.github.io/RF24/
#include <Servo.h>
#include "fonctions_print.h"
#include "fonctions_radio.h"
#include "fonctions_transmettre_consigne_moteurs.h"
#include "structures.h"
#include "constantes.h"

// ********************************** SERVO *************************************
Servo servoEmpLacet; // angle Lacet
Servo servoAileDroite;
Servo servoAileGauche;
Servo servoAideDecollageGauche;
Servo servoAideDecollageDroite;
Servo moteurExtDroite;
Servo moteurIntDroite;
Servo moteurIntGauche;
Servo moteurExtGauche;
Servo servoDroiteStab; // angle vertical joystick tanguage
Servo servoGaucheStab; // angle vertical joystick tanguage
Servo trainAtterissage;

// *********************************** RADIO ************************************
// Configurer vos radio nRF24L01+ sur le bus SPI et mettre  CE sur D7 et CSN sur D8
RF24 radio(7, 8);
const byte adresses[][6] = {"0pipe", "1pipe"}; 
uint8_t role;
long compteur(0);
DONNEES buffer;
unsigned long temps;
// ------------------------------------------------------------------

void setup() {
  Serial.begin(115200);
  
  // PIN POUR LES SERVOS ET LES MOTEURS
  servoEmpLacet.attach(2);
  servoAileDroite.attach(A4);//A5
  servoAileGauche.attach(A5);
  servoAideDecollageGauche.attach(A3);
  servoAideDecollageDroite.attach(A2);
  moteurExtDroite.attach(3);
  moteurIntDroite.attach(4);
  moteurIntGauche.attach(5);
  moteurExtGauche.attach(6);
  servoGaucheStab.attach(A1);
  servoDroiteStab.attach(A0);
  trainAtterissage.attach(1);
  
  delay(15);

  // INITIALISATION DES MODULES nrf24l01+
  role =  1 ;
  Serial.print(F("\nMon Role = ")); 
  Serial.println(role);

  // CONFIGURATION DE LA RADIO
  radio.begin();
  // pour le test on règle le niveau d'énergie à RF24_PA_LOW pour éviter les interférences
  // mettre à RF24_PA_MAX si on veut la puissance d'émission max
  radio.setPALevel(RF24_PA_HIGH); //RF24_PA_LOW
  //radio.setDataRate(RF24_250KBPS);
  
  radio.openWritingPipe(adresses[1]); // role doit être 0 ou 1
  radio.openReadingPipe(1, adresses[0]); // 1 - role = l'autre adresse
  
  // Start the radio listening for data
  radio.startListening();
  
  // Demarrage et initialisation des ESC pour les rendre utilisables
  initESC();
  
  // INITIALISATION DU COMPTEUR DE TEMPS
  temps = millis();
}

void loop() {
  
  //testFunction();
  buffer = ecouterRadio();
  transmettreConsigneMoteurs(buffer);
  printEtatSorties();
  if ((millis()-temps) > 3000)
  {
    arretUrgence();
  }
}