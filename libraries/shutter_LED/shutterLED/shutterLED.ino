#include <SPI.h>
#include <shutterSerial.h>
#include <shutterLED.h>

ShutterSerial s = ShutterSerial();
ShutterLED s_LED = ShutterLED();

void setup() {

  s.begin(115200);
  s.setVerboseLevel(DEBUG);
  s_LED.init();

  s_LED.setCouleur(RED);
  delay(1000);
  s_LED.setCouleur(GREEN);
  delay(1000);
  s_LED.setCouleur(BLUE);
  delay(1000);
}


void loop() {
}
