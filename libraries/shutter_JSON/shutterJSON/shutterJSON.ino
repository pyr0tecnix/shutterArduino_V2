#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoJson.h>
#include <shutterSerial.h>
#include <shutterJSON.h>
#include <shutterConfig.h>
#include <shutterLED.h>
#include <shutterCommande.h>

ShutterSerial s = ShutterSerial();
ShutterJSON s_JSON = ShutterJSON();
ShutterConfig s_Config = ShutterConfig();
ShutterLED s_LED = ShutterLED();

void setup() {
  char json[] = "{\"date_fichier_config\":\"26/02/16\",\"shutter_verbosite_level\":3,\"shutter_control_Mode\":\"OSC\",\"shutter_osc_Ip\":\"192.168.0.90\",\"shutter_osc_Port\":8000,\"master_osc_Ip\":\"192.168.0.50\",\"master_osc_Port\":9000,\"shutter_osc_Mac\":\"0xDE:0xAD:0xBE:0xEF:0xFE:0xED\",\"shutter_bonjour_name\":\"AMshutter 01\",\"shutter_servo_nombre\":1,\"shutter_http_server_activate\":1}";

  s.begin(115200);
  s.setVerboseLevel(DEBUG);
  s_LED.init();

  JsonObject& root = s_JSON.decode(json, false);
  s_JSON.setConfig(root);
  s_Config.print();

}


void loop() {
}
