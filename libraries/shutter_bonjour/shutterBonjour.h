/*shutterBonjour.h
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
      Renvoie true si tout c'est bien passé et false dans le cas contraire
    - void sbool addService(char* serviceName, unsigned int port, bool UDPorTCP) : Déclare le service serviceName
      sur le port port et indique si c'est de l'UDP (UDPorTCP = true) ou du TCP (UDPorTCP = false)
      Renvoie true si tout c'est bien passé et false dans le cas contraire
    - void run() : Permet de maintenir le protocole bonjour. Cette méthode doit être appelée dans le boucle loop() de l'Arduino
*/

#ifndef shutterBonjour
#define shutterBonjour

#include <Ethernet.h>
#include <EthernetBonjour.h>
#include <shutterSerial.h>
#include <shutterLED.h>

class ShutterBonjour {

  public:
    /*Constructeur*/
    ShutterBonjour();

    static bool init(char* bonjourName);
    static bool addService(char* serviceName, unsigned int port, bool UDPorTCP);
    static void run();
};
#endif
