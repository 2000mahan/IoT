#define LDR_PIN A0
#define BUZZER_PIN D8

void setup() { 
  pinMode(BUZZER_PIN, OUTPUT); 
}

void loop(){
  Serial.println(analogRead(LDR_PIN));
  if (analogRead(LDR_PIN) < 650)
    digitalWrite(BUZZER_PIN, HIGH);
  else
    digitalWrite(BUZZER_PIN, LOW);
  
  delay(100);
}
