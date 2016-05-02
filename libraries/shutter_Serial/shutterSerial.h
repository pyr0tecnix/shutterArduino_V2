/*shutterSerial.h
Dernière modification : 28/04/16
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
    - unsigned int getVerboseLevel() : Permet de récupérer le niveau de verbosité :

    - void print(Var msg, int level, bool crlf) : Méthode d'affichage à l'écran.
        Trois surcharges existent pour des messages de type String, char* et int.
        Le paramètre level représente le type de message (ERROR, INFO, STACK ou DEBUG)
        le booléen crlf lorsqu'il est à true ajoute un saut de ligne après le message

Attributs
  Privés

    - Level _level : Variable contenant le niveau de verbosité actuel. Peut être
        modifié grâce à setVerboseLevel et récupéré grâce à getVerboseLevel.
*/

#ifndef shutterSerial
#define shutterSerial

#include <SPI.h>

#define DATASIZE 256

enum Level {
  ERROR,
  INFO,
  STACK,
  DEBUG,
  };

class ShutterSerial {

  public:

    /*Constructeur*/
    ShutterSerial();
    static void begin(unsigned int baudRate);

    static void setVerboseLevel(unsigned int level);
    static unsigned int getVerboseLevel();

    static void print(String msg, int level, bool crlf);
    static void print(int msg, int level, bool crlf);
    static void print(char* msg, int level, bool crlf);

    static void historique_reset();
    /*Historique des messages séries. On ne conserve que les messages d'INFO et d'ERROR*/
    static char _historique[][DATASIZE];
    static void historique_put(char* nouveau);
    static void historique_print();
    static unsigned int _compteur;

    static char* intToCharArray(int value, bool crlf);

    static const unsigned int _history_size = 60;
  private:
    static Level _verbosite;

};
#endif
