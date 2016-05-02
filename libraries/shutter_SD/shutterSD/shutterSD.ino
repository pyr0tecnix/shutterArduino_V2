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
  s_SD.write("hello.txt", "Hello world, i'm an Arduino", false);
  s.print("Lecture du fichier hello.txt", DEBUG, true);
  s.print(s_SD.read_charArray("hello.txt", false), DEBUG, true);
  s.print(s_SD.read_charArray("hello.txt", false), DEBUG, true);

}


void loop() {
}
