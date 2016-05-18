
/*shutterSD.cpp
Dernière modification : 17/05/16
© Patrice Vieyra - contact@magicofthings.fr

Librairie ayant pour but de gérer la carte SD

Méthodes
  Publiques

  - Constructeur : Instanciation de l'objet. Pas utilisé dans notre cas, la classe
    est statique
  - void init() : Initialisation de la carte SD
  - char* read_charArray(char* filename, bool isCritical) : Lecture du fichier filename et retour sous la forme d'une chaîne de caractères
  le paramètre isCritical permet d'indiquer si l'opération est critique
  (en cas d'erreur on passe la LED de notification au rouge et on bloque l'éxecution du programme)
  - String read_string(char* filename, bool isCritical) : Lecture du fichier filename et retour sous la forme d'une String
  le paramètre isCritical permet d'indiquer si l'opération est critique
  (en cas d'erreur on passe la LED de notification au rouge et on bloque l'éxecution du programme)
  - void write(char* filename, char* data, bool isCritical) : Ecriture de data dans le fichier filename
  le paramètre isCritical permet d'indiquer si l'opération est critique
  (en cas d'erreur on passe la LED de notification au rouge et on bloque l'éxecution du programme)
  - void write(char* filename, String data, bool isCritical) : Ecriture de data dans le fichier filename
  Si le fichier existe il sera écrasé
  le paramètre isCritical permet d'indiquer si l'opération est critique
  (en cas d'erreur on passe la LED de notification au rouge et on bloque l'éxecution du programme)
  - void write(char* filename, char** data, bool isCritical) : Ecriture des datas dans le fichier filename
  Si le fichier existe il sera écrasé
  Si le fichier existe , data sera ajouté à la suite
  le paramètre isCritical permet d'indiquer si l'opération est critique
  (en cas d'erreur on passe la LED de notification au rouge et on bloque l'éxecution du programme)
  Privées
  - String _read(char* filename, bool isCritical) : Méthode appelée par read_string et read_charArray pour lire le fichier filename
  le paramètre isCritical permet d'indiquer si l'opération est critique
  (en cas d'erreur on passe la LED de notification au rouge et on bloque l'éxecution du programme)
*/

#include "shutterSD.h"

/*Variables permettant d'initialiser la carte SD*/
Sd2Card _card;
SdVolume _volume;

SdFile ShutterSD::_root;

/*Variable contenant le fichier*/
SdFile ShutterSD::_file;

ShutterSD::ShutterSD() {}

/* Fonction d'initialisation de la communication avec la carte SD
* Port utilisé pour la carte SD : SDCARD_CS 4
*/
bool ShutterSD::init() {
  ShutterSerial::print("(ShutterSD) init", STACK, true);
  /*On inhibe la partie ethernet pour pouvoir utiliser la carte SD*/
  pinMode(W5200_CS, OUTPUT);
  digitalWrite(W5200_CS, HIGH);

  ShutterSerial::print("Initialisation carte SD...", DEBUG, false);
  /*Permet de désactiver le module Ethernet pendant l'intialisation du module SD*/
  pinMode(SDCARD_CS, OUTPUT);
  pinMode(W5200_CS, OUTPUT);
  digitalWrite(SDCARD_CS, LOW);
  digitalWrite(W5200_CS, HIGH);

  if (!_card.init(SPI_FULL_SPEED, SDCARD_CS)) {
    ShutterSerial::print("Echec Initialisation. Est-ce que la carte est bien insérée ?", ERROR, true);
    ShutterLED::setCouleur(RED);
    return false;
  }
  if (!_volume.init(&_card)) {
    ShutterSerial::print("Partition FAT16/32 introuvable", ERROR, true);
    ShutterLED::setCouleur(RED);
    return false;
  }
  if (!_root.openRoot(&_volume)) {
    ShutterSerial::print("Echec Initialisation openRoot", ERROR, true);
    ShutterLED::setCouleur(RED);
    return false;
  }
  /*On inhibe la carte SD*/
  digitalWrite(SDCARD_CS, HIGH);

  ShutterSerial::print("Ok", DEBUG, true);
  return true;
}


