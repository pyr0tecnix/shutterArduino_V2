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

*/


#include "shutterOSC.h"

/*Constructeur*/
ShutterOSC::ShutterOSC() {
}

/*Déclaration des attributs statiques*/
EthernetUDP ShutterOSC::_udp;
IPAddress ShutterOSC::_master_osc_Ip;
unsigned int  ShutterOSC::_master_osc_Port;

/*Buffer de réception des trames UDP*/
unsigned char packetBuffer[UDP_TX_PACKET_MAX_SIZE];

bool ShutterOSC::init(IPAddress master_osc_Ip, unsigned int master_osc_Port, IPAddress shutter_osc_Ip, unsigned int shutter_osc_Port, byte* shutter_osc_Mac) {
  ShutterSerial::print("(shutterOSC) init", STACK, true);
  ShutterSerial::print("Initialisation OSC...", DEBUG, false);
  _master_osc_Ip = master_osc_Ip;
  _master_osc_Port = master_osc_Port;

  /*On inhibe la carte SD*/
  pinMode(SDCARD_CS, OUTPUT);
  digitalWrite(SDCARD_CS, HIGH);

  Ethernet.begin(shutter_osc_Mac, shutter_osc_Ip);
  if(!_udp.begin(shutter_osc_Port)) {
    ShutterSerial::print("OSC - Echec initialisation", ERROR, true);
    ShutterSerial::print("OSC - Vérifier que le câble ethernet est bien branché, que l'adresse IP et l'adresse mac ne sont pas déjà utilisées.", ERROR, true);
    ShutterLED::setCouleur(RED);
    return false;
  }
  ShutterSerial::print("IP locale : ", DEBUG, false);
  ShutterSerial::print(ShutterCommande::IPAddressToString(Ethernet.localIP()), DEBUG, true);
  ShutterSerial::print("Ok", DEBUG, true);
  return true;
}

void ShutterOSC::receive() {
  OSCMessage msgIN;
  int packetSize = _udp.parsePacket();
  if (packetSize)
  {
    ShutterSerial::print("Réception d'un paquet de ", DEBUG, false);
    ShutterSerial::print(packetSize, DEBUG, false);
    ShutterSerial::print(" bits.", DEBUG, true);

    ShutterSerial::print("De ", DEBUG, false);
    IPAddress remote = _udp.remoteIP();
    for (int i = 0; i < 4; i++)
    {
      ShutterSerial::print(remote[i], DEBUG, false);
      if (i < 3)
      {
        ShutterSerial::print(".", DEBUG, false);
      }
    }
    ShutterSerial::print(", port ", DEBUG, false);
    ShutterSerial::print(_udp.remotePort(), DEBUG, true);

    _udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    ShutterSerial::print("Contenu : ", DEBUG, false);
    ShutterSerial::print((char*)packetBuffer, DEBUG, true);
    /*Remplissage du message OSC avec le buffer UDP*/
    msgIN.fill(packetBuffer, packetSize);
    if(!msgIN.hasError()) {
      ShutterOSC::dispatch(msgIN);
    }
    else {
      ShutterSerial::print("Erreur réception OSC : ", ERROR, false);
      ShutterSerial::print(msgIN.getError(), ERROR, true);
      ShutterLED::setCouleur(RED);
    }
  }
}

void ShutterOSC::send(OSCMessage &msg) {
  ShutterSerial::print("(shutterOSC - sendOSC)", STACK, true);
  _udp.beginPacket(_master_osc_Ip, _master_osc_Port);
  msg.send(_udp);
  _udp.endPacket();
  msg.empty();
  ShutterSerial::print("Message OSC envoyé", DEBUG, true);
}

void ShutterOSC::dispatch(OSCMessage msgIN) {
  ShutterSerial::print("(shutterOSC) dispatch", STACK, true);
  msgIN.route("/*/shutter_on", close);
  msgIN.route("/*/shutter_off", open);

  msgIN.route("/*/shutter_position", getPosition);
  msgIN.route("/*/shutter_move", move);

  msgIN.route("/shutter_reboot", reboot);
  msgIN.route("/shutter_ping", ping);
  msgIN.route("/shutter_runservotest", test);
}

