#include "fonctions_transmettre_consigne_moteurs.h"
#include "fonctions_print.h"

void transmettreConsigneMoteurs(DONNEES &message)
{
  
    //SERVO ANGLE LACET
	if (message.LUn == 0 && message.RUn == 1 )
	{
		servoEmpLacet.write(Constantes::ANGLE_SECU_SERVO + Constantes::ANGLE_MAX_LACET); 
	}
	else if (message.LUn == 1 && message.RUn == 0 )
	{
		servoEmpLacet.write(Constantes::ANGLE_SECU_SERVO + Constantes::ANGLE_MIN_LACET);
	}
	else 
	{
		servoEmpLacet.write(Constantes::ANGLE_SECU_SERVO); 
	}
  
	
	//SERVO AILE DROITE
    servoAileDroite.write(map(255-message.XRoulis, 0, 255, Constantes::ANGLE_MIN_AILETTE_AILE_DROITE + Constantes::ANGLE_SECU_SERVO, Constantes::ANGLE_MAX_AILETTE_AILE_DROITE + Constantes::ANGLE_SECU_SERVO)); 
    
	  
	//SERVO AILE GAUCHE
    servoAileGauche.write(map(255-message.XRoulis, 0, 255, Constantes::ANGLE_MIN_AILETTE_AILE_GAUCHE + Constantes::ANGLE_SECU_SERVO, Constantes::ANGLE_MAX_AILETTE_AILE_GAUCHE + Constantes::ANGLE_SECU_SERVO)); 
	
	//MOTEUR
	  moteurExtDroite.writeMicroseconds(map(message.Acc, 0, 255, Constantes::ACC_MIN_MOTEUR, Constantes::ACC_MAX_MOTEUR)); 
    moteurIntDroite.writeMicroseconds(map(message.Acc, 0, 255, Constantes::ACC_MIN_MOTEUR, Constantes::ACC_MAX_MOTEUR)); 
    moteurIntGauche.writeMicroseconds(map(message.Acc, 0, 255, Constantes::ACC_MIN_MOTEUR, Constantes::ACC_MAX_MOTEUR)); 
    moteurExtGauche.writeMicroseconds(map(message.Acc, 0, 255, Constantes::ACC_MIN_MOTEUR, Constantes::ACC_MAX_MOTEUR)); 

	//SERVO ROULIS angle vertical joystick
    servoDroiteStab.write(map(message.YRoulis, 0, 255, Constantes::ANGLE_MIN_ROULIS + Constantes::ANGLE_SECU_SERVO, Constantes::ANGLE_MAX_ROULIS + Constantes::ANGLE_SECU_SERVO)); 
	servoGaucheStab.write(map(message.YRoulis, 0, 255, Constantes::ANGLE_MIN_ROULIS + Constantes::ANGLE_SECU_SERVO, Constantes::ANGLE_MAX_ROULIS + Constantes::ANGLE_SECU_SERVO));
	
	//Train d'atterissage
	if (message.decollage == 1)
	{
		trainAtterissage.write(Constantes::ANGLE_MAX_TRAIN_ATTERISSAGE); // ON ABAISSE LE TRAIN D'ATTERISSAGE
		servoAideDecollageGauche.write(Constantes::ANGLE_SECU_SERVO + Constantes::ANGLE_MAX_VOLET_HYPERSUSTENTATEUR); // ON ABAISSE LES VOLETS POUR AUGMENTER LA PORTANCE
		servoAideDecollageDroite.write(Constantes::ANGLE_SECU_SERVO - Constantes::ANGLE_MAX_VOLET_HYPERSUSTENTATEUR);
	}
	else
	{
		trainAtterissage.write(Constantes::ANGLE_MIN_TRAIN_ATTERISSAGE); // ON REMONTE LE TRAIN D'ATTERISSAGE
		servoAideDecollageGauche.write(Constantes::ANGLE_SECU_SERVO); // ON REMONTE LES VOLETS POUR AUGMENTER LA PORTANCE 
		servoAideDecollageDroite.write(Constantes::ANGLE_SECU_SERVO);
	}
	temps = millis();
}



