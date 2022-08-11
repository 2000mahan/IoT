#include <Servo.h>

#define LDR_RIGHT D1
#define LDR_CENTER D2
#define LDR_LEFT D3
#define SERVO_PIN D4

Servo servo;

unsigned int ldrRightValue = 0;
unsigned int ldrCenterValue = 0;
unsigned int ldrLeftValue = 0;


void setup() {
  pinMode(LDR_RIGHT, INPUT);
  pinMode(LDR_CENTER, INPUT);
  pinMode(LDR_LEFT, INPUT);
  servo.attach(SERVO_PIN);
  Serial.begin(9600);
}

void loop() {
  ldrRightValue = digitalRead(LDR_RIGHT);
  ldrCenterValue = digitalRead(LDR_CENTER);
  ldrLeftValue = digitalRead(LDR_LEFT);
  
  if(ldrRightValue == 1)
    servo.write(180);
  else if(ldrCenterValue == 1)
    servo.write(90);
  else
    servo.write(0);
    
  Serial.printf("Right: %d\n", ldrRightValue);
  Serial.printf("Center: %d\n", ldrCenterValue);
  Serial.printf("Left: %d\n", ldrLeftValue);
  delay(1000);
}