unsigned int ShutterOSC::getOcsMessageAddress(OSCMessage &msg) {
  unsigned int retour = 0;
  if(msg.fullMatch("/1/*")) {
    retour = 0;
  }
  else if(msg.fullMatch("/2/*")) {
    retour = 1;
  }
  else if(msg.fullMatch("/3/*")){
    retour = 2;
  }
  else if(msg.fullMatch("/4/*")) {
    retour = 3;
  }
  return retour;
}


/*OSC Commandes*/

void ShutterOSC::close(OSCMessage &msg, int addressOffset) {
  ShutterSerial::print("(shutterOSC) close", STACK, true);
  unsigned int indexServo = getOcsMessageAddress(msg);
  ShutterSerial::print("Fermeture du servomoteur ", DEBUG, false);
  ShutterSerial::print((indexServo + 1), DEBUG, true);
  ShutterServomoteur::close(indexServo);
}

void ShutterOSC::open(OSCMessage &msg, int addressOffset) {
  ShutterSerial::print("(shutterOSC) open", STACK, true);
  unsigned int indexServo = getOcsMessageAddress(msg);
  ShutterSerial::print("Ouverture du servomoteur ", DEBUG, false);
  ShutterSerial::print((indexServo + 1), DEBUG, true);
  ShutterServomoteur::open(indexServo);
}

void ShutterOSC::getPosition(OSCMessage &msg, int addressOffset) {
  ShutterSerial::print("(shutterOSC) getPosition", STACK, true);
  unsigned int indexServo = getOcsMessageAddress(msg);
  unsigned int positionServo = ShutterServomoteur::getPosition(indexServo);
  ShutterSerial::print("Demande de position du servomoteur ", DEBUG, false);
  ShutterSerial::print((indexServo + 1), DEBUG, true);
  char address[20];
  sprintf(address, "/%d/label_position", (indexServo + 1));
  /*Envoie de la position au label*/
  OSCMessage message(address);
  message.add(positionServo);
  send(message);
  /*Envoie de la position au fader*/
  sprintf(address, "/%d/shutter_move", (indexServo + 1));
  message.setAddress(address);
  message.add(positionServo);
  send(message);
  ShutterSerial::print("Servomoteur ", DEBUG, false);
  ShutterSerial::print((indexServo + 1), DEBUG, false);
  ShutterSerial::print(" en position ", DEBUG, false);
  ShutterSerial::print(positionServo, DEBUG, true);
}

void ShutterOSC::move(OSCMessage &msg, int addressOffset) {
  ShutterSerial::print("(shutterOSC moveShutter)", STACK, true);
  unsigned int indexServo = getOcsMessageAddress(msg);

  ShutterSerial::print("Déplacement du servomoteur ", DEBUG, false);
  ShutterSerial::print((indexServo + 1), DEBUG, false);
  ShutterSerial::print(" à la position ", DEBUG, false);
  ShutterSerial::print((int)msg.getFloat(0), DEBUG, true);
  ShutterServomoteur::move(indexServo, (int)msg.getFloat(0));
}
void ShutterOSC::ping(OSCMessage &msg, int addressOffset) {
  ShutterSerial::print("(shutterOSC) ping", STACK, true);

  ShutterSerial::print("Ping .. ", INFO, false);
  OSCMessage message("/shutter_pong");
  send(message);
  ShutterSerial::print("Pong", INFO, true);
}

void ShutterOSC::test(OSCMessage &msg, int addressOffset) {
  ShutterSerial::print("(shutterOSC) test", STACK, true);
  ShutterServomoteur::test();
}

void ShutterOSC::reboot(OSCMessage &msg, int addressOffset) {
  ShutterSerial::print("(shutterOSC) reboot", STACK, true);
  ShutterCommande::reboot();
}
