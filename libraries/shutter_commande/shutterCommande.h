/*shutterCommande.h
Dernière modification : 01/04/16
© Patrice Vieyra - contact@magicofthings.fr

Librairie ayant pour but de contenir les fonctions propres à la gestion de l'arduino
et des utilitaires
Méthodes
  Publiques

    - Constructeur : Instanciation de l'objet. Pas utilisé dans notre cas, la classe
      est statique
    - void reboot() : Reboot l'Arduino après avoir attendu 2s
    - String IPAddressToString(IPAddress ip) : Convertit une adresse IP en String
    - String MACAddressToString(byte* mac) : Convertit un byte* en String

*/

#ifndef shutterCommande
#define shutterCommande

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <shutterSerial.h>
#include <Ethernet.h>

#define SERVICESD 0
#define SERVICEOSC 1
#define SERVICEBONJOUR 2

#define NOMBREDESERVICE 3

class ShutterCommande {

  public:

    /*Constructeur*/
    ShutterCommande();

    static void reboot();
    static String IPAddressToString(IPAddress ip);
    static String MACAddressToString(byte* mac);

    static bool statutService[];
};
#endif
