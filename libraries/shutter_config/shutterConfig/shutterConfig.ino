#include <SPI.h>
#include <Ethernet.h>

#include <shutterSerial.h>
#include <shutterConfig.h>
#include <shutterCommande.h>

ShutterSerial s = ShutterSerial();
ShutterConfig s_Config = ShutterConfig();
void setup() {

  s.begin(115200);
  s.setVerboseLevel(DEBUG);

  IPAddress s_IP(192, 168, 0, 9);
  IPAddress m_IP(192, 168, 0, 50);
  char mode[3] = {'O', 'S', 'C'};
  char mac[20];
  sprintf(mac, "AA:BB:CC:DD:EE:FF");
  s_Config.setShutterOscIp(s_IP);
  s_Config.setMasterOscIp(m_IP);
  s_Config.setShutterOscPort(800);
  s_Config.setMasterOscPort(9000);
  s_Config.setShutterOscMac(mac);
  s_Config.setShutterBonjourName("MoT - Shutter 01");
  s_Config.setShutterControlMode(mode);
  s_Config.setShutterServoNombre(2);
  s_Config.setShutterVerbositeLevel(3);
  s_Config.setShutterHTTPServerActivate(true);

  s_Config.print();

}


void loop() {
}
