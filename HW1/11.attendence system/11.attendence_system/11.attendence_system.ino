#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define RST_PIN    D0
#define SS_PIN     D8

#define BUZZER_PIN D1
#define LED_PIN    D2
#define SERVO_PIN  D3
#define LDR_PIN    A0

#define THRESHOLD  550
#define STUDS_NUM  2

#define LATE_SEC   15

#define SSID       "SSID"
#define PASSWORD   "WIFI-PASSWORD"

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
String tag;

Servo servo;
#define CLOSE_DEG 10
#define OPEN_DEG  170

bool buttonState, preButtonState = false;

typedef struct student {
  char tag_id[16];
  char name[16];
  bool present = false;
} Student;

Student students[STUDS_NUM] = {
  {{.tag_id = "24618110144"}, {.name = "Morteza Mirzai"}},
  {{.tag_id = "3717458213"}, {.name = "Mahan Ahmadvand"}}
};

unsigned int class_state = 0;

void onForSeconds(uint8_t pin, int second) {
  digitalWrite(pin, HIGH);
  delay(second*1000);
  digitalWrite(pin, LOW);
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  servo.attach(SERVO_PIN);
  servo.write(CLOSE_DEG);

  SPI.begin();
  rfid.PCD_Init();

  // Connect to Wi-Fi
  Serial.print(String("Connecting to ") + SSID);
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected.");

  // Initialize a NTPClient to get time
  timeClient.begin();
  timeClient.setTimeOffset(12600); // Tehran: +3:30 × 3600  = 12600
}

void loop() {
  // start and end the class
  buttonState = analogRead(LDR_PIN) > THRESHOLD;
  if (buttonState && !preButtonState && timeClient.update()) {
    class_state = (class_state)? 0 : timeClient.getEpochTime();
    if (!class_state){
      Serial.println("class ended at " + timeClient.getFormattedTime());
      Serial.println("-----------Report-----------");
      for (Student* s = &students[0]; s < &students[STUDS_NUM]; s++){
        Serial.printf("%s was %s\n", s->name, (s->present)?"present":"absent");
        s->present = false;
      }
      Serial.println("----------------------------");
    }
    else
      Serial.println("class started at " + timeClient.getFormattedTime());
  }
  preButtonState = buttonState;


  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial() && timeClient.update()) {
    if (class_state && (timeClient.getEpochTime() - class_state < LATE_SEC)) {
      for (byte i = 0; i < 4; i++)
        tag += rfid.uid.uidByte[i];
      for (Student* s = &students[0]; s < &students[STUDS_NUM]; s++)
        if (tag == s->tag_id) {
          s->present = true;
          Serial.println(s->name + String(" Entered at ") + timeClient.getFormattedTime());
          servo.write(OPEN_DEG);
          onForSeconds(LED_PIN, 3);
          servo.write(CLOSE_DEG);
        }
      tag = "";
    } else
      onForSeconds(BUZZER_PIN, 1);
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }
}
