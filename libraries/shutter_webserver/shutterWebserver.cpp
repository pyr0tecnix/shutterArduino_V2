/*
shutterWebserver.h
Dernière modification : 02/05/16
© Patrice Vieyra - contact@magicofthings.fr

Librairie contenant les fonctions relatives au serveur web

Librairie utilisée : Arduino tiny webserver
Lien : https://webweavertech.com/ovidiu/weblog/archives/000484.html
https://github.com/ovidiucp/TinyWebServer

Méthodes
Publiques

- Constructeur : Instanciation de l'objet. Pas utilisé dans notre cas, la classe
est statique
- void init() : Initialisation dsu serveur WEB
- void run() : Permet de recevoir les requêtes des navigateurs web. La méthode doit être appelée dans la méthode loop de l'Arduino

Privées
- boolean index_handler(TinyWebServer& web_server) : Affiche la page index.htm dans le navigateur client
- boolean file_handler(TinyWebServer& web_server) : Affiche la page demandée dans le navigateur client
- boolean getdata_handler(TinyWebServer& web_server) : Gère l'envoie de données de l'Arduino vers le navigateur client
- boolean setdata_handler(TinyWebServer& web_server) : Gère la réception de données du navigateur client vers l'Arduino
- void file_uploader_handler(TinyWebServer& web_server, TinyWebPutHandler::PutAction action, char* buffer, int size) :
Permet d'uploader des fichiers sur la carte SD en utilisant le script upload.sh
- JsonObject& send_statut() : Retourne un objet jSON contenant le statut des différents services du shutter
- JsonObject& send_console() : Retourne un objet jSON contenant les messages séries de niveau INFO et ERROR
- JsonObject& send_dataForm() : Retourne un objet jSON contenant la configuration actuelle du shutter
- void servomoteur_dispatch(String requete) : Dispatch les commandes envoyées aux servomoteurs (open, close, position)
- void shutter_dispatch(String requete) : Dispatch les commandes envoyées au shutter (reboot, test)
- void receive_formulaire(String requete) : Reçoit la configuration envoyée par le navigateur et l'enregistre sur la carte SD
- void file_upload(String requete) : Reçoit un fichier depuis le navigateur et l'enregistre sur la carte SD
- send_file_name(TinyWebServer& web_server, const char* filename) : Ouvre et renvoie le fichier demandé depuis le navigateur

*/

#include "shutterWebserver.h"

/*Buffer jSON*/
StaticJsonBuffer<2000> json_Buffer;
char index_console[ShutterSerial::_history_size][10];

TinyWebServer::PathHandler ShutterWebserver::_handlers[] = {
  {"/", TinyWebServer::GET, &ShutterWebserver::index_handler },
  {"/upload/" "*", TinyWebServer::PUT, &TinyWebPutHandler::put_handler },
  {"/getdata", TinyWebServer::POST, &ShutterWebserver::getdata_handler },
  {"/setdata", TinyWebServer::POST, &ShutterWebserver::setdata_handler },
  {"/" "*", TinyWebServer::GET, &ShutterWebserver::file_handler },
  {NULL},
};

const char* headers[] = {
  "Content-Length",
  NULL
};

TinyWebServer ShutterWebserver::_web = TinyWebServer(_handlers, headers);

ShutterWebserver::ShutterWebserver() {}

void ShutterWebserver::init() {
  ShutterSerial::print("(ShutterWebserver) init", STACK, true);
  ShutterSerial::print("Initialisation du serveur HTTP...", DEBUG, false);
  /*Affecte le handler d'upload au handler de la librairie TinyWebServer*/
  TinyWebPutHandler::put_handler_fn = ShutterWebserver::file_uploader_handler;
  _web.begin();
  ShutterSerial::print("Ok", DEBUG, true);
}


void ShutterWebserver::run() {
  _web.process();
}

boolean ShutterWebserver::index_handler(TinyWebServer& web_server) {
  ShutterSerial::print("(ShutterWebserver) index_handler", STACK, true);
  send_file_name(web_server, "INDEX.HTM");
  return true;
}

