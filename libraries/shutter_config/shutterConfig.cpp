/*shutterConfig.cpp
Dernière modification : 28/04/16
© Patrice Vieyra - contact@magicofthings.fr

Librairie ayant pour but de gérer les servomoteurs

Méthodes
  Publiques

    - Constructeur : Instanciation de l'objet. Pas utilisé dans notre cas, la classe
      est statique

    - IPAddress getShutterOscIp() : Renvoie l'adresse IP du shutter
    - IPAddress getMasterOscIp() : Renvoie l'adresse IP du Master
    - unsigned int getShutterOscPort() : Renvoie le port utilisé par le shutter pour communiquer en OSC
    - unsigned int getMasterOscPort() : Renvoie le port utilisé par le master pour communiquer en OSC
    - byte*        getShutterOscMac() : Renvoie l'adresse MAC du module Ethernet du shutter
    - char*        getShutterBonjourName() : Renvoie l'identifiant Bonjour du shutter
    - char*        getShutterBonjourServiceOSCName() : Renvoie le nom du service OSC présent sur le shutter
    - char*        getShutterBonjourServiceHTTPName() : Renvoie le nom du service HTTP présent sur le shutter
    - unsigned int getShutterServoNombre() : Renvoie le nombre de servomoteurs actifs du shutter
    - unsigned int getShutterVerbositeLevel() : Renvoie le niveau de verbosité actuel du shutter (ERROR, INFO, STACK, DEBUG)
    - unsigned int getShutterHTTPServerActivate() : Renvoie un booléen indiquant si le service web est actif

    - void setShutterOscIp(IPAddress) : Fixe l'adresse IP du shutter
    - void setMasterOscIp(IPAddress) : Fixe l'adresse IP du Master
    - void setShutterOscPort(unsigned int) : Fixe le port utilisé par le shutter pour communiquer en OSC
    - void setMasterOscPort(unsigned int) : Fixe le port utilisé par le master pour communiquer en OSC
    - void setShutterOscMac(byte*) : Fixe l'adresse MAC du module Ethernet du shutter
    - void setShutterBonjourName(char*) : Fixe l'identifiant Bonjour du shutter
    - void setShutterBonjourServiceOSCName(char*) : Fixe le nom du service OSC présent sur le shutter
    - void setShutterBonjourServiceHTTPName(char*) : Fixe le nom du service HTTP présent sur le shutter
    - void setShutterServoNombre(unsigned int) : Fixe le nombre de servomoteurs actifs du shutter
    - void setShutterVerbositeLevel(unsigned int) : Fixe le niveau de verbosité actuel du shutter (ERROR, INFO, STACK, DEBUG)
    - void setShutterHTTPServerActivate(unsigned int) : Fixe un booléen indiquant si le service web est actif

    - void print() : Affiche la configuration actuelle
    - String IPAddressToString(IPAddress ip) : Convertit une adresse IP en String
    - String MACAddressToString(byte* mac) : Convertit un byte* en String

*/
#include "shutterConfig.h"
ShutterConfig::ShutterConfig() {}

/*Initialisation des variables statiques*/
IPAddress     ShutterConfig::_shutter_osc_Ip;
IPAddress     ShutterConfig::_master_osc_Ip;
byte          ShutterConfig::_shutter_osc_Mac[6];
unsigned int  ShutterConfig::_shutter_osc_Port = 0;
unsigned int  ShutterConfig::_master_osc_Port = 0;
char          ShutterConfig::_shutter_bonjour_name[SHUTTERBONJOURNAMESIZE];
char          ShutterConfig::_shutter_bonjour_service_osc_name[SHUTTERBONJOURSERVICENAMESIZE];
char          ShutterConfig::_shutter_bonjour_service_http_name[SHUTTERBONJOURSERVICENAMESIZE];
char          ShutterConfig::_shutter_control_mode[SHUTTERCONTROLMODESIZE];
bool          ShutterConfig::_shutter_http_server_activate;
unsigned int  ShutterConfig::_shutter_servo_nombre = 0;
unsigned int  ShutterConfig::_shutter_verbosite_level = 0;

