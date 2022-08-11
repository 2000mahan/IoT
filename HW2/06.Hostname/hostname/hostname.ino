/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp8266-nodemcu-set-custom-hostname-arduino/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <ESP8266WiFi.h>

// Replace with your network credentials (STATION)
const char* ssid = "ssid";
const char* password = "Pass";

String newHostname = "Mahan_Morteza";

void setup() {
  Serial.begin(9600);
  while(!Serial){}
  WiFi.mode(WIFI_STA);

  //Get Current Hostname
  Serial.printf("Default hostname: %s\n", WiFi.hostname().c_str());

//Set new hostname
  WiFi.hostname(newHostname.c_str());

  //Get Current Hostname
  Serial.printf("New hostname: %s\n", WiFi.hostname().c_str());
  
}
void loop(){
  }
