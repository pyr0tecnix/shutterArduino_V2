/*shutterSerial.cpp
Dernière modification : 18/05/16
© Patrice Vieyra - contact@magicofthings.fr

Librairie ayant pour but de gérer l'affichage sur le port série.
Le degré de verbosité des messages est règlable (ERROR, INFO, STACK, DEBUG).

Méthodes
  Publiques

    - Constructeur : Instanciation de l'objet. Pas utilisé dans notre cas, la classe
      est statique
    - void begin(unsigned int baudRate) : Initialise la liaison série au baudRate (nombre de bits/seconde) indiqué :
        Valeurs possible de baudrate : 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 38400, 57600, ou 115200
    - void setVerboseLevel() : Permet de définir le niveau de verbosité :
        ERROR -> Message d'erreur
        INFO -> Information sur l'état courant du système. Niveau par défaut
        STACK -> Permet de suivre la stack en afifchant l'intégralité des appels de fonctions
        DEBUG -> Affiche tout les messages
    - unsigned int getVerboseLevel() : Permet de récupérer le niveau de verbosité
    - void print(Var msg, int level, bool crlf) : Méthode d'affichage à l'écran.
        Trois surcharges existent pour des messages de type String, char* et int.
        Le paramètre level représente le type de message (ERROR, INFO, STACK ou DEBUG)
        le booléen crlf lorsqu'il est à true ajoute un saut de ligne après le message
    - void historique_reset() : Remet à 0 le compteur de message
    - void historique_print() : Envoie le contenu de l'historique sur le port série
    - void historique_put (char* nouveau) : Ajoute le message nouveau à l'historique
    - char* intToCharArray(int value, bool crlf) : Convertit un int en chaîne de caractère
        le booléen crlf lorsqu'il est à true ajoute un saut de ligne après le message
*/

#include "shutterSerial.h"

ShutterSerial::ShutterSerial() {}

/*Initialisation du niveau de verbosité à INFO*/
Level ShutterSerial::_verbosite = INFO;

char ShutterSerial::_historique[_history_size][256];
unsigned int ShutterSerial::_compteur = 0;
char temp[256];
char convertedValue[10];


void ShutterSerial::begin(unsigned int baudRate) {
  Serial.begin(baudRate);
  /*Ces deux lignes permettent d'éviter de perdre le premier caractère affiché
  L'arduino à tendance à ne pas afficher le premier caractère.
  */
  while(!Serial);
  Serial.println("");
}

unsigned int ShutterSerial::getVerboseLevel() {
  return (unsigned int)_verbosite;
}

void ShutterSerial::setVerboseLevel(unsigned int level) {
  _verbosite = (Level) level;
}

void ShutterSerial::print(String msg, int level, bool crlf) {
  if(level <= _verbosite) {
    if (crlf) {
      Serial.println(msg);
      if(level <= INFO) {
        msg += '\n';
        msg.toCharArray(temp, msg.length());
        historique_put(temp);
      }
    }
    else {
      Serial.print(msg);
      if(level <= INFO) {
        msg.toCharArray(temp, msg.length());
        historique_put(temp);
      }
    }
  }
}
void ShutterSerial::print(int msg, int level, bool crlf) {
  if(level <= _verbosite) {
    if (crlf) {
      Serial.println(msg);
      if(level <= INFO) {
        historique_put(ShutterSerial::intToCharArray(msg, true));
      }
    }
    else {
      Serial.print(msg);
      if(level <= INFO) {
        historique_put(ShutterSerial::intToCharArray(msg, false));
      }
    }
  }
}
void ShutterSerial::print(char* msg, int level, bool crlf) {
  if(level <= _verbosite) {
    if (crlf) {
      Serial.println(msg);
      if(level <= INFO) {
        sprintf(temp, "%s\n", msg);
        historique_put(temp);
      }
    }
    else {
      Serial.print(msg);
      if(level <= INFO) {
        historique_put(msg);
      }
    }
  }
}


/*Manipulation de l'historique*/
void ShutterSerial::historique_reset() {
  _compteur = 0;
}

void ShutterSerial::historique_put(char* nouveau) {
  sprintf(_historique[_compteur], "%s", nouveau);
  _compteur = (_compteur + 1)%_history_size;
  Serial.print("Compteur : ");
  Serial.println(_compteur);
}

void ShutterSerial::historique_print() {
  int cpt = 0;
  Serial.println("Affichage de l'historique : ");

  if(_compteur > 0) {
    for(cpt = 0; cpt < _compteur; cpt++) {
      Serial.print(_historique[cpt]);
    }
  }
  else {
    Serial.println("L'historique est vide.");
  }
}

char* ShutterSerial::intToCharArray(int value, bool crlf) {
  String _temp(value);
  if(crlf) {
    _temp += "\n";
  }
  _temp.toCharArray(convertedValue, _temp.length() + 1);
  return convertedValue;
}