boolean ShutterWebserver::file_handler(TinyWebServer& web_server) {
  ShutterSerial::print("(ShutterWebserver) file_handler", STACK, false);
  char* filename = TinyWebServer::get_file_from_path(web_server.get_path());
  if(!filename) {
    web_server.send_error_code(400);
    web_server << F("Requête invalide");
    return true;
  }
  send_file_name(web_server, filename);
  free(filename);
  return true;
}

void ShutterWebserver::file_uploader_handler(TinyWebServer& web_server, TinyWebPutHandler::PutAction action, char* buffer, int size) {
  static uint32_t total_size;

  switch (action) {
    case TinyWebPutHandler::START:
    total_size = 0;
    if (!ShutterSD::_file.isOpen()) {

      char* fname = web_server.get_file_from_path(web_server.get_path());
      if (fname) {
        ShutterSerial::print("Création du fichier ", DEBUG, false);
        ShutterSerial::print(fname, INFO, true);
        ShutterSD::_file.open(&ShutterSD::_root, fname, O_CREAT | O_WRITE | O_TRUNC);
        free(fname);
      }
    }
    break;

    case TinyWebPutHandler::WRITE:
    if (ShutterSD::_file.isOpen()) {
      ShutterSD::_file.write(buffer, size);
      total_size += size;
    }
    break;

    case TinyWebPutHandler::END:
    ShutterSD::_file.sync();
    ShutterSerial::print("Ecriture de ", DEBUG, false);
    ShutterSerial::print(ShutterSD::_file.fileSize(), DEBUG, false);
    ShutterSerial::print(" octets sur ", DEBUG, false);
    ShutterSerial::print(total_size, DEBUG, false);
    ShutterSerial::print(" octets.", DEBUG, true);
    total_size = 0;
    ShutterSD::_file.close();
  }
}

boolean ShutterWebserver::getdata_handler(TinyWebServer& web_server){
  ShutterSerial::print("(ShutterWebserver) getdata_handler", STACK, true);
  web_server.send_error_code(200);
  web_server.send_content_type("application/json");
  web_server.end_headers();
  /*Récupération de l'argument du POST*/
  Client& client = web_server.get_client();
  char c;
  String requete = "";
  if (client.connected()) {
    while (client.available()) {
      c = (char)client.read();
      requete += c;
    }
  }

  ShutterSerial::print("Requête reçue : ", DEBUG, false);
  ShutterSerial::print(requete, DEBUG, true);

  /*Dispatch*/
  JsonObject& json = json_Buffer.createObject();
  if(requete.indexOf("statut") >= 0) {
    json["statut"] = send_statut();
  }
  if(requete.indexOf("console") >= 0) {
    json["console"] = send_console();
  }
  if(requete.indexOf("formulaire") >= 0) {
    json["formulaire"] = send_dataForm();
  }
  ShutterSerial::print("Réponse JSON : ", DEBUG, true);
  char json_char[2048];
  json.printTo(json_char, 2048);
  ShutterSerial::print(json_char, DEBUG, true);

  /*Réponse Json*/
  web_server << json_char;
  client.stop();

  return true;
}

