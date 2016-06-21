#include "WebSocketClient.h"
#include "Ethernet.h"
#include "SPI.h"
EthernetClient eClient;
WebSocketClient client;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char hostname[] = "sandigma.net";
int port = 3636;
char protocol[] = "device"; 

void ondata(WebSocketClient client, char *data) {
  Serial.print("received data : ");
  Serial.println(data);
}

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac);
  client.setDataArrivedDelegate(ondata);
  if (!client.connect(hostname, port,protocol)) Serial.println(F("Not connected."));
}

void loop() {
  client.monitor();
}
