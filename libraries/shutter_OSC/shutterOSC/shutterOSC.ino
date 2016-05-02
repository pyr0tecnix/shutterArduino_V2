#include <SPI.h>
#include <Ethernet.h>
#include <OSCBundle.h>
#include <EthernetUdp.h>
#include <Servo.h>

#include <shutterSerial.h>
#include <shutterOSC.h>
#include <shutterLED.h>
#include <shutterServomoteur.h>
#include <shutterCommande.h>

ShutterOSC s_OSC = ShutterOSC();
ShutterSerial s = ShutterSerial();
ShutterLED s_LED = ShutterLED();
ShutterServomoteur s_Servo = ShutterServomoteur();
IPAddress s_IP(192, 168, 0, 9);
IPAddress m_IP(192, 168, 0, 50);
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

void setup() {

  s.begin(115200);
  s.setVerboseLevel(DEBUG);
  s_LED.init();
  s_Servo.init(1);
  s_OSC.init(m_IP, 9000, s_IP, 800, mac);
}


void loop() {
   s_OSC.receive();
}
