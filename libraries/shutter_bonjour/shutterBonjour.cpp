/*shutterBonjour.cpp
Dernière modification : 01/04/16
© Patrice Vieyra - contact@magicofthings.fr

Librairie ayant pour but la gestion du protocole Bonjour/Zeroconf

Librairie utilisée : Bonjour/Zeroconf
Lien : http://gkaindl.com/software/arduino-ethernet/bonjour

Méthodes
  Publiques

    - Constructeur : Instanciation de l'objet. Pas utilisé dans notre cas, la classe
      est statique
    - bool init(char* bonjourName) : Initialise le protocole bonjour avec le nom bonjourName qui identifiera l'appareil sur le réseau
      Renvoie true si tout s'est bien passé et false dans le cas contraire
    - void sbool addService(char* serviceName, unsigned int port, bool UDPorTCP) : Déclare le service serviceName
      sur le port port et indique si c'est de l'UDP (UDPorTCP = true) ou du TCP (UDPorTCP = false)
      Renvoie true si tout c'est bien passé et false dans le cas contraire
    - void run() : Permet de maintenir le protocole bonjour. Cette méthode doit être appelée dans le boucle loop() de l'Arduino
*/

#include "shutterBonjour.h"

ShutterBonjour::ShutterBonjour() {}

bool ShutterBonjour::init(char* bonjourName) {
  ShutterSerial::print("(ShutterBonjour) init", STACK, true);
  bool retour = false;
  ShutterSerial::print("Initialisation du protocole Bonjour...", DEBUG, false);
  if(EthernetBonjour.begin(bonjourName) != 0) {
    ShutterSerial::print("OK", DEBUG, true);
    retour = true;
  }
  else {
    ShutterSerial::print("Erreur d'initialisation du service Bonjour.", ERROR, true);
    retour = false;
    ShutterLED::setCouleur(BLUE);
  }
  return retour;
}

bool ShutterBonjour::addService(char* serviceName, unsigned int port, bool UDPorTCP){
  ShutterSerial::print("(ShutterBonjour) addService", STACK, true);
  bool retour = false;
  ShutterSerial::print("Déclaration du service bonjour ", DEBUG, false);
  ShutterSerial::print(serviceName, DEBUG, false);
  ShutterSerial::print(", port ", DEBUG, false);
  ShutterSerial::print(port, DEBUG, false);
  ShutterSerial::print(((UDPorTCP) ? " UDP" : " TCP"), DEBUG, false);

  if(EthernetBonjour.addServiceRecord(serviceName, port, ((UDPorTCP) ? MDNSServiceUDP : MDNSServiceTCP)) == 0) {
    ShutterSerial::print("Erreur de déclaration du service osc", ERROR, true);
    retour = false;
    ShutterLED::setCouleur(BLUE);
  }
  else {
    ShutterSerial::print(" Ok", DEBUG, true);
    retour = true;
  }
  return retour;
}

void ShutterBonjour::run() {
   EthernetBonjour.run();
}
