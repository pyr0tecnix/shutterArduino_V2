# README Shutter OSC
Version 1 - 02/05/2016


Shutter pour vidéoprojecteur, commandé via OSC (IP statique) et permettant de contrôler jusqu'à 4 servomoteurs.
Le service OSC est annoncé sur le réseau via le protocole Bonjour/Zeroconf.
Le shutter dispose d'une interface web permettant d'afficher l'état des différents services (SD, OSC, Bonjour), les messages séries, modifier la configuration via un formulaire et uploader des fichiers textes sur la carte SD.

A noter qu'il est également possible d'uploader des fichiers sur la carte SD via un script bash ou MSDOS.
## Installation des bibliothèques

Les bibliothèques contenues dans le dossier libraries sont à installer dans le dossier contenant les bibliothèques Arduino en suivant la procédure officielle : [Installer une Librairie](https://www.arduino.cc/en/Guide/Libraries)

## Liste des dépendances

Certaines librairies du projet ont besoin de bibliothèques externes pour fonctionner :
* shutterBonjour : [Bonjour/Zeroconf](http://gkaindl.com/software/arduino-ethernet/bonjour) Cette bibliothèque nécessite la modification de la bibliothèque arduino [EthernetUdp](http://forum.arduino.cc/index.php?topic=234340.0)
* shutterJSON : [ArduinoJson](https://github.com/bblanchon/ArduinoJson/)
* shutterOSC : [Oscuino](https://github.com/CNMAT/OSC)
* shutterWebserver : [TinyWebServer](https://github.com/ovidiucp/TinyWebServer)

## Configuration hardware testée

* Micro-contrôleur :
	* [Arduino M0 Pro](http://www.arduino.org/products/boards/4-arduino-boards/arduino-m0-pro)
	* [Arduino Zero](http://www.arduino.cc/en/Main/ArduinoBoardZero)

* Shield ethernet :
	* [Neuftech® Ethernet shield module W5100 avec module Micro SD](http://www.amazon.fr/dp/B00PQA6G8O/ref=sr_ph?ie=UTF8&qid=1456398408&sr=1&keywords=neuftech)
* Servomoteur :
	* [Microservo tower pro SG90](http://boutique.semageek.com/fr/104-micro-servo-tower-pro-sg90.html)

* LED :
	* [LED RVB à anode commune](http://boutique.semageek.com/fr/62-led-rgb-transparente-5mm.html)

## Liste de course recommandée

* Arduino : Arduino M0 (ou zero) Pro
	* Recommandé (le petit frère du Pro qui n'embarque pas de puce de débug) [semageek](http://boutique.semageek.com/fr/574-arduino-m0-8058333491806.html)
	* Sur [selectronic](http://www.selectronic.fr/carte-arduino-zero-pro.html)
	* Sur [lextronic](http://www.lextronic.fr/P35395-arduino-m0-pro.html)

* Shield Ethernet :
	* Sur [Amazon](https://www.amazon.fr/Neuftech%C2%AE-Ethernet-Shield-Module-Arduino/dp/B00PQA6G8O)

* Servomoteur : J'utilise des SG90, à vous de voir en fonction de vos besoins en terme de couple, de vitesse et d'angle de rotation. A noter que les servomoteurs sont alimentés par le 5V de l'arduino, si vous voulez utiliser des servomoteurs plus puissant il faudra passer sur une alimentation extérieure. De plus le boîtier servomoteur est conçu pour la forme des SG90
	* Sur [semageek](http://boutique.semageek.com/fr/104-micro-servo-tower-pro-sg90.html)

* Led RGB : (A anode commune)
	* Sur [Semageek](http://boutique.semageek.com/fr/62-led-rgb-transparente-5mm.html)

* Embase ethernet : pour les boîtier (4 pour le boîtier principal et 1 par boîtier servomoteur) :
	* Sur [gotronic](http://www.gotronic.fr/art-embase-ci-srt8-8b-4924.htm)

* Des fils pour les connexions (privilégier le multi-brins) :
	* Sur [gotronix](http://www.gotronic.fr/art-assortiment-fcs60-5211.htm)
	* Sur [selectronic](http://www.selectronic.fr/assortiment-de-fil-de-cablage-0-2mm-mono-brin.html)

* Bloc d'alimentation :
	* Sur [semageek](http://boutique.semageek.com/fr/370-alimentation-arduino-9v-660ma-jack-21mm.html)
	* Sur [amazon](https://www.amazon.fr/Aukru-Universel-Alimentation-adaptateur-chargeur/dp/B00MVLJ6NI)

## Câblage ##
* La LED de notification utilisée est une LED RVB à anode commune. L'anode est à connectée sur le 3.3V de l'arduino et les cathodes R, V et B sont à brancher respectivement sur les pins 11, 12 et 13 de l'arduino.
* Les servomoteurs sont à brancher sur les pins 2, 3, 6 et 7 et sur le 5V et le GND de l'arduino.

## Mise en route

* Connecter le câble ethernet
* Vérifier que la carte SD est bien insérée et contient bien le fichier de configuration **"config.txt"**
* Brancher l’alimentation du shutter
* A la fin de l'initialisation la LED de notification est verte et clignote une fois
* Il est possible de brancher un câble USB pour avoir les informations de débug par liaison série (115200 baud)

## Code couleur de la LED de notification

La LED de notification est soit :
* rouge : Il y a eu une erreur critique qui empêche l'exécution normale du programme. Typiquement une erreur au niveau de la carte SD ou du réseau. Le système se bloque, vous trouverez plus d'informations en branchant un câble USB pour lire le retour série ou consulter le fichier log.txt sur la carte SD.
* bleu : Il y a eu une erreur mais non critique. Le programme continue son exécution. Typiquement une erreur dans le protocole Bonjour ou une erreur de lecture / écriture de fichiers non critique. Vous trouverez plus d'information en branchant un câble USB ou encore sur la partie console du webserver ou dans le fichier log.txt sur la carte SD.
* vert : Tout va bien, le système fonctionne correctement.

## Lire les retours série de l'arduino

### Linux et Mac Os

Utiliser la commande `screen` qui est installée par défaut sur les versions récentes de Mac OS et certaines distributions de Linux.

Dans le cas contraire, installer `screen` à l'aide du gestionnaire de paquet propre à votre distribution :
* [ArchLinux](https://wiki.archlinux.org/index.php/GNU_Screen)
* [Debian](https://wiki.debian.org/fr/Screen)
* [Ubuntu](https://doc.ubuntu-fr.org/screen)

Ouvrir le terminal et lancer `screen /dev/cu.usbmodem1411 9600`

Pour quitter `screen` appuyer sur **CTRL-A**

### Windows ###

Utiliser [PuTTY](http://www.putty.org/)

## Structure du fichier de Configuration
Le fichier de configuration utilise le formalisme JSON.

Liste des paramètres :

* "date_fichier_config" : paramètre indicatif, n'est pas utilisé par le shutter
* "shutter_verbosite_level" : indique le niveau de verbosité des messages séries. Les diférents niveaux disponibles sont :
	* 0 (ERROR) uniquement les messages d'erreurs
	* 1 (INFO) les messages d'erreurs et les informations principales
	* 2 (STACK) les messages d'erreurs, d'informations et les noms des fonctions appelées
	* 3 (DEBUG) l'intégralité des messages
* "shutter_control_Mode" : le mode de pilotage du shutter, pour le moment seul le mode osc est disponible
* "shutter_osc_Ip" : l'adresse IPV4 du shutter
* "shutter_osc_Port" : le port utilisé par le shutter pour la communication OSC
* "master_osc_Ip" : l'adresse IPV4 du master. Quelque soit l'interlocuteur le shutter réponds uniquement au master
* "master_osc_Port" : le port utilisé par le master pour la communication OSC,
* "shutter_osc_Mac" : l'adresse MAC utilisée par le shield éthernet
* "shutter_bonjour_name" : identifiant bonjour **de moins de 50 caractères**
* "shutter_servo_nombre" : nombre de servomoteur à activer, compris entre 1 et 4
* "shutter_http_server_activate" : activation du serveur web

Exemple de fichier de configuration

`{
	"date_fichier_config" : "26/02/16",
	"shutter_verbosite_level" : 1,
	"shutter_control_Mode" : "OSC" ,
	"shutter_osc_Ip" : "192.168.0.90",
	"shutter_osc_Port" : 8000,
	"master_osc_Ip" : "192.168.0.50",
	"master_osc_Port" : 9000,
	"shutter_osc_Mac" : "0xDE:0xAD:0xBE:0xEF:0xFE:0xED",
	"shutter_bonjour_name" : "AMshutter 01",
	"shutter_servo_nombre" : 1,
	"shutter_http_server_activate" : 1
}
`
## Glossaire des trames ##

* i/shutter_on
	* Mets le servomoteur i en position de travail (cache devant la lentille)
* i/shutter_off
	* Mets le servomoteur i en position de repos (cache relevé)

* i/shutter_position
	* Envoie une demande de position au servomoteur i qui renvoie sa position sur la liaison série et renvoie i/label_position position et i/shutter_move position (utilisé pour mettre à jour le fader et le label dans touchOSC)

* i/shutter_move (float)
	* Envoie une demande de déplacement au servomoteur i : le float est compris entre 0(min) et 100(max)

* /shutter_reboot
	* Reboot le shutter

* /shutter_runservotest
	* Chaque servomoteur actif est mis en position de travail puis en position de repos

* /shutter_ping
	* Le shutter renvoie /shutter_pong


## Liens intéressants

* http://trippylighting.com/teensy-arduino-ect/touchosc-and-arduino-oscuino/
* https://www.nickv.codes/blog/arduino-osc-iphone-and-dmx/
* https://skyduino.wordpress.com/2012/06/19/arduino-charger-un-fichier-de-configuration-depuis-une-carte-sd/
* http://www.dns-sd.org/ServiceTypes.html
* http://tutorialzine.com/2014/06/10-tips-for-writing-javascript-without-jquery/
* http://blog.garstasio.com/you-dont-need-jquery/ajax/
* http://starter-kit.nettigo.eu/2015/debug-sketch-on-arduino-zero-pro-with-gdb-and-openocd/
* http://jsonviewer.stack.hu/

## Contact

pyrotecnix - contact@magicofthings.fr
