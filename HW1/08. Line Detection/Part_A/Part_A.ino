#define analogPin A0
#define LED_PIN D3

int value;
int mappedValue;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  value = analogRead(analogPin);
  Serial.print("LDR Output = ");
  Serial.println(value);
  mappedValue = map(value, 250, 900, 100, 0);
  Serial.print("Mapped Value = ");
  Serial.println(mappedValue);
  analogWrite(LED_PIN, mappedValue);
  delay(200);
}
