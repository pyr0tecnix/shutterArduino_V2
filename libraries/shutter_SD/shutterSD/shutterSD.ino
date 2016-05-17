#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

#include <shutterSerial.h>
#include <shutterSD.h>
#include <shutterLED.h>

ShutterSerial s = ShutterSerial();
ShutterSD s_SD = ShutterSD();
void setup() {

  s.begin(115200);
  s.setVerboseLevel(DEBUG);
  s_SD.init();

  String test("Hello world, i'm a string.");
  char *tab[128] = {"Première phrase\n", "Deuxième phase !\n", "Le meilleur pour la fin."};

  s_SD.write("char.txt", "Hello world, i'm a char array.", false);
  s_SD.write("string.txt", test, false);
  s_SD.write("tab.txt", tab, false);

  s.print(s_SD.read_charArray("char.txt", false), INFO, true);
  s.print(s_SD.read_string("string.txt", false), INFO, true);
  s.print(s_SD.read_charArray("tab.txt", false), INFO, true);

}


void loop() {
}
