/*shutterLED.cpp
Dernière modification : 01/04/16
© Patrice Vieyra - contact@magicofthings.fr

Librairie ayant pour but de gérer la diode de notification

Méthodes
  Publiques

    - Constructeur : Instanciation de l'objet. Pas utilisé dans notre cas, la classe
      est statique
    - void init() : Initialise la LED de notification sur la couleur verte
    - void setCouleur(unsigned int couleur) : Fixe la couleur de la diode de notification
      à couleur. La couleur est définie par une énumération (BLACK, WHITE, RED, GREEN, BLUE)

*/


#include "shutterLED.h"

/*Constructeur*/
ShutterLED::ShutterLED() {
}

void ShutterLED::init() {
	ShutterSerial::print("(ShutterLED) init", STACK, true);
	ShutterSerial::print("Initialisation LED...", DEBUG, false);
	pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
	setCouleur(GREEN);
	ShutterSerial::print("Ok", DEBUG, true);


}

void ShutterLED::setCouleur(unsigned int couleur) {
	ShutterSerial::print("(ShutterLED) setCouleur", STACK, true);
	ShutterSerial::print("Couleur : ", DEBUG, false);
	ShutterSerial::print(couleur, DEBUG, true);

	switch (couleur) {
		case BLACK :
		digitalWrite(RED_PIN, HIGH);
		digitalWrite(GREEN_PIN, HIGH);
		digitalWrite(BLUE_PIN, HIGH);
		break;

		case WHITE :
		digitalWrite(RED_PIN, LOW);
		digitalWrite(GREEN_PIN, LOW);
		digitalWrite(BLUE_PIN, LOW);
		break;

		case RED :
		digitalWrite(RED_PIN, LOW);
		digitalWrite(GREEN_PIN, HIGH);
		digitalWrite(BLUE_PIN, HIGH);
		break;

		case GREEN :
		digitalWrite(RED_PIN, HIGH);
		digitalWrite(GREEN_PIN, LOW);
		digitalWrite(BLUE_PIN, HIGH);
		break;

		case BLUE :
		digitalWrite(RED_PIN, HIGH);
		digitalWrite(GREEN_PIN, HIGH);
		digitalWrite(BLUE_PIN, LOW);
		break;
	}

}
