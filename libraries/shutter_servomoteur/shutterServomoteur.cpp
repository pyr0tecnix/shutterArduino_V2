/*shutterServomoteur.cpp
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

*/

#include "shutterServomoteur.h"


ShutterServomoteur::ShutterServomoteur() {}

Servo ShutterServomoteur::_servo[NOMBRESERVOMAX];
unsigned int ShutterServomoteur::_nombreServoActif = 0;
/*Pin utilisés par les servomoteurs : 2, 3, 6 et 7*/
const unsigned int ShutterServomoteur::_pinServomoteur[] = {2, 3, 6, 7};
unsigned int ShutterServomoteur::_position[NOMBRESERVOMAX];


void ShutterServomoteur::init(unsigned int nombreServo) {
  ShutterSerial::print("(ShutterServomoteur) init", STACK, true);

  ShutterSerial::print("Initialisation des servomoteurs ", DEBUG, true);
  int cpt = 0;
  _nombreServoActif = nombreServo;
  ShutterSerial::print("Activation de ", DEBUG, false);
  ShutterSerial::print(_nombreServoActif, DEBUG, false);
  ShutterSerial::print(" servomoteurs sur ", DEBUG, false);
  ShutterSerial::print(NOMBRESERVOMAX, DEBUG, true);

  for(cpt = 0; cpt < _nombreServoActif; cpt++) {
    _servo[cpt].attach(_pinServomoteur[cpt]);
    /*On intialise chaque servomoteur à la position POSITIONREPOS*/
    move(cpt, POSITIONREPOS);
  }
  ShutterSerial::print(".. Ok", DEBUG, true);
}

void ShutterServomoteur::move(unsigned int indexServo, unsigned int angle) {
  ShutterSerial::print("(ShutterServomoteur) move", STACK, true);
  _servo[indexServo].write(angle);
  _position[indexServo] = angle;
  ShutterSerial::print("Servomoteur ", DEBUG, false);
  ShutterSerial::print((indexServo + 1), DEBUG, false);
  ShutterSerial::print(" en position ", DEBUG, false);
  ShutterSerial::print(angle, DEBUG, true);
}

void ShutterServomoteur::move(unsigned int indexServo, unsigned int angle, unsigned int speed) {
  ShutterSerial::print("(ShutterServomoteur) move - varSpeed", STACK, true);
  int pas;
  int wait;
  /*Réglage en fonction de la vitesse*/
  if(speed == FULLSPEED) {
    move(indexServo, angle);
    return;
  }
  if(speed == MEDIUMSPEED) {
    pas = 5;
    wait = 100;
  }
  if(speed == LOWSPEED) {
    pas = 2;
    wait = 150;
  }

  if(_position[indexServo] < angle) {
    while((_position[indexServo] + 2*pas) < angle) {
      _position[indexServo] += pas;
      move(indexServo, _position[indexServo]);
      delay(wait);
    }
    _position[indexServo] = angle;
    move(indexServo, _position[indexServo]);
  }
  if(_position[indexServo] > angle) {
    while((_position[indexServo] - 2*pas) > angle) {
      _position[indexServo] -= pas;
      move(indexServo, _position[indexServo]);
      delay(wait);
    }
    _position[indexServo] = angle;
    move(indexServo, _position[indexServo]);
  }
}

void ShutterServomoteur::open(unsigned int indexServo) {
  ShutterSerial::print("(ShutterServomoteur) open", STACK, true);
  move(indexServo, POSITIONREPOS);
}

void ShutterServomoteur::close(unsigned int indexServo) {
  ShutterSerial::print("(ShutterServomoteur) close", STACK, true);
  move(indexServo, POSITIONTRAVAIL);
}


void ShutterServomoteur::test(unsigned int indexServo) {
  ShutterSerial::print("(ShutterServomoteur) test servo", STACK, true);
  ShutterSerial::print("Test du servomoteur ", DEBUG, false);
  ShutterSerial::print((indexServo + 1), DEBUG, true);
  move(indexServo, POSITIONTRAVAIL);
  delay(1000);
  move(indexServo, POSITIONREPOS);
}

void ShutterServomoteur::test() {
  ShutterSerial::print("(shutterServomoteur) test", STACK, true);
  int cpt = 0;
  for(cpt = 0; cpt < _nombreServoActif; cpt++) {
    test(cpt);
  }
}

unsigned int ShutterServomoteur::getPosition(unsigned int indexServo) {
  ShutterSerial::print("(ShutterServomoteur) getPosition", STACK, true);
  return _position[indexServo];
}
