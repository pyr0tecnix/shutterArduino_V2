/*
shutterOSC.h
Dernière modification : 28/04/16
© Patrice Vieyra - contact@magicofthings.fr

Librairie contenant les fonctions relatives à l'OSC

Librairie utilisée : OSC for Arduino and Related Microcontrollers 1.3c
Lien : http://cnmat.berkeley.edu/oscuino
https://github.com/CNMAT/OSC


Glossaire des trames :

/i/shutter_on/            Passe le servomoteur i à l'état actif (cache devant la lentille du VP)
/i/shutter_off/           Passe le servomoteur i à l'état inactif (cache relevé)
/i/shuter_position/       Requiert la position du servomoteur i. Le shutter répond /i/shutter_move/position et /i/label_position
                          Ces deux trames permettent respectivement de régler la position du fader et la valeur du label
                          correspondant au servomoteur i sur TouchOSC
/shutter_reboot/          Reboot le shutter
/i/shutter_move/          Déplace le servomoteur i à la position passée en argument. Attention les bornes de déplacement
                          sont propres à chaque modèle de servomoteur
/shutter_ping/            Le shutter renvoie pong
/shutter_runservotest/    Lance la procédure de test des servomoteurs



Méthodes
  Publiques

  - Constructeur : Non utilisé, la classe est statique
  - bool init(IPAddress master_osc_Ip, unsigned int master_osc_Port, IPAddress shutter_osc_Ip, unsigned int shutter_osc_Port, byte* shutter_osc_Mac) :
    Initialisation de la communication OSC.
    Paramètres :
      1. Addresse IP du master. Appareil à qui le shutter enverra les trames OSC
      2. Port utilisé par le master pour recevoir des trames OSC
      3. Addresse IP du shutter
      4. Port utilisé par le shuter pour recevoir des trames OSC
      5. Addresse MAC du module Ethernet du shutter
    Retour : True si l'initialisation s'est bien passée, false sinon
  - void receive() : Réception des trames OSC via le socket UDP
  - void send(OSCMessage &msg) : Permet d'envoyer des messages OSC via UDP

  Privées
  - void dispatch(OSCMessage msgIN) : Méthode permettant de router les trames OSC vers les méthodes correspondantes
  - unsigned int getOcsMessageAddress(OSCMessage &msg) : Renvoie l'addresse contenue dans le message OSC

  - void close(OSCMessage &msg, int addrOffset) : Traite la commande de fermeture
  - void open(OSCMessage &msg, int addrOffset) : Traite la commande d'ouverture
  - void getPosition(OSCMessage &msg, int addrOffset) : Traite la demande de position
  - void reboot(OSCMessage &msg, int addrOffset) : Traite la commande de reboot
  - void move(OSCMessage &msg, int addrOffset) : Traite la commande de déplacement
  - void ping(OSCMessage &msg, int addrOffset) : Traite la commande ping
  - void test(OSCMessage &msg, int addrOffset) : Traite la commande de test servo


Attributs
  Privés

  - static EthernetUDP _udp : permet d'envoyer et de recevoir des paquets UDP
  - IPAddress _master_osc_Ip : adresse IP de l'appareil maître
  - unsigned int _master_osc_Port : port de communication OSC de l'ordinateur maître
*/

#ifndef shutterOSC
#define shutterOSC
/*Libraire Arduino qui gère l'ethernet*/
#include <Ethernet.h>
/*Librairie Arduino qui gère l'UDP*/
#include <EthernetUdp.h>
/*Librairie OSC qui gére l'envoi et la réception de message  */
#include <OSCBundle.h>
/*Libraire Arduino qui gère les servomoteurs*/
#include <Servo.h>

/*Librairie gérant l'affichage sur la liaison série*/
#include <ShutterSerial.h>
/*Librairie gérant la diode de notification*/
#include <ShutterLED.h>
/*Librairie gérant les servomoteurs*/
#include <shutterServomoteur.h>
/*Librairie gérant les fonctions propres à l'Arduino*/
#include <shutterCommande.h>

/*PIN de l'arduino utilisé par la carte SD*/
#define SDCARD_CS 4

class ShutterOSC
{
  public :
    /*Constructeur*/
    ShutterOSC();
    static bool init(IPAddress master_osc_Ip, unsigned int master_osc_Port, IPAddress shutter_osc_Ip, unsigned int shutter_osc_Port, byte* shutter_osc_Mac);
    static void receive();
    static void send(OSCMessage &msg);

  private :

    static EthernetUDP         _udp;
    static IPAddress           _master_osc_Ip;
    static unsigned int        _master_osc_Port;

    static void dispatch(OSCMessage msgIN);
    static unsigned int getOcsMessageAddress(OSCMessage &msg);
    /*Commandes OSC*/
    static void close(OSCMessage &msg, int addressOffset);
    static void open(OSCMessage &msg, int addressOffset);
    static void getPosition(OSCMessage &msg, int addressOffset);
    static void reboot(OSCMessage &msg, int addressOffset);
    static void move(OSCMessage &msg, int addressOffset);
    static void ping(OSCMessage &msg, int addressOffset);
    static void test(OSCMessage &msg, int addressOffset);
};

#endif
