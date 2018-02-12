// ************* La Radio *************
#include <SPI.h>
#include <RF24.h> // voir http://tmrh20.github.io/RF24/
#include <Servo.h>



// Variables pour les angles des ailettes
#define ANGLE_MAX_AILETTE_AILE_GAUCHE 60
#define ANGLE_MIN_AILETTE_AILE_GAUCHE -60
#define ANGLE_MAX_AILETTE_AILE_DROITE 60
#define ANGLE_MIN_AILETTE_AILE_DROITE -60 
#define ANGLE_MAX_VOLET_HYPERSUSTENTATEUR 60
#define ANGLE_MIN_ROULIS -60
#define ANGLE_MAX_ROULIS 60
#define ANGLE_MAX_LACET 50 //R1 | L1
#define ANGLE_MIN_LACET -50 //R1 | L1
#define ACC_MAX_MOTEUR 2000
#define ACC_TEST_MOTEUR 1500
#define ACC_MIN_MOTEUR 1000
#define ANGLE_MAX_TRAIN_ATTERISSAGE 170
#define ANGLE_MIN_TRAIN_ATTERISSAGE 10
#define ANGLE_SECU_SERVO 90

unsigned long temps;

// *********************************** STRUCTURE RADIO ************************************

typedef struct {
  uint8_t Acc; //moteur
  uint8_t LUn; // servoEmpLacet 
  uint8_t RUn; // servoEmpLacet 
  uint8_t XRoulis; // servoAileGauche et servoAileDroite
  uint8_t YRoulis; // servoDroiteStab et servoGaucheStab
  bool decollage; // si train d'atterissage alors aide à l'atterissage/decollage
} DONNEES;


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


// *********************************** RADIO ************************************
// Configurer vos radio nRF24L01+ sur le bus SPI et mettre  CE sur D7 et CSN sur D8
RF24 radio(7, 8);
const byte adresses[][6] = {"0pipe", "1pipe"}; 
uint8_t role;
long compteur(0);

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


// ----------------------------------------------------------------------------------------
// vérifie si on a reçu une commande de la part de l'autre radio (1 octet)
// ----------------------------------------------------------------------------------------

void ecouterRadio()
{
  DONNEES message; 

  if ( radio.available()) {
    while (radio.available()) {
      radio.read( &message, sizeof(DONNEES) );  // on lit l'octet reçu (si plusieurs messages on ne conserve que le dernier)
    }

  //printMessageRadio(message);
  
    //SERVO ANGLE LACET
	if (message.LUn == 0 && message.RUn == 1 )
	{
		servoEmpLacet.write(ANGLE_SECU_SERVO+ANGLE_MAX_LACET); 
	}
	else if (message.LUn == 1 && message.RUn == 0 )
	{
		servoEmpLacet.write(ANGLE_SECU_SERVO+ANGLE_MIN_LACET);
	}
	else 
	{
		servoEmpLacet.write(ANGLE_SECU_SERVO); 
	}
  
	
	//SERVO AILE DROITE
    servoAileDroite.write(map(255-message.XRoulis, 0, 255, ANGLE_MIN_AILETTE_AILE_DROITE + ANGLE_SECU_SERVO, ANGLE_MAX_AILETTE_AILE_DROITE + ANGLE_SECU_SERVO)); 
    
	  
	//SERVO AILE GAUCHE
    servoAileGauche.write(map(255-message.XRoulis, 0, 255, ANGLE_MIN_AILETTE_AILE_GAUCHE + ANGLE_SECU_SERVO, ANGLE_MAX_AILETTE_AILE_GAUCHE + ANGLE_SECU_SERVO)); 
	
	//MOTEUR
	  moteurExtDroite.writeMicroseconds(map(message.Acc, 0, 255, ACC_MIN_MOTEUR, ACC_MAX_MOTEUR)); 
    moteurIntDroite.writeMicroseconds(map(message.Acc, 0, 255, ACC_MIN_MOTEUR, ACC_MAX_MOTEUR)); 
    moteurIntGauche.writeMicroseconds(map(message.Acc, 0, 255, ACC_MIN_MOTEUR, ACC_MAX_MOTEUR)); 
    moteurExtGauche.writeMicroseconds(map(message.Acc, 0, 255, ACC_MIN_MOTEUR, ACC_MAX_MOTEUR)); 

	//SERVO ROULIS angle vertical joystick
    servoDroiteStab.write(map(message.YRoulis, 0, 255, ANGLE_MIN_ROULIS+ANGLE_SECU_SERVO, ANGLE_MAX_ROULIS+ANGLE_SECU_SERVO)); 
	  servoGaucheStab.write(map(message.YRoulis, 0, 255, ANGLE_MIN_ROULIS+ANGLE_SECU_SERVO, ANGLE_MAX_ROULIS+ANGLE_SECU_SERVO));
	
	//Train d'atterissage
	if (message.decollage == 1)
	{
		trainAtterissage.write(ANGLE_MAX_TRAIN_ATTERISSAGE); // ON ABAISSE LE TRAIN D'ATTERISSAGE
		servoAideDecollageGauche.write(ANGLE_SECU_SERVO + ANGLE_MAX_VOLET_HYPERSUSTENTATEUR); // ON ABAISSE LES VOLETS POUR AUGMENTER LA PORTANCE
    servoAideDecollageDroite.write(ANGLE_SECU_SERVO - ANGLE_MAX_VOLET_HYPERSUSTENTATEUR);
	}
	else
	{
		trainAtterissage.write(ANGLE_MIN_TRAIN_ATTERISSAGE); // ON REMONTE LE TRAIN D'ATTERISSAGE
		servoAideDecollageGauche.write(ANGLE_SECU_SERVO); // ON REMONTE LES VOLETS POUR AUGMENTER LA PORTANCE 
    servoAideDecollageDroite.write(ANGLE_SECU_SERVO);
	}
	temps = millis();
	
  }
  else {
    //Serial.println("Aucune entrée");
  }
  //return message;
}



