/*shutterJSON.h
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

#ifndef shutterJSON
#define shutterJSON

#include <SPI.h>
#include <ArduinoJson.h>

#include <shutterSerial.h>
#include <shutterConfig.h>
#include <shutterLED.h>


class ShutterJSON {

  public:

    /*Constructeur*/
    ShutterJSON();

    static JsonObject& decode(char* json, bool isCritical);
    static void setConfig(JsonObject& jsonConfig);

};
#endif
