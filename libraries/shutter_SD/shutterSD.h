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
		Si le fichier existe il sera écrasé
		le paramètre isCritical permet d'indiquer si l'opération est critique
		(en cas d'erreur on passe la LED de notification au rouge et on bloque l'éxecution du programme)
		- void write(char* filename, String data, bool isCritical) : Ecriture de data dans le fichier filename
		Si le fichier existe il sera écrasé
		le paramètre isCritical permet d'indiquer si l'opération est critique
		(en cas d'erreur on passe la LED de notification au rouge et on bloque l'éxecution du programme)
		- void write(char* filename, char** data, bool isCritical) : Ecriture des datas dans le fichier filename
		Si le fichier existe , data sera ajouté à la suite
		le paramètre isCritical permet d'indiquer si l'opération est critique
		(en cas d'erreur on passe la LED de notification au rouge et on bloque l'éxecution du programme)

		Privées
		- String _read(char* filename, bool isCritical) : Méthode appelée par read_string et read_charArray pour lire le fichier filename
		le paramètre isCritical permet d'indiquer si l'opération est critique
		(en cas d'erreur on passe la LED de notification au rouge et on bloque l'éxecution du programme)

Attributs
	Privés

	- char _buffer[BUFFERSIZE] : Buffer de lecture du fichier. Sa taille est définie par le define BUFFERSIZE
*/

#ifndef shutterSD
#define shutterSD

/*Librairie Arduino qui gère les cartes SD*/
#include <SD.h>
#include <shutterSerial.h>
#include <shutterLED.h>

/*PIN de l'arduino utilisé par la carte SD*/
#define SDCARD_CS 4
/*PIN utilisé par la communication ethernet*/
#define W5200_CS  10

/*Taille du buffer de lecture*/
#define BUFFERSIZE 512

class ShutterSD {

public:

	/*Constructeur*/
	ShutterSD();


	static bool init();


	static char* read_charArray(char* filename, bool isCritical);
	static String read_string(char* filename, bool isCritical);
	static void write(char* filename, char* data, bool isCritical);
	static void write(char* filename, String data, bool isCritical);
	static void write(char* filename, char data[][ShutterSerial::_data_size], bool isCritical);

	static SdFile _file;
	static SdFile _root;

private:

	static String _read(char* filename, bool isCritical);


};
#endif
