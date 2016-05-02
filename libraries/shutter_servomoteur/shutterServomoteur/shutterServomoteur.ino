#include <SPI.h>
#include <Ethernet.h>
#include <Servo.h>

#include <shutterSerial.h>
#include <shutterServomoteur.h>

ShutterSerial s = ShutterSerial();
ShutterServomoteur s_Servo = ShutterServomoteur();

void setup() {

  s.begin(115200);
  s.setVerboseLevel(DEBUG);
  s_Servo.init(2);
  delay(1000);
  s_Servo.open(0);
  s.print("Servo 0 en position : ", DEBUG, false);
  s.print(s_Servo.getPosition(0), DEBUG, true);
  delay(1000);
  s_Servo.test();
}


void loop() {
}