String ShutterSD::read_string(char* filename, bool isCritical) {
  ShutterSerial::print("(ShutterSD) read_string", STACK, true);
  return _read(filename, isCritical);
}

char* ShutterSD::read_charArray(char* filename, bool isCritical) {
  ShutterSerial::print("(ShutterSD) read_charArray", STACK, true);
  String data = _read(filename, isCritical);

  char* retour = new char[data.length() + 1];
  data.toCharArray(retour, data.length() + 1);
  return retour;
}

String ShutterSD::_read(char* filename, bool isCritical) {
  ShutterSerial::print("(ShutterSD) _read", STACK, true);
  String data = "";

  /*Taille du contenu utile du buffer*/
  int size = 0;
  /*On essaie d'ouvrir le fichier en mode lecture*/
  if(_file.open(&_root, filename, O_READ)) {
    /*On lit le fichier par paquet de 512 octets*/
    char _buffer[BUFFERSIZE];
    while((size = _file.read(_buffer, sizeof(_buffer))) > 0) {
      char* fitted_buffer = new char[size];
      /*Permet de ne récupérer que la partie utile du buffer et pas l'intégralité*/
      sprintf(fitted_buffer, "%s", _buffer);
      fitted_buffer[size] = (char)0;
      ShutterSerial::print("Buffer ", DEBUG, false);
      ShutterSerial::print(size, DEBUG, false);
      ShutterSerial::print(" octets : ", DEBUG, true);
      ShutterSerial::print(fitted_buffer, DEBUG, true);
      data += fitted_buffer;
      free(fitted_buffer);
    }
    _file.close();
  }
  else {
    ShutterSerial::print("Erreur d'ouverture du fichier ", ERROR, false);
    ShutterSerial::print(filename, ERROR, true);
    if(isCritical) {
      ShutterLED::setCouleur(RED);
      while(1);
    }
    else {
      ShutterLED::setCouleur(BLUE);
    }
  }
  return data;
}


void ShutterSD::write(char* filename, char* data, bool isCritical) {
  ShutterSerial::print("(ShutterSD) write (char*)", STACK, true);

  if(_file.open(&_root, filename, O_WRITE | O_CREAT)) {
    _file.write(data, strlen(data));
    _file.close();
  }
  else {
    ShutterSerial::print("Erreur d'écriture du fichier ", ERROR, false);
    ShutterSerial::print(filename, ERROR, true);
    if(isCritical) {
      ShutterLED::setCouleur(RED);
      while(1);
    }
    else {
      ShutterLED::setCouleur(BLUE);
    }
  }
}

void ShutterSD::write(char* filename, String data, bool isCritical) {
  ShutterSerial::print("(ShutterSD) write (String)", STACK, true);
  char* data_char = new char[data.length() + 1];
  data.toCharArray(data_char, data.length() + 1);
  data_char[strlen(data_char)] = (char)0;
  write(filename, data_char, isCritical);
}

void ShutterSD::write(char* filename, char data[][ShutterSerial::_data_size], bool isCritical) {
  ShutterSerial::print("(ShutterSD) write (char**)", STACK, true);
  if(_file.open(&_root, filename, O_CREAT | O_APPEND | O_WRITE )) {
    int cpt = 0;
    for(cpt = 0; cpt < ShutterSerial::_compteur; cpt++) {
      _file.write(data[cpt], strlen(data[cpt]));
    }
    _file.close();
  }
  else {
    ShutterSerial::print("Erreur d'écriture du fichier ", ERROR, false);
    ShutterSerial::print(filename, ERROR, true);
    if(isCritical) {
      ShutterLED::setCouleur(RED);
      while(1);
    }
    else {
      ShutterLED::setCouleur(BLUE);
    }
  }
}