boolean ShutterWebserver::setdata_handler(TinyWebServer& web_server){
  ShutterSerial::print("(ShutterWebserver) setdata_handler", STACK, true);
  web_server.send_error_code(200);
  web_server.send_content_type("application/json");
  web_server.end_headers();
  /*Récupération de l'argument du POST*/
  Client& client = web_server.get_client();
  char c;
  String requete = "";
  if (client.connected()) {
    while (client.available()) {
      c = (char)client.read();
      requete += c;
    }
  }
  client.stop();
  /*Equivalence de caractères HTML - ASCII
  %7B = {
  %22 = "
  + =
  %3A = :
  %5B = [
  %2C = ,
  %7D = }
  %5D = ]
  %2F = /
  */
  requete.replace("%7B", "{");
  requete.replace("%22", "\"");
  requete.replace("+", " ");
  requete.replace("%3A", ":");
  requete.replace("%5B", "[");
  requete.replace("%2C", ",");
  requete.replace("%7D", "}");
  requete.replace("%5D", "]");
  requete.replace("%2F", "/");
  ShutterSerial::print("Requête reçue : ", DEBUG, false);
  ShutterSerial::print(requete, DEBUG, true);

  if(requete.indexOf("date_fichier_config") >= 0) {
    receive_formulaire(requete);
  }
  else if(requete.indexOf("WebKitFormBoundary") >= 0) {
    file_upload(requete);
  }
  else if(requete.indexOf("servomoteur") >= 0) {
    servomoteur_dispatch(requete);
  }
  else if(requete.indexOf("shutter") >= 0) {
    shutter_dispatch(requete);
  }
}

/*Envoyer des informations au navigateur*/
JsonObject& ShutterWebserver::send_statut() {
  ShutterSerial::print("(ShutterWebserver) send_statut", STACK, true);
  /*Encodage du JSON*/
  JsonObject& statut_json = json_Buffer.createObject();
  statut_json["statut_SD"] = ShutterCommande::statutService[SERVICESD];
  statut_json["statut_OSC"] = ShutterCommande::statutService[SERVICEOSC];
  statut_json["statut_Bonjour"] = ShutterCommande::statutService[SERVICEBONJOUR];
  return statut_json;
}
JsonObject& ShutterWebserver::send_console() {
  ShutterSerial::print("(ShutterWebserver) send_console", STACK, true);
  JsonObject& console_json = json_Buffer.createObject();
  ShutterSerial::historique_print();
  int cpt = 0;
  for(cpt = 0; cpt <= ShutterSerial::_compteur - 1; cpt++) {
    sprintf(index_console[cpt], "%d", cpt);
    console_json[index_console[cpt]] = ShutterSerial::_historique[cpt];
  }
  /*Remise à zéro du compteur*/
  ShutterSerial::_compteur = 0;
  console_json.printTo(Serial);
  return console_json;
}
JsonObject& ShutterWebserver::send_dataForm() {
  ShutterSerial::print("(ShutterWebserver) send_dataForm", STACK, true);
  JsonObject& dataForm_json = ShutterJSON::decode(ShutterSD::read_charArray("config.txt", false), false);
  return dataForm_json;
}

/*Recevoir les informations du navigateur*/
void ShutterWebserver::servomoteur_dispatch(String requete) {
  ShutterSerial::print("(ShutterWebserver) servomoteur_dispatch", STACK, true);
  /*On fait -1 car dans l'interface le nombre de servo est référencé à 1 et pas 0*/
  int indexServo = (requete.charAt(requete.length() - 1)  - '0') - 1;
  if(requete.indexOf("open") >= 0) {
    ShutterServomoteur::open(indexServo);
  }
  if(requete.indexOf("close") >= 0) {
    ShutterServomoteur::close(indexServo);
  }
  if(requete.indexOf("position") >= 0) {
    ShutterSerial::print("Servomoteur ", DEBUG, false);
    ShutterSerial::print((indexServo + 1), DEBUG, false);
    ShutterSerial::print(" en position ", DEBUG, false);
    ShutterSerial::print(ShutterServomoteur::getPosition(indexServo), DEBUG, true);
  }
}

