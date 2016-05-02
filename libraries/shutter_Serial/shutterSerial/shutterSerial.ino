#include <SPI.h>
#include <Ethernet.h>

#include <shutterSerial.h>

ShutterSerial s = ShutterSerial();

void setup() {

  s.begin(115200);
  s.setVerboseLevel(DEBUG);
  String test("Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
  char* test1 = "Test, test son";
  s.print("Hello world !", INFO, true);
  s.print(test, INFO, true);
  s.print(42, INFO, true);
  s.print(test, INFO, true);
  s.print(42, INFO, true);
  s.print(test, INFO, true);
  s.print(test1, INFO, true);


  s.historique_print();

}


void loop() {
}
