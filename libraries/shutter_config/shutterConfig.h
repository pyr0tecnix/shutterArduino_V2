/*shutterConfig.h
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


Attributs
  Privé
    - IPAddress     _shutter_osc_Ip : Adresse IP du shutter
    - IPAddress     _master_osc_Ip : Adresse IP du master OSC
    - byte          _shutter_osc_Mac[6] : Adresse MAC du module Ethernet du shutter
    - unsigned int  _shutter_osc_Port : Port utilisé par le shutter pour la communication OSC
    - unsigned int  _master_osc_Port : Port utilisé par le master pour la communication OSC
    - char         _shutter_bonjour_name[SHUTTERBONJOURNAMESIZE] : Identifiant Bonjour du shutter
    - char         _shutter_bonjour_service_osc_name[SHUTTERBONJOURSERVICENAMESIZE] : Nom du service OSC bonjour : _shutter_bonjour_name._osc
    - char         _shutter_bonjour_service_http_name[SHUTTERBONJOURSERVICENAMESIZE] : Nom du service HTTP bonjour : f_shutter_bonjour_name._http
    - char         _shutter_control_mode[SHUTTERCONTROLMODESIZE] : Mode de pilotage du shutter. Seul le mode OSC est disponible actuellement
    - bool          _shutter_http_server_activate : Activation du webserver qui sert à configurer le shutter à distance
    - unsigned int  _shutter_servo_nombre : Nombre de servomoteurs actifs. Entre 1 et 4
    - unsigned int  _shutter_verbosite_level : Verbosité du shutter, (ERROR, INFO, STACK, DEBUG)

*/

#ifndef shutterConfig
#define shutterConfig
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <String.h>
#include <shutterSerial.h>
#include <ShutterCommande.h>

#define SHUTTERBONJOURNAMESIZE 50
#define SHUTTERCONTROLMODESIZE 10
#define SHUTTERBONJOURSERVICENAMESIZE 30

class ShutterConfig {

  public:
    ShutterConfig();

    /*Getter*/
    static IPAddress    getShutterOscIp();
    static IPAddress    getMasterOscIp();
    static unsigned int getShutterOscPort();
    static unsigned int getMasterOscPort();
    static byte*        getShutterOscMac();
    static char*        getShutterBonjourName();
    static char*        getShutterControlMode();
    static char*        getShutterBonjourServiceOSCName();
    static char*        getShutterBonjourServiceHTTPName();
    static unsigned int getShutterServoNombre();
    static unsigned int getShutterVerbositeLevel();
    static bool         getShutterHTTPServerActivate();

    /*Setter*/
    static void setShutterOscIp(IPAddress ip);
    static void setMasterOscIp(IPAddress ip);
    static void setShutterOscPort(unsigned int port);
    static void setMasterOscPort(unsigned int port);
    static void setShutterOscMac(char* mac);
    static void setShutterBonjourName(char* nom);
    static void setShutterControlMode(char* mode);
    static void setShutterServoNombre(unsigned int nombre);
    static void setShutterVerbositeLevel(unsigned int level);
    static void setShutterHTTPServerActivate(bool activate);

    static void print();


  private:
    static IPAddress     _shutter_osc_Ip;
    static IPAddress     _master_osc_Ip;
    static byte          _shutter_osc_Mac[6];
    static unsigned int  _shutter_osc_Port;
    static unsigned int  _master_osc_Port;
    static char          _shutter_bonjour_name[];
    static char          _shutter_bonjour_service_osc_name[];
    static char          _shutter_bonjour_service_http_name[];
    static char          _shutter_control_mode[];
    static bool          _shutter_http_server_activate;
    static unsigned int  _shutter_servo_nombre;
    static unsigned int  _shutter_verbosite_level;

};
#endif