void ShutterWebserver::shutter_dispatch(String requete) {
  ShutterSerial::print("(ShutterWebserver) shutter_dispatch", STACK, true);
  if(requete.indexOf("test") >= 0) {
    ShutterServomoteur::test();
  }
  if(requete.indexOf("reboot") >= 0) {
    ShutterCommande::reboot();
  }
}
void ShutterWebserver::receive_formulaire(String requete) {
  ShutterSerial::print("(ShutterWebserver) receive_formulaire", STACK, true);
  ShutterSerial::print("Enregistrement de la config..", INFO, false);
  ShutterSD::write("config.txt", requete, true);
  ShutterSerial::print("Ok", INFO, true);
  ShutterCommande::reboot();
}
void ShutterWebserver::file_upload(String requete) {
  ShutterSerial::print("(ShutterWebserver) file_upload", STACK, true);
  /* Structure de la requête :

  ------WebKitFormBoundaryb134qV0xJ6E9NAJr
  Content-Disposition: form-data; name="fichier"; filename="NOMDUFICHIER.EXTENSION"
  Content-Type: type

  CONTENU DU FICHIER

  ------WebKitFormBoundaryb134qV0xJ6E9NAJr--

  */
  String _boundary = requete.substring(0, requete.indexOf('\n'));
  /*Suppression de ------WebKitFormBoundaryb134qV0xJ6E9NAJr
  _boundary.length() + 1 permet de supprimer le \n*/
  requete.remove(0, (_boundary.length() + 1));
  /*On a : filename="nom.extension"*/
  String _filename = requete.substring(requete.indexOf("filename=\""), requete.indexOf('\n'));
  /*Suppression de filename="*/
  _filename.remove(0, 10);
  /*Suppression du " final*/
  _filename.remove(_filename.indexOf('"'), 1);
  /*Suppression de Content-Disposition: form-data; name="fichier"; filename="NOMDUFICHIER.EXTENSION"*/
  requete.remove(0, (requete.indexOf('\n') + 1));
  /*Suppression de Content-Type: type*/
  requete.remove(0, (requete.indexOf('\n') + 1));
  /*Suppression de la ligne vide*/
  requete.remove(0, (requete.indexOf('\n') + 1));
  /*Suppression de ------WebKitFormBoundaryb134qV0xJ6E9NAJr--*/
  String _data = requete;
  _data.remove(_data.indexOf(_boundary.substring(0, 20)), (_data.lastIndexOf('\n') - _data.indexOf(_boundary.substring(0, 20))));

  /*Suppression des lignes vides à la fin fichier*/
  _data.remove(_data.lastIndexOf('\n'), 3);

  ShutterSerial::print("Boundary : ", DEBUG, false);
  ShutterSerial::print(_boundary, DEBUG, true);

  ShutterSerial::print("Filename : ", DEBUG, false);
  ShutterSerial::print(_filename, DEBUG, true);

  ShutterSerial::print("Data : ", DEBUG, false);
  ShutterSerial::print(_data, DEBUG, false);

  ShutterSerial::print("Enregistrement du fichier ", INFO, false);
  char* char_filename = (char*)malloc( (_filename.length() + 1) * sizeof(char)  );
  _filename.trim();
  _filename.toUpperCase();
  _filename.toCharArray(char_filename, (_filename.length() + 1));
  ShutterSerial::print(char_filename, INFO, false);
  ShutterSD::write(char_filename, _data, true);
  ShutterSerial::print(" .. Ok", INFO, false);

}

/*Utilitaires*/

void ShutterWebserver::send_file_name(TinyWebServer& web_server, const char* filename) {
  ShutterSerial::print("(ShutterWebserver) send_file_name", STACK, true);
  TinyWebServer::MimeType mime_type = TinyWebServer::get_mime_type_from_filename(filename);
  web_server.send_error_code(200);
  web_server.send_content_type(mime_type);
  web_server.end_headers();

  if (ShutterSD::_file.open(&ShutterSD::_root, filename, O_READ)) {
    ShutterSerial::print("Serveur web - lecture du fichier : ", INFO, false);
    ShutterSerial::print(filename, INFO, true);
    web_server.send_file(ShutterSD::_file);
    ShutterSD::_file.close();
  } else {
    web_server.send_error_code(404);
    web_server.send_content_type("text/plain");
    web_server.end_headers();

    ShutterSerial::print("Fichier introuvable : ", ERROR, false);
    ShutterSerial::print(filename, ERROR, true);
    ShutterLED::setCouleur(BLUE);

    web_server << F("404 - Fichier introuvable ") << filename << "\n";
  }
}
