#include <SPI.h>
#include <Ethernet.h>
#include <OSCBundle.h>
#include <EthernetUdp.h>
#include <Servo.h>
#include <Flash.h>
#include <TinyWebServer.h>
#include <SD.h>
#include <ArduinoJson.h>

#include <shutterSerial.h>
#include <shutterLED.h>
#include <shutterServomoteur.h>
#include <shutterOSC.h>
#include <shutterCommande.h>
#include <shutterWebserver.h>
#include <shutterSD.h>
#include <shutterConfig.h>
#include <shutterJSON.h>

ShutterSerial s = ShutterSerial();
ShutterLED s_LED = ShutterLED();
ShutterOSC s_OSC = ShutterOSC();
ShutterWebserver s_WEB = ShutterWebserver();
ShutterServomoteur s_Servo = ShutterServomoteur();
ShutterSD s_SD = ShutterSD();

IPAddress s_IP(192, 168, 0, 90);
IPAddress m_IP(192, 168, 0, 50);
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

void setup() {

  s.begin(115200);
  s.setVerboseLevel(DEBUG);
  s_LED.init();
  s_Servo.init(1);
  s_SD.init();
  s_WEB.init();
  s_OSC.init(m_IP, 9000, s_IP, 800, mac);
}

void loop() {
  s_WEB.run();
}
