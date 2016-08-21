#include "IOCClient.h"
#include "Ethernet.h"
#include "SPI.h"

EthernetClient eClient;
IOCClient client;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char hostname[] = "sandigma.net";
int port = 3636;
char protocol[] = "device"; 

void onSwitchChanged(int switch_id, bool status){
  Serial.print(switch_id);
  Serial.println(status);
}

void onScheduleSwitchChanged(int switch_id, long up_time, long down_time){
  
}


unsigned long lastbroadcast;

void broadcastData(){
  unsigned long now = millis();
  if ((now - lastbroadcast) >= 3000) {
    lastbroadcast = now;
    if (client.connected()){
      float data = static_cast <float> (rand()) / static_cast <float> (300);
      int sensor_id = 1;
      long time = 1471764543 + now;
      client.sendData(sensor_id,time,data); 
    }
  } 
}

void setup() {
  Serial.println(F("Setup."));
  Serial.begin(9600);

  Ethernet.begin(mac);

  client.setSwitchEvent(onSwitchChanged);
  client.setScheduleSwitchEvent(onScheduleSwitchChanged);
  
  if (!client.connect(hostname, port,protocol)) Serial.println(F("Not connected."));
}

void loop() {
  client.monitor();
  broadcastData();
}
