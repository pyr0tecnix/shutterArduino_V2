/*shutterLED.h
Dernière modification : 03/05/16
© Patrice Vieyra - contact@magicofthings.fr

Librairie ayant pour but de gérer la diode de notification

Méthodes
  Publiques

    - Constructeur : Instanciation de l'objet. Pas utilisé dans notre cas, la classe
      est statique
    - void init() : Initialise la LED de notification sur la couleur verte
    - void setCouleur(unsigned int couleur) : Fixe la couleur de la diode de notification
      à couleur. La couleur est définie par une énumération (BLACK, WHITE, RED, GREEN, BLUE)
    - void blink() : Fait clignoter la LED de notification

Attributs
  Publiques

    - Couleur _couleur : Variable contenant la couleur actuelle de la diode de notification
*/
#ifndef shutterLED
#define shutterLED

#include <ShutterSerial.h>
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define RED_PIN 11
#define GREEN_PIN 12
#define BLUE_PIN 13

enum Couleur {
  BLACK,
  WHITE,
  RED,
  GREEN,
  BLUE,
  };

class ShutterLED {

	public:
		/*Constructeur*/
		ShutterLED();

		static void init();
    static void setCouleur(unsigned int couleur);
    static void blink();

    static Couleur _couleur;
};
#endif
