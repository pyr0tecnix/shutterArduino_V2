/*
shutter.cpp
Dernière modification : 03/05/16
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



#include "shutter.h"


Shutter::Shutter() {}



/*TODO : Indiquer que l'init de LED et SD se faisant avant la lecture de la config, le réglage de la verbosité se fait en dur*/
void Shutter::init() {

	/*Initialisation de la liaison série*/
	ShutterSerial::begin(115200);
	/*Initialisation du degré de verbosité*/
	ShutterSerial::setVerboseLevel(DEBUG);

	ShutterSerial::print("Shutter MoT - version ", INFO, false);
	ShutterSerial::print(VERSION, INFO, true);
	ShutterSerial::print("Initialisation du shutter..", INFO, false);

	/*Initialisation de la LED de notification*/
	ShutterLED::init();
	/*Initialisation de la carte SD et stockage de l'état*/
	ShutterCommande::statutService[SERVICESD] = ShutterSD::init();
	/*S'il y a une erreur on bloque l'exécution*/
	if(!ShutterCommande::statutService[SERVICESD]){
		while(1);
	}
	/*Lecture de la configuration*/
	ShutterJSON::setConfig(ShutterJSON::decode(ShutterSD::read_charArray("config.txt", true), true));
	/*Redéfinition du degré de verbosité*/
	ShutterSerial::setVerboseLevel(ShutterConfig::getShutterVerbositeLevel());
	/*Initialisation des servomoteurs*/
	ShutterServomoteur::init(ShutterConfig::getShutterServoNombre());
	ShutterWebserver::init();

	/*Initialisation du serveur web*/
	if(ShutterConfig::getShutterHTTPServerActivate()) {
		ShutterWebserver::init();
	}
	/*Initialisation du mode de commande*/
	if(strcmp(ShutterConfig::getShutterControlMode(), "osc") == 0) {
		/*Initialisation de l'OSC et stockage de l'état*/
		ShutterCommande::statutService[SERVICEOSC] = ShutterOSC::init(ShutterConfig::getMasterOscIp(), ShutterConfig::getMasterOscPort(), ShutterConfig::getShutterOscIp(), ShutterConfig::getShutterOscPort(), ShutterConfig::getShutterOscMac());
		if(!ShutterCommande::statutService[SERVICEOSC]){
			while(1);
		}
		/*Initialisation du protocole Bonjour/Zeroconf et stockage de l'état*/
		ShutterCommande::statutService[SERVICEBONJOUR] = ShutterBonjour::init(ShutterConfig::getShutterBonjourName());
		/*Ajout du service OSC*/
		ShutterBonjour::addService(ShutterConfig::getShutterBonjourServiceOSCName(), ShutterConfig::getShutterOscPort(), true);
		/*Ajout du service HTTP*/
		if(ShutterConfig::getShutterHTTPServerActivate()) {
			ShutterBonjour::addService(ShutterConfig::getShutterBonjourServiceHTTPName(), 80, false);
		}
	}
	ShutterSerial::print("Ok", INFO, true);
	/*Affichage de la configuration*/
	ShutterConfig::print();

	/*Notification visuelle de fin d'initialisation*/
	ShutterLED::blink();
}


/*Lancement de l'application
Liste des modules ayant besoin d'un appel en boucle
- module OSC
- module Bonjour
- module HTTP

Paramètes : aucun
Retour : aucun
*/
void Shutter::run() {
	if(strcmp(ShutterConfig::getShutterControlMode(), "osc") == 0) {
		ShutterOSC::receive();
		ShutterBonjour::run();
	}
	if(ShutterConfig::getShutterHTTPServerActivate()) {
		ShutterWebserver::run();
	}
}
