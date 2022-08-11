#include <Arduino.h>

#include <NewPing.h>
#include <Servo.h>

#define SERVO_PIN    D4
#define ECHO_PIN     D5
#define TRIGGER_PIN  D6
#define MAX_DISTANCE 400

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
Servo servo;

unsigned int duration = 0;
unsigned int lastTime = 0;

void setup() {
  servo.attach(SERVO_PIN);
  Serial.begin(9600);
}

void loop() {
  int distance = sonar.ping_cm();
  duration = (distance < 10)? duration + 1 : 0;
  if (duration > 20)
    servo.write(170);
  else
    servo.write(10);

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println("cm");

  while (millis() - lastTime < 100);
  lastTime = millis();
}
