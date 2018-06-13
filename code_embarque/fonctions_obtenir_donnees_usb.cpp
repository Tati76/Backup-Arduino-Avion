#include "fonctions_obtenir_donnees_usb.h"

DONNEES obtenirDonneesUsb()
{
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
	
}

