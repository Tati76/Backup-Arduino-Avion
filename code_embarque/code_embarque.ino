// ************* La Radio *************
#include <SPI.h>
#include <RF24.h> // voir http://tmrh20.github.io/RF24/
#include <Servo.h>


//#define LED_PIN (4)  

// Variables pour les angles des ailettes
#define ANGLE_MAX_AILETTE_AILE_GAUCHE 60
#define ANGLE_MIN_AILETTE_AILE_GAUCHE -60
#define ANGLE_MAX_AILETTE_AILE_DROITE 60
#define ANGLE_MIN_AILETTE_AILE_DROITE -60 
#define ANGLE_MAX_AIDE_DECOLLAGE 60
#define ANGLE_MAX_LACET 50 //R1 | L1
#define ANGLE_MIN_LACET -50 //R1 | L1
#define ANGLE_MAX_ROULIS 50 // angle vertical joystick
#define ANGLE_MIN_ROULIS -50 // angle vertical joystick
#define ACC_MAX_MOTEUR 2000
#define ACC_MIN_MOTEUR 1000
#define ANGLE_MAX_TRAIN_ATTERISSAGE 170
#define ANGLE_MIN_TRAIN_ATTERISSAGE 10
#define ANGLE_SECU_SERVO 90

//#define PAS_SERVO 5

// Configurer vos radio nRF24L01+ sur le bus SPI et mettre  CE sur D7 et CSN sur D8
RF24 radio(7, 8);

// Le nom des "pipes" de communication, un en lecture, un en écriture
const byte adresses[][6] = {"0pipe", "1pipe"}; // Pipes 1-5 should share the same address, except the first byte. Only the first byte in the array should be unique

// A CONFIGURER sur la pin A0
// si A0 est à GND alors rôle = 0 --> le premier Arduino
// si A0 est à  3.3V ou 5V alors rôle = 1 --> pour le second
const byte configurationPin = A0;
uint8_t role;





unsigned long temps;

// ********************************** SERVO *************************************

Servo servoEmpLacet;
Servo servoAileDroite;
Servo servoAileGauche;
Servo servoAideDecollageGauche;
Servo servoAideDecollageDroite;
Servo moteurExtDroite;
Servo moteurIntDroite;
Servo moteurIntGauche;
Servo moteurExtGauche;
Servo servoRoulis; // angle vertical joystick
Servo servoDroiteStab;
Servo servoGaucheStab;
Servo trainAtterissage;


// *********************************** RADIO ************************************

typedef struct{
  uint8_t Acc; //moteur
  uint8_t LUn; // servoEmpLacet 
  uint8_t RUn; // servoEmpLacet 
  uint8_t XRoulis; // servoAileGauche et servoAileDroite
  uint8_t YRoulis; // servoDroiteStab et servoGaucheStab
  bool decollage; // si train d'atterissage alors aide à l'atterissage/decollage
  
} Donnees;


void fairePrintMessage(Donnees &message)
{
  Serial.print("[");
  Serial.print(message.Acc);
  Serial.print(",");
  Serial.print(message.LUn);
  Serial.print(",");
  Serial.print(message.RUn);
  Serial.print(",");
  Serial.print(message.XRoulis);
  Serial.print(",");
  Serial.print(message.YRoulis);
  Serial.print(",");
  Serial.print(message.decollage);
  Serial.print("]");
}


// ----------------------------------------------------------------------------------------
// envoi d'un octet vers l'autre radio
// ----------------------------------------------------------------------------------------
long compteur(0);
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
Donnees ecouterRadio()
{
  Donnees message; // 0 = pas de commande

  if ( radio.available()) {
    while (radio.available()) {
      radio.read( &message, sizeof(Donnees) );  // on lit l'octet reçu (si plusieurs messages on ne conserve que le dernier)
    }
    fairePrintMessage(&message);
  
    //SERVO ANGLE LACET
	if (message.LUn == 0 && message.RUn == 255 )
	{
		servoEmpLacet.write(ANGLE_SECU_SERVO+ANGLE_MAX_LACET); 
	}
	else if (message.LUn == 255 && message.RUn == 0 )
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
    moteur.writeMicroseconds(map(message.Acc, 0, 255, ACC_MIN_MOTEUR, ACC_MAX_MOTEUR)); 
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
		servoAideDecollageGauche.write(ANGLE_SECU_SERVO + ANGLE_MAX_AIDE_DECOLLAGE); // ON ABAISSE LES VOLETS POUR AUGMENTER LA PORTANCE
    servoAideDecollageDroite.write(ANGLE_SECU_SERVO - ANGLE_MAX_AIDE_DECOLLAGE);
	}
	else
	{
		trainAtterissage.write(ANGLE_MIN_TRAIN_ATTERISSAGE); // ON REMONTE LE TRAIN D'ATTERISSAGE
		servoAideDecollageGauche.write(ANGLE_SECU_SERVO); // ON REMONTE LES VOLETS POUR AUGMENTER LA PORTANCE 
    servoAideDecollageDroite.write(ANGLE_SECU_SERVO);
	}


  /*Serial.print("   ||   [[");
  Serial.print(servoEmpLacet.read());
  Serial.print(",");
  Serial.print(servoAileDroite.read());
  Serial.print(",");
  Serial.print(servoAileGauche.read());
  Serial.print(",");
  Serial.print(servoAideDecollageGauche.read());
  Serial.print(",");
  Serial.print(servoAideDecollageDroite.read());
  Serial.print(",");
  Serial.print(servoGaucheStab.read());
  Serial.print(",");
  Serial.print(servoDroiteStab.read());
  Serial.print(",");
  Serial.print(decollage.read());
  Serial.println("]]");*/
  
	temps = millis();
	
  }
  return message;
}

