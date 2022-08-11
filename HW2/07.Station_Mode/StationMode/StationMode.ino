#include "ESP8266WiFi.h"

const char *ssid =  "ssid";     // replace with your wifi ssid and wpa2 key
const char *pass =  "pass";

void setup() {

  Serial.begin(9600);

  while(!Serial)
    ;

  int numberOfNetworks = WiFi.scanNetworks();

  for (int i = 0; i < numberOfNetworks; i++) {

    Serial.print("Network name: ");
    Serial.println(WiFi.SSID(i));
    Serial.print("Signal strength: ");
    Serial.println(WiFi.RSSI(i));
    Serial.println("-----------------------");

  }

  delay(10);

  Serial.print("Connecting to ");
  Serial.print(ssid);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());   //You can get IP address
}

void loop() {}
