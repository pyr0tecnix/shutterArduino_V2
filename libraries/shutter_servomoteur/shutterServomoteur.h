/*shutterServomoteur.h
Dernière modification : 31/03/16
© Patrice Vieyra - contact@magicofthings.fr

Librairie ayant pour but de gérer les servomoteurs

Méthodes
  Publiques

    - Constructeur : Instanciation de l'objet. Pas utilisé dans notre cas, la classe
      est statique
    - void init(unsigned int nombreServo) : Initialise les servomoteurs sélectionnés
      Les nombre maximum de servomoteurs est fixé par le define NOMBRESERVOMAX
      Les pins utilisés sont stockés dans le tableau _pinServomoteur : 2, 3, 6, 7
		- void move(unsigned int indexServo, unsigned int angle) : Déplace le
      servomoteur indexServo à la position angle à la vitesse maximale
		- move(unsigned int indexServo, unsigned int angle, unsigned int speed) :
      Déplace le servomoteur indexServo à la position angle à la vitesse speed (FULL, MEDIUM, LOW)
		- void open(unsigned int indexServo) : Déplace le servomoteur indexServo à la position POSITIONREPOS
    - void close(unsigned int indexServo) : Déplace le servomoteur indexServo à la position POSITIONTRAVAIL
    - void test(unsigned int indexServo) : Appelle la méthode move(indexServo, POSITIONTRAVAIL) puis
      move(indexServo, POSITIONREPOS) 1s après
    - void test() : Appelle les méthodes move(i, POSITIONTRAVAIL) de chaque servomoteur actif, puis
      move(i, POSITIONREPOS) de chaque servomoteur actif 1s après
    - unsigned int getPosition(unsigned int indexServo) : Renvoie la position du servomoteur indexServo

Attributs
  Privés

    - Servo _servo[NOMBRESERVOMAX] : Tableau contenant les servomoteurs utilisés par le shutter
    - unsigned int _nombreServoActif : Nombre de servomoteur actif. Attribut initialisé par la méthode init
    - unsigned int _position[NOMBRESERVOMAX] : Tableau contenant les positions des servomoteurs
    - const unsigned int _pinServomoteur[NOMBRESERVOMAX] : Tableau contenant les pins utilisés par les servomoteurs
*/

#ifndef shutterServomoteur
#define shutterServomoteur

#include <Servo.h>
#include <ShutterSerial.h>
/*Permet d'utiliser les fonctions propres à l'arduino*/
#if defined(ARDUINO) && ARDUINO >= 100
      #include "Arduino.h"
    #else
      #include "WProgram.h"
    #endif

#define NOMBRESERVOMAX 4
#define POSITIONREPOS 5
#define POSITIONTRAVAIL 105

/*Enumération contenant les différentes vitesses disponibles pour le déplacement
des servomoteurs*/
enum Speed {
  FULLSPEED,
  MEDIUMSPEED,
  LOWSPEED,
  };

class ShutterServomoteur {

  public:

    /*Constructeur*/
    ShutterServomoteur();

    static void init(unsigned int nombreServo);

    static void move(unsigned int indexServo, unsigned int angle);
    static void move(unsigned int indexServo, unsigned int angle, unsigned int speed);

    static void open(unsigned int indexServo);
    static void close(unsigned int indexServo);

    static void test(unsigned int indexServo);
    static void test();

    static unsigned int getPosition(unsigned int indexServo);

;


  private:

    static Servo _servo[NOMBRESERVOMAX];
    static unsigned int _nombreServoActif;
    static unsigned int _position[NOMBRESERVOMAX];
    static const unsigned int _pinServomoteur[NOMBRESERVOMAX];
};
#endif
