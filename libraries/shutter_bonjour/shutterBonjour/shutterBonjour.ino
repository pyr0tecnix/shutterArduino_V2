#include <SPI.h>
#include <Ethernet.h>
#include <EthernetBonjour.h>
#include <OSCBundle.h>
#include <EthernetUdp.h>
#include <Servo.h>

#include <shutterSerial.h>
#include <shutterLED.h>
#include <shutterBonjour.h>
#include <shutterServomoteur.h>
#include <shutterOSC.h>
#include <shutterCommande.h>


ShutterSerial s = ShutterSerial();
ShutterLED s_LED = ShutterLED();
ShutterOSC s_OSC = ShutterOSC();
ShutterBonjour s_Bonjour = ShutterBonjour();
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
  s_Servo.init(2);
  s_OSC.init(m_IP, 9000, s_IP, 800, mac);
  s_Bonjour.init("MoT-Shutter-01");
  s_Bonjour.addService("shutter01._osc", 8000, true);
  s_Bonjour.addService("shutter._http", 80, false);

}


void loop() {
  s_OSC.receive();
  s_Bonjour.run();

}
