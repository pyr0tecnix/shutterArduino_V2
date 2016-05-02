/*
shutter.h
Dernière modification : 28/04/16
© Patrice Vieyra - contact@magicofthings.fr

Librairie contenant l'application shutter

Define

- Version de l'application : 2.1.0

Méthodes
Publiques

- Constructeur : Instanciation de l'objet shutter. Initialisation de l'attribut _config
- void init() : Initialise les modules selon la configuration
- void run() : Lance l'application et appelle les méthodes ayant besoin de tourner en boucle

*/

#ifndef shutter
#define shutter

#define VERSION ((const char*)"2.1.0")

/*Gestion des commandes générales*/
#include <shutterCommande.h>

/*Gestion des messages série*/
#include <shutterSerial.h>

/*Gestion de la LED de notification*/
#include <shutterLED.h>

/*Gestion de la configuration*/
#include <shutterConfig.h>

/*Gestion de la carte SD*/
#include <shutterSD.h>

/*Gestion du JSON*/
#include <shutterJSON.h>

/*Gestion des servomoteurs*/
#include <shutterServomoteur.h>

/*Gestion de Bonjour/Zeroconf*/
#include <shutterBonjour.h>

/*Gestion du serveur Web*/
#include <shutterWebserver.h>

/*Gestion de la communication OSC*/
#include <shutterOSC.h>

class Shutter {

public:
  /*Constructeur*/
  Shutter();

  void init();
  void run();

};
#endif
