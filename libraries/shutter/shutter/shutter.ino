#include <SPI.h>
#include <Ethernet.h>
#include <OSCBundle.h>
#include <EthernetUdp.h>
#include <Servo.h>
#include <Flash.h>
#include <TinyWebServer.h>
#include <SD.h>
#include <ArduinoJson.h>
#include <EthernetBonjour.h>

#include <shutter.h>
#include <shutterCommande.h>
#include <shutterSerial.h>
#include <shutterLED.h>
#include <shutterConfig.h>
#include <shutterSD.h>
#include <shutterJSON.h>
#include <shutterServomoteur.h>
#include <shutterBonjour.h>
#include <shutterWebserver.h>
#include <shutterOSC.h>

Shutter _shutter = Shutter();
void setup() {
  _shutter.init();
}

void loop() {
  _shutter.run();
}