void testFunction()
{
  servoEmpLacet.write(Constantes::ANGLE_SECU_SERVO);  
  servoAileDroite.write(Constantes::ANGLE_SECU_SERVO);  
  servoAileGauche.write(Constantes::ANGLE_SECU_SERVO);  
  servoAideDecollageGauche.write(Constantes::ANGLE_SECU_SERVO);  
  servoAideDecollageDroite.write(Constantes::ANGLE_SECU_SERVO);  
  moteurExtDroite.writeMicroseconds(Constantes::ACC_TEST_MOTEUR);
  moteurIntDroite.writeMicroseconds(Constantes::ACC_TEST_MOTEUR);
  moteurIntGauche.writeMicroseconds(Constantes::ACC_TEST_MOTEUR);
  moteurExtGauche.writeMicroseconds(Constantes::ACC_TEST_MOTEUR);
  servoGaucheStab.write(Constantes::ANGLE_SECU_SERVO);  
  servoDroiteStab.write(Constantes::ANGLE_SECU_SERVO);  
  trainAtterissage.write(Constantes::ANGLE_SECU_SERVO);  
  
  temps = millis();
  while((millis()-temps) < 2000)
  {
    printEtatSorties();
  }
  
  servoEmpLacet.write(Constantes::ANGLE_SECU_SERVO + 20);  
  servoAileDroite.write(Constantes::ANGLE_SECU_SERVO + 20);  
  servoAileGauche.write(Constantes::ANGLE_SECU_SERVO + 20);  
  servoAideDecollageGauche.write(Constantes::ANGLE_SECU_SERVO + 20);  
  servoAideDecollageDroite.write(Constantes::ANGLE_SECU_SERVO + 20);  
  moteurExtDroite.writeMicroseconds(Constantes::ACC_MIN_MOTEUR);
  moteurIntDroite.writeMicroseconds(Constantes::ACC_MIN_MOTEUR);
  moteurIntGauche.writeMicroseconds(Constantes::ACC_MIN_MOTEUR);
  moteurExtGauche.writeMicroseconds(Constantes::ACC_MIN_MOTEUR);
  servoGaucheStab.write(Constantes::ANGLE_SECU_SERVO + 20);  
  servoDroiteStab.write(Constantes::ANGLE_SECU_SERVO + 20);  
  trainAtterissage.write(Constantes::ANGLE_SECU_SERVO + 20); 
    
  temps = millis();
  while((millis()-temps) < 2000)
  {
    printEtatSorties();
  }

}

void initESC()
{
	// Initialisation de l'ESC
	//  (certains ESC ont besoin d'une "procédure d'initialisation"
	//   pour devenir opérationnels - voir notice)
	moteurExtDroite.writeMicroseconds(Constantes::ACC_MAX_MOTEUR);
	moteurIntDroite.writeMicroseconds(Constantes::ACC_MAX_MOTEUR);
	moteurIntGauche.writeMicroseconds(Constantes::ACC_MAX_MOTEUR);
	moteurExtGauche.writeMicroseconds(Constantes::ACC_MAX_MOTEUR);
	delay(2000);
	moteurExtDroite.writeMicroseconds(Constantes::ACC_MIN_MOTEUR);
	moteurIntDroite.writeMicroseconds(Constantes::ACC_MIN_MOTEUR);
	moteurIntGauche.writeMicroseconds(Constantes::ACC_MIN_MOTEUR);
	moteurExtGauche.writeMicroseconds(Constantes::ACC_MIN_MOTEUR);
	delay(3000);
}

void arretUrgence()
{
  servoEmpLacet.write(Constantes::ANGLE_SECU_SERVO);  
  servoAileDroite.write(Constantes::ANGLE_SECU_SERVO);  
  servoAileGauche.write(Constantes::ANGLE_SECU_SERVO);  
  servoAideDecollageGauche.write(Constantes::ANGLE_SECU_SERVO);  
  servoAideDecollageDroite.write(Constantes::ANGLE_SECU_SERVO);  
  moteurExtDroite.writeMicroseconds(Constantes::ACC_MIN_MOTEUR);
  moteurIntDroite.writeMicroseconds(Constantes::ACC_MIN_MOTEUR);
  moteurIntGauche.writeMicroseconds(Constantes::ACC_MIN_MOTEUR);
  moteurExtGauche.writeMicroseconds(Constantes::ACC_MIN_MOTEUR);
  servoGaucheStab.write(Constantes::ANGLE_SECU_SERVO);  
  servoDroiteStab.write(Constantes::ANGLE_SECU_SERVO);  
  trainAtterissage.write(Constantes::ANGLE_SECU_SERVO); 
  
  Serial.println("Arrêt d'Urgence !!");
}