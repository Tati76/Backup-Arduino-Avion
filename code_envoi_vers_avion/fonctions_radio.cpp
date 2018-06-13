#include "fonctions_radio.h"

extern RF24 radio;

// ----------------------------------------------------------------------------------------
// envoi d'un octet vers l'autre radio
// ----------------------------------------------------------------------------------------
void envoyerMessage(uint8_t nombre)
{
  radio.stopListening();   // On arrête d'écouter pour qu'on puisse émettre

  if (!radio.write( &nombre, sizeof(nombre) )) {
    Serial.println(F("erreur d'envoi"));
  }
  radio.startListening(); // On se remet en mode écoute
}

// ----------------------------------------------------------------------------------------
// vérifie si on a reçu une commande de la part de l'autre radio (1 octet)
// ----------------------------------------------------------------------------------------
DONNEES ecouterRadio()
{
  DONNEES message; 

  if ( radio.available()) {
    while (radio.available()) {
      radio.read( &message, sizeof(DONNEES) );  // on lit l'octet reçu (si plusieurs messages on ne conserve que le dernier)
    }
	
  }
  else {
    Serial.println("Aucune entrée");
  }
  return message;
}
