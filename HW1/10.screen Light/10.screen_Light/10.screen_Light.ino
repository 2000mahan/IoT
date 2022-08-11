#define analogPin A0
#define LED_PIN D3

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  Serial.println(analogRead(analogPin));
  analogWrite(LED_PIN, map(analogRead(analogPin), 400, 900, 255, 0));
  delay(50);
}
