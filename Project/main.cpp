#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ArduinoMqttClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include "arduino_secrets.h"

#define LDR_PIN A0
int LEDS_PIN[] = {D1, D2, D3, D4};

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

const char broker[] = "192.168.224.203";  // broker IP !!!!!!!!!!!!!!!!!!!!!!!!!!*******
int        port     = 1883;
const char ping_topic[]  = "I1820/main/agent/ping";
const char log_topic[]   = "I1820/main/log/send";
const char notif_topic[] = "I1820/main/configuration/request";

//set interval for sending messages (milliseconds)
const long interval = 5000;
unsigned long previousMillis = 0;

StaticJsonDocument<200> doc;

void onMqttMessage(int messageSize) {
  // we received a message, print out the topic and contents
  Serial.println("Received a message with topic '");
  Serial.print(mqttClient.messageTopic());
  Serial.print("', length ");
  Serial.print(messageSize);
  Serial.println(" bytes:");

  String json = "";

  // use the Stream interface to print the contents
  while (mqttClient.available())
    json += (char)mqttClient.read();
  
  Serial.println(json);
  deserializeJson(doc, json);

  if ((doc["agent"] == "nodemcu") && (doc["settings"][0]["name"] == "on")) {
    if (doc["device"] == "l1")
      digitalWrite(LEDS_PIN[0], doc["settings"][0]["value"]);
    else if(doc["device"] == "l2")
      digitalWrite(LEDS_PIN[1], doc["settings"][0]["value"]);
    else if(doc["device"] == "l3")
      digitalWrite(LEDS_PIN[2], doc["settings"][0]["value"]);
    else if(doc["device"] == "l4")
      digitalWrite(LEDS_PIN[3], doc["settings"][0]["value"]);
  }

  Serial.println();
}

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.print(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }
  Serial.println();

  Serial.println("You're connected to the network");
  Serial.println();

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();

  mqttClient.onMessage(onMqttMessage);

  mqttClient.subscribe(notif_topic);

  for(unsigned int i = 0; i < sizeof(LEDS_PIN)/sizeof(LEDS_PIN[0]); i++)
    pinMode(LEDS_PIN[i], OUTPUT);

  timeClient.begin();
  timeClient.setTimeOffset(0);
}

void loop() {
  // call poll() regularly to allow the library to send MQTT keep alive which
  // avoids being disconnected by the broker
  mqttClient.poll();

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    timeClient.update();
    // save the last time a message was sent
    previousMillis = currentMillis;

    // **************** ping
    String ping = "{\"id\": \"nodemcu\",\"things\": [{ \"id\": \"ldr\", \"type\": \"light\" }, { \"id\": \"l1\", \"type\": \"lamp\" }, { \"id\": \"l2\", \"type\": \"lamp\" }, { \"id\": \"l3\", \"type\": \"lamp\" }, { \"id\": \"l4\", \"type\": \"lamp\" }],\"actions\": []}";

    Serial.print("Sending message to topic: ");
    Serial.println(ping_topic);
    Serial.println(ping);

    mqttClient.beginMessage(ping_topic);
    mqttClient.print(ping);
    mqttClient.endMessage();


    // **************** ldr
    int ldr_value = analogRead(LDR_PIN);
    String ldr_message = "{\"timestamp\": " + String(timeClient.getEpochTime()) + ", \"type\": \"light\", \"device\": \"ldr\", \"states\": [{ \"name\": \"light\", \"value\": " + String(map(ldr_value, 0, 1024, 100, 0)) + " }], \"agent\": \"nodemcu\"}";

    Serial.print("Sending message to topic: ");
    Serial.println(log_topic);
    Serial.println(ldr_message);

    mqttClient.beginMessage(log_topic);
    mqttClient.print(ldr_message);
    mqttClient.endMessage();

    Serial.println();
  }
}

