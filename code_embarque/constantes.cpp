#include "constantes.h"

namespace Constantes 
{
	// Variables pour les angles des ailettes
	extern const int ANGLE_MAX_AILETTE_AILE_GAUCHE(60);
	extern const int ANGLE_MIN_AILETTE_AILE_GAUCHE(-60);
	extern const int ANGLE_MAX_AILETTE_AILE_DROITE(60);
	extern const int ANGLE_MIN_AILETTE_AILE_DROITE(-60);
	extern const int ANGLE_MAX_VOLET_HYPERSUSTENTATEUR(60);
	extern const int ANGLE_MIN_ROULIS(-60);
	extern const int ANGLE_MAX_ROULIS(60);
	extern const int ANGLE_MAX_LACET(50); //R1 | L1
	extern const int ANGLE_MIN_LACET(-50); //R1 | L1
	extern const int ACC_MAX_MOTEUR(2000);
	extern const int ACC_TEST_MOTEUR(1500);
	extern const int ACC_MIN_MOTEUR(1000);
	extern const int ANGLE_MAX_TRAIN_ATTERISSAGE(170);
	extern const int ANGLE_MIN_TRAIN_ATTERISSAGE(10);
	extern const int ANGLE_SECU_SERVO(90);
	extern const int TIMEOUT_RADIO_EN_MILLISECONDES(3000);
}