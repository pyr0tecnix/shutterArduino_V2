/*shutterJSON.cpp
Dernière modification : 01/05/16
© Patrice Vieyra - contact@magicofthings.fr

Librairie ayant pour but de l'encodage et le décodage de châine de caractères en JSON

Librairie utilisée : ArduinoJson
Lien : https://github.com/bblanchon/ArduinoJson/

Méthodes
  Publiques

  - Constructeur : Instanciation de l'objet. Pas utilisé dans notre cas, la classe
  est statique
  - JsonObject& decode(char* json, bool isCritical) : Décode la chaîne de caractère passée en argument et
    renvoie un objet JSON contenant les données, le paramètre isCritical permet d'indiquer si l'opération est critique
    (en cas d'erreur on passe la LED de notification au rouge et on bloque l'éxecution du programme)
  - void setConfig(JsonObject& jsonConfig) : Initialise la classe de configuration avec
  les valeurs issues de l'objet JSON
*/
#include "shutterJSON.h"



ShutterJSON::ShutterJSON() {}



JsonObject& ShutterJSON::decode(char* json, bool isCritical){
  ShutterSerial::print("(ShutterJSON) decode", STACK, true);
  ShutterSerial::print("Parsing du Json..", DEBUG, false);

  /*Buffer de lecture JSON*/
  StaticJsonBuffer<2000> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  if (!root.success()) {
    ShutterSerial::print("Echec du parsing du JSON", ERROR, true);
    if(isCritical) {
      ShutterLED::setCouleur(RED);
      while(1);
    }
    else {
      ShutterLED::setCouleur(BLUE);
    }
  }
  else  {
    ShutterSerial::print("Affichage du jSON", DEBUG, true);
    for(JsonObject::iterator it=root.begin(); it != root.end(); ++it) {
      ShutterSerial::print("Clé : ", DEBUG, false);
      /*On passe par une variable car on ne peut pas caster it->key directement*/
      const char* key = it->key;
      ShutterSerial::print(key, DEBUG, false);
      ShutterSerial::print(" Valeur : ", DEBUG, false);
      ShutterSerial::print(it->value.as<const char*>(), DEBUG, true);
    }
    ShutterSerial::print("OK", DEBUG, true);
  }
  return root;
}


void ShutterJSON::setConfig(JsonObject& jsonConfig) {
  ShutterSerial::print("(ShutterJSON) setConfig", STACK, true);
  /*Général*/
  char* t;
  const char* temp_0;
  const char* temp_1;
  const char* temp_2;
  const char* temp_3;
  const char* temp_4;
  char* ip0; //Variable temporaire de stockage du byte[0] des adresses IP
  char* ip1; //Variable temporaire de stockage du byte[1] des adresses IP
  char* ip2; //Variable temporaire de stockage du byte[2] des adresses IP
  char* ip3; //Variable temporaire de stockage du byte[3] des adresses IP

  ShutterSerial::print("Lecture des clés..", DEBUG, true);

  temp_0 = jsonConfig["shutter_control_Mode"];
  ShutterConfig::setShutterControlMode(const_cast<char *>(temp_0));
  ShutterConfig::setShutterVerbositeLevel(atoi(jsonConfig["shutter_verbosite_level"]));

  /*OSC*/

  temp_1 = jsonConfig["shutter_osc_Ip"];
  ip0 = strtok_r(const_cast<char*>(temp_1), ".", &t); //Split la variable value en deux chaînes en utilisant comme séparateur le "." Renvoie la partie gauche et stock le reste dans le pointeur t
  ip1 = strtok_r(t, ".", &t);
  ip2 = strtok_r(t, ".", &t);
  ip3 = strtok_r(t, ".", &t);
  ShutterConfig::setShutterOscIp(IPAddress(atoi(ip0), atoi(ip1), atoi(ip2), atoi(ip3)));

  ShutterConfig::setShutterOscPort(atoi(jsonConfig["shutter_osc_Port"]));

  temp_2 = jsonConfig["shutter_osc_Mac"];
  ShutterConfig::setShutterOscMac(const_cast<char*>(temp_2));

  temp_3 = jsonConfig["master_osc_Ip"];
  ip0 = strtok_r(const_cast<char*>(temp_3), ".", &t);
  ip1 = strtok_r(t, ".", &t);
  ip2 = strtok_r(t, ".", &t);
  ip3 = strtok_r(t, ".", &t);
  ShutterConfig::setMasterOscIp(IPAddress(atoi(ip0), atoi(ip1), atoi(ip2), atoi(ip3)));
  ShutterConfig::setMasterOscPort(atoi(jsonConfig["master_osc_Port"]));

  /*Bonjour*/
  temp_4 = jsonConfig["shutter_bonjour_name"];
  ShutterConfig::setShutterBonjourName(const_cast<char*>(temp_4));

  /*Servomoteur*/
  ShutterConfig::setShutterServoNombre(atoi(jsonConfig["shutter_servo_nombre"]));

  /*Serveur HTTP*/
  ShutterConfig::setShutterHTTPServerActivate(atoi(jsonConfig["shutter_http_server_activate"]));
}