// ------------------------------------------------------------------

void testFunction()
{
  servoEmpLacet.write(ANGLE_SECU_SERVO);  
  servoAileDroite.write(ANGLE_SECU_SERVO);  
  servoAileGauche.write(ANGLE_SECU_SERVO);  
  servoAideDecollageGauche.write(ANGLE_SECU_SERVO);  
  servoAideDecollageDroite.write(ANGLE_SECU_SERVO);  
  moteurExtDroite.writeMicroseconds(ACC_TEST_MOTEUR);
  moteurIntDroite.writeMicroseconds(ACC_TEST_MOTEUR);
  moteurIntGauche.writeMicroseconds(ACC_TEST_MOTEUR);
  moteurExtGauche.writeMicroseconds(ACC_TEST_MOTEUR);
  servoGaucheStab.write(ANGLE_SECU_SERVO);  
  servoDroiteStab.write(ANGLE_SECU_SERVO);  
  trainAtterissage.write(ANGLE_SECU_SERVO);  
  
  temps = millis();
  while((millis()-temps) < 2000)
  {
    printEtatSorties();
  }
  
  servoEmpLacet.write(ANGLE_SECU_SERVO+20);  
  servoAileDroite.write(ANGLE_SECU_SERVO+20);  
  servoAileGauche.write(ANGLE_SECU_SERVO+20);  
  servoAideDecollageGauche.write(ANGLE_SECU_SERVO+20);  
  servoAideDecollageDroite.write(ANGLE_SECU_SERVO+20);  
  moteurExtDroite.writeMicroseconds(ACC_MIN_MOTEUR);
  moteurIntDroite.writeMicroseconds(ACC_MIN_MOTEUR);
  moteurIntGauche.writeMicroseconds(ACC_MIN_MOTEUR);
  moteurExtGauche.writeMicroseconds(ACC_MIN_MOTEUR);
  servoGaucheStab.write(ANGLE_SECU_SERVO+20);  
  servoDroiteStab.write(ANGLE_SECU_SERVO+20);  
  trainAtterissage.write(ANGLE_SECU_SERVO+20); 
    
  temps = millis();
  while((millis()-temps) < 2000)
  {
    printEtatSorties();
  }

}

// ------------------------------------------------------------------
// ------------------------------------------------------------------
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
  Serial.print(F("\nMon Role = ")); Serial.println(role);

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
  
  // Initialisation de l'ESC
  //  (certains ESC ont besoin d'une "procédure d'initialisation"
  //   pour devenir opérationnels - voir notice)
  moteurExtDroite.writeMicroseconds(ACC_MAX_MOTEUR);
  moteurIntDroite.writeMicroseconds(ACC_MAX_MOTEUR);
  moteurIntGauche.writeMicroseconds(ACC_MAX_MOTEUR);
  moteurExtGauche.writeMicroseconds(ACC_MAX_MOTEUR);
  delay(2000);
  moteurExtDroite.writeMicroseconds(ACC_MIN_MOTEUR);
  moteurIntDroite.writeMicroseconds(ACC_MIN_MOTEUR);
  moteurIntGauche.writeMicroseconds(ACC_MIN_MOTEUR);
  moteurExtGauche.writeMicroseconds(ACC_MIN_MOTEUR);
  delay(3000);

  // INITIALISATION DU COMPTEUR DE TEMPS
  temps = millis();
  
}



void loop() {
  
  //testFunction();
  ecouterRadio();
  printEtatSorties();
  /*if ((millis()-temps) > 3000)
  {
    //arretUrgence();
    Serial.println("AU");
  }*/
}