/*Getter*/
IPAddress    ShutterConfig::getShutterOscIp(){return _shutter_osc_Ip;}
IPAddress    ShutterConfig::getMasterOscIp(){return _master_osc_Ip;}
byte*        ShutterConfig::getShutterOscMac(){return _shutter_osc_Mac;}
unsigned int ShutterConfig::getShutterOscPort(){return _shutter_osc_Port;}
unsigned int ShutterConfig::getMasterOscPort(){return _master_osc_Port;}
char*        ShutterConfig::getShutterBonjourName(){return _shutter_bonjour_name;}
char*        ShutterConfig::getShutterBonjourServiceOSCName(){return _shutter_bonjour_service_osc_name;}
char*        ShutterConfig::getShutterBonjourServiceHTTPName(){return _shutter_bonjour_service_http_name;}
char*        ShutterConfig::getShutterControlMode(){return _shutter_control_mode;}
unsigned int ShutterConfig::getShutterServoNombre() {return _shutter_servo_nombre;}
unsigned int ShutterConfig::getShutterVerbositeLevel(){return _shutter_verbosite_level;}
bool         ShutterConfig::getShutterHTTPServerActivate(){return _shutter_http_server_activate;}


/*Setter*/
void ShutterConfig::setShutterHTTPServerActivate(bool activate) {
  ShutterSerial::print("(ShutterConfig) setShutterHTTPServerActivate", STACK, true);
  ShutterSerial::print("Set _shutter_http_server_activate.. ", DEBUG, false);
  _shutter_http_server_activate = activate;
  ShutterSerial::print("Ok", DEBUG, true);
}
void ShutterConfig::setShutterOscIp(IPAddress ip) {
  ShutterSerial::print("(ShutterConfig) setShutterOscIp", STACK, true);
  ShutterSerial::print("Set _shutter_osc_Ip.. ", DEBUG, false);
  _shutter_osc_Ip = ip;
  ShutterSerial::print("Ok", DEBUG, true);
}
void ShutterConfig::setMasterOscIp(IPAddress ip) {
  ShutterSerial::print("(ShutterConfig) setMasterOscIp", STACK, true);
  ShutterSerial::print("Set _master_osc_Ip.. ", DEBUG, false);
  _master_osc_Ip = ip;
  ShutterSerial::print("Ok", DEBUG, true);
}
void ShutterConfig::setShutterOscMac(char* mac) {
  ShutterSerial::print("(ShutterConfig) setShutterOscMac", STACK, true);
  char *ptr, *t;
  ShutterSerial::print("Set _shutter_osc_Mac.. ", DEBUG, false);

  /*Strtok permet de découper la chaîne fournie en paramètre suivant le délimitateur indiqué
  La chaîne fournie en paramètre est modifiée par la fonction
  On renvoie un pointeur vers le dernier token trouvé dans la châine
  */
  t = strtok(mac, ":");
  /*Strtol permet de transformer une chaînte de caractère en long en précisant la base*/
  _shutter_osc_Mac[0] = strtol(t, &ptr, 16);

  t = strtok(NULL, ":");
  _shutter_osc_Mac[1] = strtol(t, &ptr, 16);

  t = strtok(NULL, ":");
  _shutter_osc_Mac[2] = strtol(t, &ptr, 16);

  t = strtok(NULL, ":");
  _shutter_osc_Mac[3] = strtol(t, &ptr, 16);

  t = strtok(NULL, ":");
  _shutter_osc_Mac[4] = strtol(t, &ptr, 16);

  t = strtok(NULL, ":");
  _shutter_osc_Mac[5] = strtol(t, &ptr, 16);

  ShutterSerial::print("Ok", DEBUG, true);
}
void ShutterConfig::setShutterOscPort(unsigned int port) {
  ShutterSerial::print("(ShutterConfig) setShutterOscPort", STACK, true);
  ShutterSerial::print("Set _shutter_osc_Port.. ", DEBUG, false);
  _shutter_osc_Port = port;
  ShutterSerial::print("Ok", DEBUG, true);
}
void ShutterConfig::setMasterOscPort(unsigned int port) {
  ShutterSerial::print("(ShutterConfig) setMasterOscPort", STACK, true);
  ShutterSerial::print("Set _master_osc_Port.. ", DEBUG, false);
  _master_osc_Port = port;
  ShutterSerial::print("Ok", DEBUG, true);
}

