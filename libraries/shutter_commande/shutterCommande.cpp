/*shutterCommande.cpp
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

#include "shutterCommande.h"

bool ShutterCommande::statutService[NOMBREDESERVICE];

ShutterCommande::ShutterCommande() {}

void ShutterCommande::reboot() {
  ShutterSerial::print("(ShutterCommande) reboot", STACK, true);
  ShutterSerial::print("Le shutter va redémarrer dans 1s.. ", INFO, true);
  delay(2000);
  SCB->AIRCR = ((0x5FA << SCB_AIRCR_VECTKEY_Pos) | SCB_AIRCR_SYSRESETREQ_Msk);
}

String ShutterCommande::IPAddressToString(IPAddress ip) {
  ShutterSerial::print("(ShutterCommande) IPAddressToString", STACK, true);
  String str = String(ip[0]);
  str += ".";
  str += String(ip[1]);
  str += ".";
  str += String(ip[2]);
  str += ".";
  str += String(ip[3]);
  return str;
}

String ShutterCommande::MACAddressToString(byte* mac) {
  ShutterSerial::print("(ShutterCommande) MACAddressToString", STACK, true);
  /*Le 16 permet de préciser la base utilisée, l'hexadécimal ici*/
  String str = String(mac[0], 16);
  str += ":";
  str += String(mac[1], 16);
  str += ":";
  str+= String(mac[2], 16);
  str += ":";
  str +=String(mac[3], 16);
  str += ":";
  str +=String(mac[4], 16);
  str += ":";
  str +=String(mac[5], 16);
  return str;
}
