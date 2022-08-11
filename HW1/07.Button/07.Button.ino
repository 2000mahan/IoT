#define analogPin A0 // ESP8266 Analog Pin ADC0 = A0
#define threshold 700

bool LEDState;
bool buttonState, preButtonState;

void setup()
{
  LEDState = false;
  preButtonState = false;
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  buttonState = analogRead(analogPin) > threshold;
  Serial.println(analogRead(analogPin));
  LEDState = (buttonState && !preButtonState)? !LEDState : LEDState;
  digitalWrite(LED_BUILTIN, LEDState);
  preButtonState = buttonState;
  delay(100);
}
