// ************* La Radio *************
#include <SPI.h>
#include <RF24.h> // voir http://tmrh20.github.io/RF24/

// ************* La Manette *************
#include <PS4USB.h>
// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif



// Configurer vos radio nRF24L01+ sur le bus SPI et mettre  CE sur D7 et CSN sur D8
RF24 radio(7, 8);

// Le nom des "pipes" de communication, un en lecture, un en écriture
const byte adresses[][6] = {"0pipe", "1pipe"}; // Pipes 1-5 should share the same address, except the first byte. Only the first byte in the array should be unique

// A CONFIGURER sur la pin A0
// si A0 est à GND alors rôle = 0 --> le premier Arduino
// si A0 est à  3.3V ou 5V alors rôle = 1 --> pour le second
const byte configurationPin = A0;
uint8_t role;

// *************  Deux Leds avec résitances de limiation de courant *************
const byte pinLed0 = 9;
const byte pinLed1 = 10;

USB Usb;
PS4USB PS4(&Usb);

uint8_t ancienEtatL3;
bool decollage;

typedef struct{
  uint8_t Acc;
  uint8_t LUn;
  uint8_t RUn;
  uint8_t XRoulis;
  uint8_t YRoulis;
  bool decollage;
} DONNEES;



// ----------------------------------------------------------------------------------------
// envoi d'un octet vers l'autre radio
// ----------------------------------------------------------------------------------------

void envoyerMessage(DONNEES nombre)
{
  radio.stopListening();   // On arrête d'écouter pour qu'on puisse émettre

  if (!radio.write( &nombre, sizeof(DONNEES) )) {
    //Serial.println(F("erreur d'envoi"));
  }
  radio.startListening(); // On se remet en mode écoute
}

// ----------------------------------------------------------------------------------------
// vérifie si on a reçu une commande de la part de l'autre radio (1 octet)
// ----------------------------------------------------------------------------------------
uint8_t ecouterRadio()
{
  uint8_t message; // 0 = pas de commande

  if ( radio.available()) {
    while (radio.available()) {
      radio.read( &message, sizeof(message) );  // on lit l'octet reçu (si plusieurs messages on ne conserve que le dernier)
    }
    Serial.print(F("J'ai recu ")); Serial.println(message);
  }
  return message;
}

// ------------------------------------------------------------------
// ------------------------------------------------------------------
// ------------------------------------------------------------------

void setup() {


  Serial.begin(115200);

  role =  0 ;
  Serial.print(F("\nMon Role = ")); Serial.println(role);

  // On configure la radio
  radio.begin();
  // pour le test on règle le niveau d'énergie à RF24_PA_LOW pour éviter les interférences
  // mettre à RF24_PA_MAX si on veut la puissance d'émission max
  radio.setPALevel(RF24_PA_HIGH);

  // On ouvre un pipe de lecture et un d'écriture avec des noms opposés en fonction du rôle
  // comme ça un parle sur "pipe0" et l'autre écoute sur "pipe0"
  // et l'autre parle sur "pipe1" tandisque Le premier écoute sur "pipe1"

  radio.openWritingPipe(adresses[0]); // role doit être 0 ou 1
  radio.openReadingPipe(1, adresses[1]); // 1 - role = l'autre adresse

  // Start the radio listening for data
  //radio.startListening();


  // INIT Manette
  #if !defined(__MIPSEL__)
    while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
  #endif
  if (Usb.Init() == -1) {
    Serial.println(F("\r\nOSC did not start"));
    while (1); // Halt
  }
  Serial.println(F("\r\nPS4 USB Library Started"));


	// VARIABLES pour le programme 
	ancienEtatL3 = 0;
	decollage = true;
}

// ------------------------------------------------------------------



uint8_t valeurAccelerateurs;
uint8_t L1Press;
uint8_t R1Press;
uint8_t L3Press;
uint8_t angleTangage;
uint8_t angleRoulis;
long compteur(0);

DONNEES msg;



void loop() {


  Usb.Task();
  
  if (PS4.connected()) {
	
	
	// REGARDER SI LE TRAIN D'ATTERRISSAGE DOIT RESTER SORTI OU RENTRER
	if ( PS4.getButtonClick(L3))
	{
		decollage = !decollage;
	}
    
	// ACCELERATEUR
    msg.Acc = PS4.getAnalogButton(R2);
	
	// ANGLE LACET
    msg.LUn = PS4.getButtonPress(L1);
    msg.RUn = PS4.getButtonPress(R1);
	
	// ANGLE ROULIS
    msg.YRoulis = PS4.getAnalogHat(LeftHatY);
	
	// ANGLE TANGAGE
    msg.XRoulis = PS4.getAnalogHat(LeftHatX);
	
	// SORTIE DU TRAIN D'ATTERRISSAGE
	msg.decollage = decollage;
	
    // ENVOI DU MESSAGE PAR LE MODULE NRF24L01+
    envoyerMessage(msg);
    delay(25);
    Serial.print("[");
    Serial.print(msg.Acc);
    Serial.print(",");
    Serial.print(msg.LUn);
    Serial.print(",");
    Serial.print(msg.RUn);
    Serial.print(",");
    Serial.print(msg.XRoulis);
    Serial.print(",");
    Serial.print(msg.YRoulis);
    Serial.print(",");
    Serial.print(msg.decollage);
    Serial.println("]");

  }
  
}