void ShutterConfig::setShutterBonjourName(char* nom) {
  ShutterSerial::print("(ShutterConfig) setShutterBonjourName", STACK, true);
  ShutterSerial::print("Set _shutter_bonjour_name.. ", DEBUG, false);
  sprintf(_shutter_bonjour_name, "%s", nom);
  ShutterSerial::print("Ok", DEBUG, true);

  /*Set Bonjour Service Name*/
  ShutterSerial::print("Set _shutter_bonjour_service_osc_name.. ", DEBUG, false);
  sprintf(_shutter_bonjour_service_osc_name, "%s._osc", nom);
  ShutterSerial::print("Ok", DEBUG, true);

  ShutterSerial::print("Set _shutter_bonjour_service_http_name.. ", DEBUG, false);
  sprintf(_shutter_bonjour_service_http_name, "%s._http", nom);
  ShutterSerial::print("Ok", DEBUG, true);

}

void ShutterConfig::setShutterControlMode(char* mode) {
  ShutterSerial::print("(ShutterConfig) setShutterControlMode", STACK, true);
  ShutterSerial::print("Set _shutter_control_mode.. ", DEBUG, false);
  for(int i = 0; i < strlen(mode); i++) {
    _shutter_control_mode[i] = tolower(mode[i]);
  }
  ShutterSerial::print("Ok", DEBUG, true);
}

void ShutterConfig::setShutterServoNombre(unsigned int nombre) {
  ShutterSerial::print("(ShutterConfig) setShutterServoNombre", STACK, true);
  ShutterSerial::print("Set _shutter_servo_nombre.. ", DEBUG, false);
  _shutter_servo_nombre = nombre;
  ShutterSerial::print("Ok", DEBUG, true);
}

void ShutterConfig::setShutterVerbositeLevel(unsigned int level) {
  ShutterSerial::print("(ShutterConfig) setShutterVerbositeLevel", STACK, true);
  ShutterSerial::print("Set _shutter_verbosite_level.. ", DEBUG, false);
  _shutter_verbosite_level = level;
  ShutterSerial::print("Ok", DEBUG, true);
}

void ShutterConfig::print() {
  ShutterSerial::print("(ShutterConfig) print", STACK, true);
  ShutterSerial::print("Affichage du fichier de configuration", INFO, true);
  ShutterSerial::print("Mode de contrôle : ", INFO, false);
  ShutterSerial::print(_shutter_control_mode, INFO, true);
  ShutterSerial::print("Verbosité  : ", INFO, false);
  ShutterSerial::print((int)_shutter_verbosite_level, INFO, true);

  if(strcmp(_shutter_control_mode, "osc") == 0) {
    ShutterSerial::print("-- BONJOUR", INFO, true);

    ShutterSerial::print("ID Bonjour : ", INFO, false);
    ShutterSerial::print(_shutter_bonjour_name, INFO, true);

    ShutterSerial::print("Service Bonjour : ", INFO, false);
    ShutterSerial::print(_shutter_bonjour_service_osc_name, INFO, true);

    ShutterSerial::print("-- OSC", INFO, true);

    ShutterSerial::print("Shutter Mac : ", INFO, false);
    ShutterSerial::print(ShutterCommande::MACAddressToString(_shutter_osc_Mac), INFO, true);

    ShutterSerial::print("Shutter IP : ", INFO, false);
    ShutterSerial::print(ShutterCommande::IPAddressToString(_shutter_osc_Ip), INFO, false);
    ShutterSerial::print(":", INFO, false);
    ShutterSerial::print((int)_shutter_osc_Port, INFO, true);

    ShutterSerial::print("Master IP : ", INFO, false);
    ShutterSerial::print(ShutterCommande::IPAddressToString(_master_osc_Ip), INFO, false);
    ShutterSerial::print(":", INFO, false);
    ShutterSerial::print((int)_master_osc_Port, INFO, true);
  }
  ShutterSerial::print("-- SERVO", INFO, true);
  ShutterSerial::print("Nombre de servomoteur : ", INFO, false);
  ShutterSerial::print((int)_shutter_servo_nombre, INFO, true);

  if(_shutter_http_server_activate) {
    ShutterSerial::print("-- HTTP", INFO, true);
    ShutterSerial::print("ID Bonjour : ", INFO, false);
    ShutterSerial::print(_shutter_bonjour_name, INFO, true);

    ShutterSerial::print("Service Bonjour : ", INFO, false);
    ShutterSerial::print(_shutter_bonjour_service_http_name, INFO, true);
  }
  ShutterSerial::print("-----", INFO, true);

}
