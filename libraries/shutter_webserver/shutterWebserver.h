/*
  shutterWebserver.h
  Dernière modification : 01/05/16
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

  Attributs
  Privé
    - TinyWebServer::PathHandler _handlers[] : Contient les URLs accessibles
    - TinyWebServer _web : Instance du serveur web
 */

#ifndef ShutterWebserver_h
#define ShutterWebserver_h

#include <SPI.h>
#include <Ethernet.h>
/*Bibliothèque utilisée par TinyWebServer*/
#include <Flash.h>
#include <SD.h>
#include <TinyWebServer.h>
#include <ArduinoJson.h>

#include <shutterSerial.h>
#include <shutterLED.h>
#include <shutterSD.h>
#include <shutterConfig.h>
#include <shutterJSON.h>
#include <shutterServomoteur.h>
#include <shutterCommande.h>

class ShutterWebserver {

  public:

    /*Constructeur*/
    ShutterWebserver();

    static void init();
    static void run();

  private:
    static TinyWebServer::PathHandler _handlers[];
    static TinyWebServer _web;

    /*Page handler*/
    static boolean index_handler(TinyWebServer& web_server);
    static boolean file_handler(TinyWebServer& web_server);
    static boolean getdata_handler(TinyWebServer& web_server);
    static boolean setdata_handler(TinyWebServer& web_server);

    static void file_uploader_handler(TinyWebServer& web_server, TinyWebPutHandler::PutAction action, char* buffer, int size);

    /*Envoyer des informations au navigateur*/
    static JsonObject& send_statut();
    static JsonObject& send_console();
    static JsonObject& send_dataForm();

    /*Recevoir les informations du navigateur*/
    static void servomoteur_dispatch(String requete);
    static void shutter_dispatch(String requete);
    static void receive_formulaire(String requete);
    static void file_upload(String requete);


    /*Utilitaires*/
    static void send_file_name(TinyWebServer& web_server, const char* filename);

};
#endif
