#include <ESP8266WiFi.h>

IPAddress subnet(255, 255, 255, 0);                  
IPAddress gateway(192, 168, 4, 9);                  
IPAddress local_IP(192, 168, 4, 22);

void setup() {
  Serial.begin(9600);

  while (!Serial)
    ;

  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP("Mahan_Morteza", "123456");

  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  WiFi.softAPConfig(local_IP, gateway, subnet);

  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  // put your main code here, to run repeatedly:

}