// ------------------------------------------------------------------
// ------------------------------------------------------------------
// ------------------------------------------------------------------


void arretUrgence()
{
	servoEmpLacet.write(ANGLE_SECU_SERVO);   
  servoAileDroite.write(ANGLE_SECU_SERVO);  
  servoAileGauche.write(ANGLE_SECU_SERVO);  
  servoAideDecollageGauche.write(ANGLE_SECU_SERVO); 
  servoAideDecollageDroite.write(ANGLE_SECU_SERVO);     
  moteurExtDroite.writeMicroseconds(ACC_MIN_MOTEUR);
  moteurIntDroite.writeMicroseconds(ACC_MIN_MOTEUR);
  moteurIntGauche.writeMicroseconds(ACC_MIN_MOTEUR);
  moteurExtGauche.writeMicroseconds(ACC_MIN_MOTEUR); 
  servoDroiteStab.write(ANGLE_SECU_SERVO);  
  servoGaucheStab.write(ANGLE_SECU_SERVO);
  trainAtterissage.write(0); 
  delay(2000);
  servoEmpLacet.write(10);  
  servoAileDroite.write(10);  
  servoAileGauche.write(10);  
  servoAideDecollageGauche.write(10); 
  servoAideDecollageDroite.write(10);     
  moteurExtDroite.writeMicroseconds(ACC_MIN_MOTEUR);
  moteurIntDroite.writeMicroseconds(ACC_MIN_MOTEUR);
  moteurIntGauche.writeMicroseconds(ACC_MIN_MOTEUR);
  moteurExtGauche.writeMicroseconds(ACC_MIN_MOTEUR); 
  servoDroiteStab.write(10);  
  servoGaucheStab.write(10);
  trainAtterissage.write(10); 
}









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

  // INITIALISATION DES MODULES nrf24l01+
  role =  1 ;
  Serial.print(F("\nMon Role = ")); Serial.println(role);

  // CONFIGURATION DE LA RADIO
  radio.begin();
  // pour le test on règle le niveau d'énergie à RF24_PA_LOW pour éviter les interférences
  
  // mettre à RF24_PA_MAX si on veut la puissance d'émission max
  radio.setPALevel(RF24_PA_MIN);
  //radio.setDataRate(RF24_250KBPS);
  
  radio.openWritingPipe("1pipe"); // role doit être 0 ou 1
  radio.openReadingPipe(1, "0pipe"); // 1 - role = l'autre adresse
  
  // Start the radio listening for data
  radio.startListening();

  // INITIALISATION DU COMPTEUR DE TEMPS
  temps = millis();
  
  //moteur.writeMicroseconds(ACC_MAX_MOTEUR);
  //delay(1000);
  moteur.writeMicroseconds(ACC_MIN_MOTEUR);
  delay(2000);

  delay(2000);
}

// ------------------------------------------------------------------

void testFunction()
{
    servoEmpLacet.write(ANGLE_SECU_SERVO);  
    servoAileDroite..write(ANGLE_SECU_SERVO);  
    servoAileGauche.write(ANGLE_SECU_SERVO);  
    servoAideDecollageGauche.write(ANGLE_SECU_SERVO);  
    servoAideDecollageDroite.write(ANGLE_SECU_SERVO);  
    moteurExtDroite.attach(ACC_MIN_MOTEUR);
    moteurIntDroite.attach(ACC_MIN_MOTEUR);
    moteurIntGauche.attach(ACC_MIN_MOTEUR);
    moteurExtGauche.attach(ACC_MIN_MOTEUR);
    servoGaucheStab.write(ANGLE_SECU_SERVO);  
    servoDroiteStab.write(ANGLE_SECU_SERVO);  
    trainAtterissage.write(ANGLE_SECU_SERVO);  

    delay(5000);

    servoEmpLacet.write(ANGLE_SECU_SERVO+20);  
    servoAileDroite..write(ANGLE_SECU_SERVO+20);  
    servoAileGauche.write(ANGLE_SECU_SERVO+20);  
    servoAideDecollageGauche.write(ANGLE_SECU_SERVO+20);  
    servoAideDecollageDroite.write(ANGLE_SECU_SERVO+20);  
    moteurExtDroite.attach(ACC_MAX_MOTEUR);
    moteurIntDroite.attach(ACC_MAX_MOTEUR);
    moteurIntGauche.attach(ACC_MAX_MOTEUR);
    moteurExtGauche.attach(ACC_MAX_MOTEUR);
    servoGaucheStab.write(ANGLE_SECU_SERVO+20);  
    servoDroiteStab.write(ANGLE_SECU_SERVO+20);  
    trainAtterissage.write(ANGLE_SECU_SERVO+20);  
}

void loop() {

    testFunction();
    
    /*ecouterRadio();
      
    if ((millis()-temps) > 3000)
    {
		arretUrgence();
    Serial.println("AU");
    }*/
}
